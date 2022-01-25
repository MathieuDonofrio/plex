#ifndef GENEBITS_ENGINE_PARALLEL_TASK_H
#define GENEBITS_ENGINE_PARALLEL_TASK_H

#include "genebits/engine/parallel/threading.h"
#include "genebits/engine/util/delegate.h"

#include <coroutine>
#include <iostream>
#include <span>

namespace genebits::engine
{
class Task
{
public:
  class TaskPromise;

  using promise_type = TaskPromise;
  using handle_type = std::coroutine_handle<TaskPromise>;

  explicit Task(handle_type handle) : handle_(handle) {}

  ~Task()
  {
    if (handle_) handle_.destroy();
  }

  Task(Task&& other) noexcept : handle_(other.handle_)
  {
    other.handle_ = nullptr;
  }

  Task& operator=(Task&& other) noexcept
  {
    if (std::addressof(other) != this) // TODO copy-swap?
    {
      if (handle_) handle_.destroy();

      handle_ = other.handle_;
      other.handle_ = nullptr;
    }

    return *this;
  }

  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  ///
  /// co_await operator.
  ///
  /// Sets the caller coroutine as the continuation of this task and starts the task.
  ///
  auto operator co_await() noexcept
  {
    class TaskAwaiter
    {
    public:
      TaskAwaiter(handle_type handle) : handle_(handle) {}

      bool await_ready() const noexcept
      {
        return !handle_ || handle_.done();
      }

      std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept
      {
        handle_.promise().SetContinuation(awaiting_coroutine);
        return handle_;
      }

      void await_resume() const noexcept {}

    private:
      handle_type handle_;
    };

    return TaskAwaiter { handle_ };
  }

public:
  class TaskPromise
  {
  public:
    TaskPromise() noexcept : continuation_(std::noop_coroutine()) {}

    Task get_return_object() noexcept
    {
      return Task { handle_type::from_promise(*this) };
    };

    std::suspend_always initial_suspend() const noexcept
    {
      return {};
    }

    auto final_suspend() const noexcept
    {
      struct FinalAwaitable
      {
        bool await_ready() const noexcept
        {
          return false;
        }

        std::coroutine_handle<> await_suspend(handle_type handle) noexcept
        {
          return handle.promise().continuation_;
        }

        void await_resume() noexcept {}
      };

      return FinalAwaitable {};
    }

    void return_void() noexcept {}

    void unhandled_exception() noexcept
    {
      std::terminate();
    }

    void SetContinuation(std::coroutine_handle<> continuation) noexcept
    {
      continuation_ = continuation;
    }

  private:
    std::coroutine_handle<> continuation_;
  };

private:
  handle_type handle_;
};

class SyncCounter
{
public:
  constexpr SyncCounter(size_t amount) : counter_(amount) {}

  void Wait() const noexcept
  {
    size_t last;

    while ((last = counter_.load(std::memory_order_relaxed)) != 0)
    {
      counter_.wait(last, std::memory_order_relaxed);
    }
  }

  void Set()
  {
    if (counter_.fetch_sub(1, std::memory_order_acq_rel) == 1) { counter_.notify_all(); }
  }

  [[nodiscard]] bool Finished() const noexcept
  {
    return counter_.load(std::memory_order_relaxed) == 0;
  }

private:
  std::atomic_size_t counter_;
};

class SyncFlag
{
public:
  constexpr SyncFlag() : flag_(false) {}

  void Wait() const noexcept
  {
    while (!Finished())
    {
      flag_.wait(false, std::memory_order_relaxed);
    }
  }

  void Set()
  {
    flag_.store(true, std::memory_order_relaxed);
    flag_.notify_all();
  }

  [[nodiscard]] bool Finished() const noexcept
  {
    return flag_.load(std::memory_order_relaxed);
  }

private:
  std::atomic_bool flag_;
};

template<typename SyncType>
class Sync
{
public:
  class SyncPromise;

  using promise_type = SyncPromise;
  using handle_type = std::coroutine_handle<SyncPromise>;

  Sync(handle_type handle) : handle_(handle) {}

  ~Sync()
  {
    if (handle_) { handle_.destroy(); }
  }

  Sync(Sync&& other) noexcept : handle_(other.handle_)
  {
    other.handle_ = nullptr;
  }

  Sync& operator=(Sync&& other) noexcept
  {
    if (std::addressof(other) != this) // TODO copy-swap?
    {
      if (handle_) handle_.destroy();

      handle_ = other.handle_;
      other.handle_ = nullptr;
    }

    return *this;
  }

  Sync(const Sync&) = delete;
  Sync& operator=(const Sync&) = delete;

  void Start(SyncType& flag)
  {
    handle_.promise().SetEvent(&flag);
    handle_.resume();
  }

public:
  class SyncPromise
  {
  public:
    SyncPromise() : event_(nullptr) {}

    struct FinalAwaitable
    {
      bool await_ready() const noexcept
      {
        return false;
      }

      void await_suspend(handle_type handle) const noexcept
      {
        auto* const event = handle.promise().event_;

        if (event) event->Set();
      }

      constexpr void await_resume() const noexcept {}
    };

    std::suspend_always initial_suspend() const noexcept
    {
      return {};
    }

    FinalAwaitable final_suspend() const noexcept
    {
      return {};
    }

    Sync get_return_object() noexcept
    {
      return { handle_type::from_promise(*this) };
    }

    void return_void() noexcept {}

    void unhandled_exception() noexcept
    {
      std::terminate();
    }

    void SetEvent(SyncType* event)
    {
      event_ = event;
    }

  private:
    SyncType* event_;
  };

private:
  handle_type handle_;
};

template<typename SyncType>
Sync<SyncType> MakeSync(Task& task)
{
  co_await task;
}

} // namespace genebits::engine

#endif
