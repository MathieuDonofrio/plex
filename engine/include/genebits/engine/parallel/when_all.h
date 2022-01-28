#ifndef GENEBITS_ENGINE_PARALLEL_WHEN_ALL_H
#define GENEBITS_ENGINE_PARALLEL_WHEN_ALL_H

#include <atomic>

#include "genebits/engine/parallel/task.h"
#include "genebits/engine/util/fast_vector.h"

namespace genebits::engine
{
// TODO WhenAllReady

template<typename Impl>
concept WhenAllTrigger = requires(Impl event, std::coroutine_handle<> awaiting)
{
  {
    event.IsReady()
    } -> std::convertible_to<bool>;
  {
    event.TryAwait(awaiting)
    } -> std::convertible_to<bool>;
  event.Notify();
};

template<WhenAllTrigger Trigger>
class WhenAllTriggerAwaiter
{
public:
  WhenAllTriggerAwaiter(Trigger& trigger) : trigger_(trigger) {}

  bool await_ready() const noexcept
  {
    return trigger_.IsReady();
  }

  bool await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept
  {
    return trigger_.TryAwait(awaiting_coroutine);
  }

  void await_resume() const noexcept {}

private:
  Trigger& trigger_;
};

class WhenAllCounter
{
public:
  constexpr WhenAllCounter(size_t amount) : counter_(amount) {}

  auto operator co_await() noexcept
  {
    return WhenAllTriggerAwaiter<WhenAllCounter> { *this };
  }

  bool IsReady() noexcept
  {
    return static_cast<bool>(continuation_);
  }

  bool TryAwait(std::coroutine_handle<> awaiting) noexcept
  {
    continuation_ = awaiting;
    return counter_.fetch_sub(1, std::memory_order_release) != 0;
  }

  void Notify() noexcept
  {
    if (counter_.fetch_sub(1, std::memory_order_acquire) == 0) continuation_.resume();
  }

private:
  std::atomic_size_t counter_;
  std::coroutine_handle<> continuation_;
};

class WhenAllFlag
{
public:
  constexpr WhenAllFlag() : flag_(false) {}

  auto operator co_await() noexcept
  {
    return WhenAllTriggerAwaiter<WhenAllFlag> { *this };
  }

  bool IsReady() noexcept
  {
    return static_cast<bool>(continuation_);
  }

  bool TryAwait(std::coroutine_handle<> awaiting) noexcept
  {
    continuation_ = awaiting;
    // Maybe we can just do a load here?
    return !flag_.exchange(true, std::memory_order_release);
  }

  void Notify()
  {
    // Maybe we can just do a store here?
    if (flag_.exchange(true, std::memory_order_acquire)) continuation_.resume();
  }

private:
  std::atomic_bool flag_;
  std::coroutine_handle<> continuation_;
};

template<WhenAllTrigger Trigger>
class WhenAllTask : public TaskBase
{
public:
  class WhenAllTaskPromise;

  using promise_type = WhenAllTaskPromise;
  using handle_type = std::coroutine_handle<WhenAllTaskPromise>;

  class WhenAllTaskPromise
  {
  public:
    WhenAllTaskPromise() : trigger_(nullptr) {}

    struct FinalAwaiter
    {
      bool await_ready() const noexcept
      {
        return false;
      }

      void await_suspend(handle_type handle) noexcept
      {
        // TODO assert trigger non-null
        handle.promise().trigger_->Notify();
      }

      void await_resume() const noexcept {}
    };

    std::suspend_always initial_suspend() const noexcept
    {
      return {};
    }

    FinalAwaiter final_suspend() const noexcept
    {
      return {};
    }

    WhenAllTask get_return_object() noexcept
    {
      return WhenAllTask { handle_type::from_promise(*this) };
    }

    void return_void() noexcept {}

    void SetTrigger(Trigger* trigger)
    {
      trigger_ = trigger;
    }

    COROUTINE_UNHANDLED_EXCEPTION;

  private:
    Trigger* trigger_;
  };

  explicit WhenAllTask(handle_type handle) : TaskBase(handle) {}

  void Start(Trigger& trigger)
  {
    Handle<WhenAllTaskPromise>().promise().SetTrigger(&trigger);
    Handle<WhenAllTaskPromise>().resume();
  }
};

template<typename Trigger, typename Awaitable>
WhenAllTask<Trigger> MakeWhenAllTask(Awaitable&& awaitable)
{
  co_await awaitable;
}

template<std::ranges::range Awaitables>
Task<> WhenAll(const Awaitables& awaitables)
{
  const size_t amount = std::ranges::size(awaitables);

  WhenAllCounter when_all_counter(amount);

  FastVector<WhenAllTask<WhenAllCounter>> tasks;
  tasks.Reserve(amount);

  for (auto& awaitable : awaitables)
  {
    tasks.PushBack(MakeWhenAllTask<WhenAllCounter>(awaitable));
    tasks.back().Start(when_all_counter);
  }

  co_await when_all_counter;
}

template<typename Iterator>
requires(Awaitable<decltype(*Iterator {})>) Task<> WhenAll(Iterator begin, Iterator end)
{
  const size_t amount = std::distance(begin, end);

  WhenAllCounter when_all_counter(amount);

  FastVector<WhenAllTask<WhenAllCounter>> tasks;
  tasks.Reserve(amount);

  for (; begin != end; ++begin)
  {
    tasks.PushBack(MakeWhenAllTask<WhenAllCounter>(*begin));
    tasks.back().Start(when_all_counter);
  }

  co_await when_all_counter;
}

inline Task<> WhenAll()
{
  co_return;
}

template<Awaitable Awaitable>
Task<> WhenAll(Awaitable&& awaitable)
{
  co_await awaitable;
}

template<Awaitable Awaitable1, Awaitable Awaitable2>
Task<> WhenAll(Awaitable1&& awaitable1, Awaitable2&& awaitable2)
{
  WhenAllFlag when_all_flag;

  auto when_all_task = MakeWhenAllTask<WhenAllFlag>(std::forward<Task>(awaitable1));
  when_all_task.Start(when_all_flag);

  co_await awaitable2;

  co_await when_all_flag;
}

template<Awaitable... Awaitables>
Task<> WhenAll(Awaitables&&... awaitables)
{
  WhenAllCounter when_all_counter(sizeof...(Awaitables));

  // We need to store tasks somewhere to not call the destructor
  [[maybe_unused]] auto when_all_tasks = std::make_tuple(
    [&when_all_counter](Awaitables&& awaitable)
    {
      auto when_all_task = MakeWhenAllTask<WhenAllCounter>(std::forward<Awaitables>(awaitable));
      when_all_task.Start(when_all_counter);
      return when_all_task;
    }(std::forward<Awaitables>(awaitables))...);

  co_await when_all_counter;
}
} // namespace genebits::engine

#endif
