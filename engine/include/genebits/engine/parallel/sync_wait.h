#ifndef GENEBITS_ENGINE_PARALLEL_SYNC_WAIT_H
#define GENEBITS_ENGINE_PARALLEL_SYNC_WAIT_H

#include <atomic>

#include "genebits/engine/parallel/task.h"

namespace genebits::engine
{
///
/// Concept required to be a trigger for a SyncWaitTask.
///
/// @tparam Impl Type to check.
///
template<typename Impl>
concept SyncWaitTrigger = requires(Impl event)
{
  event.Wait();
  event.Fire();
  event.IsDone();
};

///
/// SyncWaitTrigger that uses an atomic counter to count down the events needed to be fired before notifying all
/// waiters.
///
/// Best for waiting on multiple awaitable.
///
class SyncWaitCounter
{
public:
  ///
  /// Constructor.
  ///
  /// @param amount Amount of events needed to be fired to notify waiters.
  ///
  constexpr SyncWaitCounter(size_t amount) noexcept : counter_(amount) {}

  ///
  /// Blocks and waits until the required amount of events have been fired.
  ///
  void Wait() const noexcept
  {
    size_t last;

    while ((last = counter_.load(std::memory_order_acquire)) != 0)
    {
      counter_.wait(last, std::memory_order_relaxed);
    }
  }

  ///
  /// Fires an invent. Will notify all waiters, if there are no more events needed to be fired.
  ///
  void Fire() noexcept
  {
    if (counter_.fetch_sub(1, std::memory_order_acq_rel) == 1) counter_.notify_all();
  }

  ///
  /// Whether or not the trigger is done.
  ///
  /// @return True if trigger is done, false otherwise.
  ///
  [[nodiscard]] bool IsDone() const noexcept
  {
    return counter_.load(std::memory_order_acquire) == 0;
  }

private:
  std::atomic_size_t counter_;
};

///
/// SyncWaitTrigger that uses flips an atomic flag to notify all waiters.
///
/// Best for waiting on a single awaitable.
///
class SyncWaitFlag
{
public:
  ///
  /// Constructor.
  ///
  constexpr SyncWaitFlag() noexcept : flag_(false) {}

  ///
  /// Blocks and waits until the awaitable fires the event.
  ///
  void Wait() const noexcept
  {
    while (!IsDone())
    {
      flag_.wait(false, std::memory_order_acquire);
    }
  }

  ///
  /// Fires the invent. Will notify all waiters.
  ///
  void Fire() noexcept
  {
    flag_.store(true, std::memory_order_release);
    flag_.notify_all();
  }

  ///
  /// Whether or not the trigger is done.
  ///
  /// @return True if trigger is done, false otherwise.
  ///
  [[nodiscard]] bool IsDone() const noexcept
  {
    return flag_.load(std::memory_order_acquire);
  }

private:
  std::atomic_bool flag_;
};

template<typename Type, SyncWaitTrigger Trigger>
class SyncWaitTask;

namespace details
{
  template<typename Type, SyncWaitTrigger Trigger>
  class SyncWaitTaskPromise;

  ///
  /// Base for sync wait task promise.
  ///
  /// @tparam Trigger Trigger used to fire events and wait on.
  ///
  template<SyncWaitTrigger Trigger>
  class SyncWaitTaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    SyncWaitTaskPromiseBase() noexcept : trigger_(nullptr) {}

    ///
    /// Final awaiter. Used to fire event on trigger.
    ///
    struct FinalAwaiter
    {
      ///
      /// Called before suspending to check if we should avoid suspending.
      ///
      /// @return Always false.
      ///
      bool await_ready() const noexcept
      {
        return false;
      }

      ///
      /// Called after suspension. Fires event on trigger.
      ///
      /// @param[in] handle
      ///
      template<typename Type>
      void await_suspend(std::coroutine_handle<SyncWaitTaskPromise<Type, Trigger>> handle) const noexcept
      {
        ASSERT(handle.promise().trigger_, "Trigger should not be nullptr here");
        handle.promise().trigger_->Fire();
      }

      ///
      /// Does nothing.
      ///
      void await_resume() const noexcept {}
    };

    ///
    /// Initially suspended to allow setting the trigger before manually resuming.
    ///
    /// @return Always suspend.
    ///
    std::suspend_always initial_suspend() const noexcept
    {
      return {};
    }

    ///
    /// Returns final awaiter to fire event on trigger.
    ///
    /// @return Final awaiter.
    ///
    FinalAwaiter final_suspend() const noexcept
    {
      return {};
    }

    ///
    /// Sets the trigger for this sync wait task.
    ///
    /// @param[in] trigger Trigger to set.
    ///
    void SetTrigger(Trigger* trigger)
    {
      trigger_ = trigger;
    }

    COROUTINE_UNHANDLED_EXCEPTION;

  private:
    Trigger* trigger_;
  };

  ///
  /// SyncWaitTask promise.
  ///
  /// @tparam Type The return type of the task.
  /// @tparam Trigger Trigger used to fire events and wait on.
  ///
  template<typename Type, SyncWaitTrigger Trigger>
  class SyncWaitTaskPromise final : public SyncWaitTaskPromiseBase<Trigger>
  {
  public:
    ///
    /// Constructor.
    ///
    SyncWaitTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SyncWaitTask<Type, Trigger> get_return_object() noexcept;

