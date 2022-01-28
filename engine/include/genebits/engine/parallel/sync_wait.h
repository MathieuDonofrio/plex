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
  constexpr SyncWaitCounter(size_t amount) : counter_(amount) {}

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
  void Fire()
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
  constexpr SyncWaitFlag() : flag_(false) {}

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
  void Fire()
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

///
/// Task used to allow of possibility of a blocking wait. Used to synchronize with a thread, normally the root caller.
///
/// @tparam Trigger Trigger used to fire events and wait on.
///
template<SyncWaitTrigger Trigger>
class SyncWaitTask : public TaskBase
{
public:
  class SyncWaitTaskPromise;

  using promise_type = SyncWaitTaskPromise;
  using handle_type = std::coroutine_handle<SyncWaitTaskPromise>;

  ///
  /// SyncWaitTask promise.
  ///
  class SyncWaitTaskPromise
  {
  public:
    ///
    /// Constructor.
    ///
    SyncWaitTaskPromise() : trigger_(nullptr) {}

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
      void await_suspend(handle_type handle) const noexcept
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
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SyncWaitTask get_return_object() noexcept
    {
      return SyncWaitTask { handle_type::from_promise(*this) };
    }

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
    /// Called by co_return. Does nothing.
    ///
    void return_void() noexcept {}

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
  /// Constructor.
  ///
  /// @param[in] handle Coroutine handle managed by the task.
  ///
  explicit SyncWaitTask(handle_type handle) : TaskBase(handle) {}

  ///
  /// Manually starts/resumes the task after setting the trigger.
  ///
  /// @param[in] trigger Trigger to set.
  ///
  void Start(Trigger& trigger)
  {
    Handle<SyncWaitTaskPromise>().promise().SetTrigger(&trigger);
    Handle<SyncWaitTaskPromise>().resume();
  }
};

///
/// Creates a SyncWaitTask from an awaitable by co_awaiting it. The sync wait task must be started manually.
///
/// @tparam Trigger Trigger type of the SyncWaitTask.
/// @tparam Awaitable Awaitable type to create task from.
///
/// @param[in] awaitable Awaitable to used to create SyncWaitTask.
///
/// @return SyncWaitTask for the awaitable.
///
template<SyncWaitTrigger Trigger, Awaitable Awaitable>
SyncWaitTask<Trigger> MakeSyncWaitTask(Awaitable&& awaitable)
{
  co_await awaitable;
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
void SyncWait(Awaitable&& awaitable)
{
  SyncWaitFlag flag;

  auto sync_wait_task = MakeSyncWaitTask<SyncWaitFlag, Awaitable>(std::forward<Awaitable>(awaitable));

  sync_wait_task.Start(flag);

  flag.Wait();
}

} // namespace genebits::engine

#endif
