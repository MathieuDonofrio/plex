#ifndef PLEX_ASYNC_WHEN_ALL_H
#define PLEX_ASYNC_WHEN_ALL_H

#include <atomic>

#include "plex/async/trigger_task.h"
#include "plex/containers/vector.h"

namespace plex
{
///
/// Concept required to be a trigger for a when all.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept WhenAllTrigger = Trigger<Type> && requires(Type trigger, std::coroutine_handle<> awaiting) {
                                            {
                                              trigger.TryAwait(awaiting)
                                              } -> std::convertible_to<bool>;
                                          };

///
/// Awaiter for a when all trigger.
///
/// Sets the awaiting coroutine as the callback for the trigger. Will not suspend if the trigger is already ready.
///
/// @tparam Trigger Trigger type to await.
///
template<WhenAllTrigger Trigger>
class WhenAllTriggerAwaiter
{
public:
  ///
  /// Constructor.
  ///
  /// @param[in] trigger Trigger to await on.
  ///
  WhenAllTriggerAwaiter(Trigger& trigger) : trigger_(trigger) {}

  ///
  /// Called before suspending to check if we should avoid suspending.
  ///
  /// @return Always false
  ///
  bool await_ready() const noexcept
  {
    return false;
  }

  ///
  /// Called after suspension. Tries to set the awaiting coroutine as the callback. Will suspend only if the trigger is
  /// not ready.
  ///
  /// @param[in] awaiting Awaiting coroutine.
  ///
  bool await_suspend(std::coroutine_handle<> awaiting) noexcept
  {
    return trigger_.TryAwait(awaiting);
  }

  ///
  /// Does nothing.
  ///
  void await_resume() const noexcept {}

private:
  Trigger& trigger_;
};

///
/// When all counter trigger that uses an atomic counter to count down the events needed to be fired.
///
/// Best for awaiting on multiple awaitable.
///
class WhenAllCounter
{
public:
  ///
  /// Constructor.
  ///
  /// @param amount Amount of events needed to be fired.
  ///
  constexpr WhenAllCounter(size_t amount) : counter_(amount) {}

  ///
  /// Sets the awaiter as the continuation and awaits until the counter hits zero.
  ///
  /// @return Awaiter.
  ///
  auto operator co_await() noexcept
  {
    return WhenAllTriggerAwaiter<WhenAllCounter> { *this };
  }

  ///
  /// Sets the awaiting handle as the continuation and returns whether or not the counter is at zero.
  ///
  /// @param[in] awaiting Awaiting coroutine.
  ///
  /// @return True if trigger already done, false otherwise.
  ///
  bool TryAwait(std::coroutine_handle<> awaiting) noexcept
  {
    continuation_ = awaiting;
    return counter_.fetch_sub(1, std::memory_order_release) != 0;
  }

  ///
  /// Decrements the counter and resumes the continuation if the continuation was set and the counter is at zero.
  ///
  void Fire() noexcept
  {
    if (counter_.fetch_sub(1, std::memory_order_acquire) == 0) continuation_.resume();
  }

private:
  std::atomic_size_t counter_;
  std::coroutine_handle<> continuation_;
};

///
/// When all trigger that uses an atomic flag.
///
/// Best for awaiting on a single awaitable.
///
class WhenAllFlag
{
public:
  ///
  /// Constructor.
  ///
  constexpr WhenAllFlag() : flag_(false) {}

  ///
  /// Sets the awaiter as the continuation and awaits until the flag is set.
  ///
  /// @return Awaiter.
  ///
  auto operator co_await() noexcept
  {
    return WhenAllTriggerAwaiter<WhenAllFlag> { *this };
  }

  ///
  /// Sets the awaiting handle as the continuation and returns whether or not the the flag was set.
  ///
  /// @param[in] awaiting Awaiting coroutine.
  ///
  /// @return True if trigger already done, false otherwise.
  ///
  bool TryAwait(std::coroutine_handle<> awaiting) noexcept
  {
    continuation_ = awaiting;
    return !flag_.exchange(true, std::memory_order_release);
  }

  ///
  /// Sets the flag and resumes the continuation if the continuation was set.
  ///
  void Fire()
  {
    if (flag_.exchange(true, std::memory_order_acquire)) continuation_.resume();
  }

private:
  std::atomic_bool flag_;
  std::coroutine_handle<> continuation_;
};

