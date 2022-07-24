#ifndef GENEBITS_ENGINE_ASYNC_SYNC_WAIT_H
#define GENEBITS_ENGINE_ASYNC_SYNC_WAIT_H

#include <atomic>

#include "genebits/engine/async/trigger_task.h"

namespace plex
{
///
/// Concept required to be a trigger for a sync wait.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept SyncWaitTrigger = Trigger<Type> && requires(Type trigger)
{
  trigger.Wait();
  trigger.IsDone();
};

///
/// Sync wait trigger that uses an atomic counter to count down the events needed to be fired before notifying all
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
/// Sync wait trigger that uses flips an atomic flag to notify all waiters.
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

///
/// Creates a sync wait task for the awaitable and synchronously waits until its done.
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

  auto trigger_task = MakeTriggerTask<SyncWaitFlag>(std::forward<Awaitable>(awaitable));

  trigger_task.Start(flag);

  flag.Wait();

  return trigger_task.Result();
}

} // namespace plex

#endif