    ///
    /// Returns final awaiter to fire event on trigger.
    ///
    /// @param[in] result
    ///
    /// @return Final awaiter
    ///
    auto yield_value(Type&& result) noexcept
    {
      value_ = std::addressof(result);
      return SyncWaitTaskPromiseBase<Trigger>::final_suspend();
    }

    ///
    /// Does nothing.
    ///
    /// Should never be called. Is there is a result value the coroutine should have been yielded.
    ///
    void return_void() const noexcept
    {
      ASSERT(false, "Coroutine should have been yielded");
    }

    ///
    /// Returns the result that was set by yield_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value
    ///
    Type&& Result() const noexcept
    {
      return static_cast<Type&&>(*value_);
    }

  private:
    std::remove_reference_t<Type>* value_;
  };

  ///
  /// SyncWaitTask promise.
  ///
  /// @tparam Type The return type of the task.
  /// @tparam Trigger Trigger used to fire events and wait on.
  ///
  template<SyncWaitTrigger Trigger>
  class SyncWaitTaskPromise<void, Trigger> final : public SyncWaitTaskPromiseBase<Trigger>
  {
  public:
    ///
    /// Constructor.
    ///
    SyncWaitTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SyncWaitTask<void, Trigger> get_return_object() noexcept;

    ///
    /// Does nothing.
    ///
    void return_void() const noexcept {}

    ///
    /// Does nothing
    ///
    void Result() const noexcept {}
  };
} // namespace details

///
/// Task used to allow of possibility of a blocking wait. Used to synchronize with a thread, normally the root caller.
///
/// @tparam Type The return type of the task.
/// @tparam Trigger Trigger used to fire events and wait on.
///
template<typename Type, SyncWaitTrigger Trigger>
class SyncWaitTask : public TaskBase
{
public:
  using promise_type = details::SyncWaitTaskPromise<Type, Trigger>;
  using handle_type = std::coroutine_handle<promise_type>;

  ///
  /// Constructor.
  ///
  /// @param[in] handle Coroutine handle managed by the task.
  ///
  SyncWaitTask(handle_type handle) noexcept : TaskBase(handle) {}

  ///
  /// Manually starts/resumes the task after setting the trigger.
  ///
  /// @param[in] trigger Trigger to set.
  ///
  void Start(Trigger& trigger) const noexcept
  {
    Handle<promise_type>().promise().SetTrigger(&trigger);
    Handle<promise_type>().resume();
  }

  ///
  /// Returns the result of the task. Void if Type is void.
  ///
  /// @return Result of the task.
  ///
  [[nodiscard]] decltype(auto) Result() const noexcept
  {
    return Handle<promise_type>().promise().Result();
  }
};

namespace details
{
  // Out of line definitions

  template<typename Type, SyncWaitTrigger Trigger>
  SyncWaitTask<Type, Trigger> SyncWaitTaskPromise<Type, Trigger>::get_return_object() noexcept
  {
    return SyncWaitTask<Type, Trigger> { std::coroutine_handle<SyncWaitTaskPromise<Type, Trigger>>::from_promise(
      *this) };
  }

  template<SyncWaitTrigger Trigger>
  SyncWaitTask<void, Trigger> SyncWaitTaskPromise<void, Trigger>::get_return_object() noexcept
  {
    return SyncWaitTask<void, Trigger> { std::coroutine_handle<SyncWaitTaskPromise<void, Trigger>>::from_promise(
      *this) };
  }
} // namespace details

///
/// Creates a SyncWaitTask from an awaitable by co_awaiting it. The sync wait task must be started manually.
///
/// @tparam Trigger Trigger type of the SyncWaitTask.
/// @tparam Awaitable Awaitable type to create task from.
/// @tparam Result The result type, defaults to awaitable result type.
///
/// @param[in] awaitable Awaitable to used to create SyncWaitTask.
///
/// @return SyncWaitTask for the awaitable.
///
template<SyncWaitTrigger Trigger,
  Awaitable Awaitable,
  typename Result = typename AwaitableTraits<Awaitable>::AwaitResultType>
SyncWaitTask<Result, Trigger> MakeSyncWaitTask(Awaitable&& awaitable)
{
  if constexpr (std::is_void_v<Result>) co_await std::forward<Awaitable>(awaitable);
  else
  {
    co_yield co_await std::forward<Awaitable>(awaitable);
  }
}

///
/// Creates a SyncWaitTask for the awaitable and synchronously waits until its done.
///
/// Will block on the current thread until the awaitable is done.
///
/// @tparam Awaitable Awaitable type to synchronously wait on.
///
/// @param[in] awaitable To synchronously wait on.
///
template<Awaitable Awaitable>
auto SyncWait(Awaitable&& awaitable) -> typename AwaitableTraits<Awaitable>::AwaitResultType
{
  SyncWaitFlag flag;

  auto sync_wait_task = MakeSyncWaitTask<SyncWaitFlag>(std::forward<Awaitable>(awaitable));

  sync_wait_task.Start(flag);

  flag.Wait();

  return sync_wait_task.Result();
}

} // namespace genebits::engine

#endif