///
/// Creates a new awaitable that completes when all of the input awaitables are complete. If the awaitables complete
/// asynchronously, they will all be executed concurrently.
///
/// This overload does may have some iteration and memory allocation overhead. Prefer using the variadic version of this
/// function when possible.
///
/// @tparam Awaitables Container type of awaitables.
/// @tparam Awaitable Awaitable type contained by the container (defaults to Awaitables::value_type).
///
/// @param[in] awaitables Container of awaitables
///
/// @return Task that co_awaits all awaitables in container.
///
template<std::ranges::input_range Awaitables, Awaitable Awaitable = typename Awaitables::value_type>
Task<> WhenAll(Awaitables awaitables)
{
  const size_t amount = std::ranges::size(awaitables);

  WhenAllCounter when_all_counter(amount);

  Vector<TriggerTask<void, WhenAllCounter>> trigger_tasks;
  trigger_tasks.reserve(amount);

  for (auto&& awaitable : awaitables)
  {
    trigger_tasks.push_back(MakeTriggerTask<WhenAllCounter, Awaitable, void>(std::move(awaitable)));
    trigger_tasks.back().Start(when_all_counter);
  }

  co_await when_all_counter;
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete. If the awaitables complete
/// asynchronously, they will all be executed concurrently.
///
/// This overload does nothing because there are no awaitable, but is here for ease of use in certain cases with
/// variadic templates.
///
/// @return Task that does nothing.
///
inline Task<> WhenAll()
{
  co_return;
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete. If the awaitables complete
/// asynchronously, they will all be executed concurrently.
///
/// This overload simply co_awaits the awaitable.
///
/// @tparam Awaitable Awaitable type to co_await.
///
/// @param[in] awaitable Awaitable to co_await.
///
/// @return Task that co_awaits the awaitable.
///
template<Awaitable Awaitable>
Task<> WhenAll(Awaitable&& awaitable)
{
  if constexpr (WhenReadyAwaitable<Awaitable>) co_await std::forward<Awaitable>(awaitable).WhenReady();
  else
  {
    co_await std::forward<Awaitable>(awaitable);
  }
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete. If the awaitables complete
/// asynchronously, they will all be executed concurrently.
///
/// This overload co_awaits both awaitables and uses a flag instead of a counter for slightly better performance.
///
/// @tparam FirstAwaitable First awaitable type to co_await.
/// @tparam SecondAwaitable Second awaitable type to co_await.
///
/// @param[in] first_awaitable First awaitable to co_await.
/// @param[in] second_awaitable Second awaitable to co_await.
///
/// @return Task that co_awaits both awaitables.
///
template<Awaitable FirstAwaitable, Awaitable SecondAwaitable>
Task<> WhenAll(FirstAwaitable&& first_awaitable, SecondAwaitable&& second_awaitable)
{
  WhenAllFlag when_all_flag;

  auto trigger_task = MakeTriggerTask<WhenAllFlag>(std::forward<FirstAwaitable>(first_awaitable));
  trigger_task.Start(when_all_flag);

  if constexpr (WhenReadyAwaitable<SecondAwaitable>)
    co_await std::forward<SecondAwaitable>(second_awaitable).WhenReady();
  else
    co_await std::forward<SecondAwaitable>(second_awaitable);

  co_await when_all_flag;
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete. If the awaitables complete
/// asynchronously, they will all be executed concurrently.
///
/// @tparam Awaitables All awaitable types to co_await.
///
/// @param[in] awaitables All awaitables to co_await.
///
/// @return Task that co_awaits all awaitables.
///
template<Awaitable... Awaitables>
Task<> WhenAll(Awaitables&&... awaitables)
{
  WhenAllCounter when_all_counter(sizeof...(Awaitables));

  // We need to store tasks somewhere to not call the destructor
  [[maybe_unused]] auto trigger_tasks = std::make_tuple(
    [&when_all_counter](Awaitables&& awaitable)
    {
      auto trigger_task = MakeTriggerTask<WhenAllCounter>(std::forward<Awaitables>(awaitable));
      trigger_task.Start(when_all_counter);
      return trigger_task;
    }(std::forward<Awaitables>(awaitables))...);

  co_await when_all_counter;
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete and returns an aggregate of
/// the results. If the awaitables complete asynchronously, they will all be executed concurrently.
///
/// This overload does nothing because there are no awaitable, but is here for ease of use in certain cases with
/// variadic templates.
///
/// @return Task that does nothing and returns empty aggregate results.
///
inline Task<AgrAwaitResult<>> CollectAll()
{
  co_return AgrAwaitResult<> {};
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete and returns an aggregate of the
/// results. If the awaitables complete asynchronously, they will all be executed concurrently.
///
/// This overload simply co_awaits the awaitable.
///
/// @note If the result type of the input awaitables are void, the aggregate result will return VoidAwaitResult for that
/// awaitable.
///
/// @tparam Awaitable Awaitable type to co_await.
///
/// @param[in] awaitable Awaitable to co_await.
///
/// @return Task that co_awaits the awaitable and returns an aggregate of the results.
///
template<Awaitable Awaitable>
Task<AgrAwaitResult<Awaitable>> CollectAll(Awaitable&& awaitable)
{
  if constexpr (std::is_void_v<typename AwaitableTraits<Awaitable>::AwaitResultType>)
  {
    if constexpr (WhenReadyAwaitable<Awaitable>) co_await std::forward<Awaitable>(awaitable).WhenReady();
    else
      co_await std::forward<Awaitable>(awaitable);

    co_return AgrAwaitResult<Awaitable>(VoidAwaitResult {});
  }
  else
  {
    co_return AgrAwaitResult<Awaitable>(co_await std::forward<Awaitable>(awaitable));
  }
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete and returns an aggregate of the
/// results. If the awaitables complete asynchronously, they will all be executed concurrently.
///
/// This overload co_awaits both awaitables and uses a flag instead of a counter for slightly better performance.
///
/// @note If the result type of the input awaitables are void, the aggregate result will return VoidAwaitResult for that
/// awaitable.
///
/// @tparam FirstAwaitable First awaitable type to co_await.
/// @tparam SecondAwaitable Second awaitable type to co_await.
///
/// @param[in] first_awaitable First awaitable to co_await.
/// @param[in] second_awaitable Second awaitable to co_await.
///
/// @return Task that co_awaits both awaitables and returns an aggregate of the results.
///
template<Awaitable FirstAwaitable, Awaitable SecondAwaitable>
Task<AgrAwaitResult<FirstAwaitable, SecondAwaitable>> CollectAll(
  FirstAwaitable&& first_awaitable, SecondAwaitable&& second_awaitable)
{
  WhenAllFlag when_all_flag;

  auto trigger_task = MakeTriggerTask<WhenAllFlag>(std::forward<FirstAwaitable>(first_awaitable));
  trigger_task.Start(when_all_flag);

  if constexpr (std::is_void_v<typename AwaitableTraits<SecondAwaitable>::AwaitResultType>)
  {
    if constexpr (WhenReadyAwaitable<SecondAwaitable>)
      co_await std::forward<SecondAwaitable>(second_awaitable).WhenReady();
    else
      co_await std::forward<SecondAwaitable>(second_awaitable);

    co_await when_all_flag;

    if constexpr (std::is_void_v<typename AwaitableTraits<FirstAwaitable>::AwaitResultType>)
      co_return AgrAwaitResult<FirstAwaitable, SecondAwaitable>(VoidAwaitResult {}, VoidAwaitResult {});
    else
      co_return AgrAwaitResult<FirstAwaitable, SecondAwaitable>(std::move(trigger_task).Result(), VoidAwaitResult {});
  }
  else
  {
    auto result = co_await std::forward<SecondAwaitable>(second_awaitable);
    co_await when_all_flag;

    if constexpr (std::is_void_v<typename AwaitableTraits<FirstAwaitable>::AwaitResultType>)
      co_return AgrAwaitResult<FirstAwaitable, SecondAwaitable>(VoidAwaitResult {}, std::move(result));
    else
      co_return AgrAwaitResult<FirstAwaitable, SecondAwaitable>(std::move(trigger_task).Result(), std::move(result));
  }
}

///
/// Creates a new awaitable that completes when all of the input awaitables are complete and returns an aggregate of the
/// results. If the awaitables complete asynchronously, they will all be executed concurrently.
///
/// @note If the result type of the input awaitables are void, the aggregate result will return VoidAwaitResult for that
/// awaitable.
///
/// @tparam Awaitables All awaitable types to co_await.
///
/// @param[in] awaitables All awaitables to co_await.
///
/// @return Task that co_awaits all awaitables and returns an aggregate of the results.
///
template<Awaitable... Awaitables>
Task<AgrAwaitResult<Awaitables...>> CollectAll(Awaitables&&... awaitables)
{
  WhenAllCounter when_all_counter(sizeof...(Awaitables));

  [[maybe_unused]] auto trigger_tasks = std::make_tuple(
    [&when_all_counter](Awaitables&& awaitable)
    {
      auto trigger_task = MakeTriggerTask<WhenAllCounter>(std::forward<Awaitables>(awaitable));
      trigger_task.Start(when_all_counter);
      return trigger_task;
    }(std::forward<Awaitables>(awaitables))...);

  co_await when_all_counter;

  // Transform trigger task tuple to results tuple
  co_return std::apply(
    [](auto&&... tasks)
    {
      return AgrAwaitResult<Awaitables...>(
        [](auto&& task)
        {
          if constexpr (std::is_void_v<decltype(task.Result())>) return VoidAwaitResult {};
          else
          {
            return std::move(task).Result();
          }
        }(std::move(tasks))...);
    },
    std::move(trigger_tasks));
}
} // namespace plex

#endif
