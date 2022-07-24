#ifndef GENEBITS_ENGINE_ASYNC_TRIGGER_TASK_H
#define GENEBITS_ENGINE_ASYNC_TRIGGER_TASK_H

#include "genebits/engine/async/task.h"

namespace plex
{
///
/// Concept required to be a trigger.
///
/// @tparam Impl Type to check.
///
template<typename Impl>
concept Trigger = requires(Impl event)
{
  event.Fire();
};

template<typename Type, Trigger Trigger>
class TriggerTask;

namespace details
{
  template<typename Type, Trigger Trigger>
  class TriggerTaskPromise;

  ///
  /// Base for trigger task.
  ///
  /// @tparam Trigger Trigger used to fire events.
  ///
  template<Trigger Trigger>
  class TriggerTaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    TriggerTaskPromiseBase() noexcept : trigger_(nullptr) {}

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
      /// @param[in] handle Coroutine of the task.
      ///
      template<typename Type>
      void await_suspend(std::coroutine_handle<TriggerTaskPromise<Type, Trigger>> handle) const noexcept
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
  /// Trigger task promise.
  ///
  /// @tparam Type The return type of the task.
  /// @tparam Trigger Trigger used to fire events and wait on.
  ///
  template<typename Type, Trigger Trigger>
  class TriggerTaskPromise final : public TriggerTaskPromiseBase<Trigger>
  {
  public:
    ///
    /// Constructor.
    ///
    TriggerTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    TriggerTask<Type, Trigger> get_return_object() noexcept;

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
      return TriggerTaskPromiseBase<Trigger>::final_suspend();
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
  /// Trigger task promise.
  ///
  /// Specialization for void return result type.
  ///
  /// @tparam Type The return type of the task.
  /// @tparam Trigger Trigger used to fire events and wait on.
  ///
  template<Trigger Trigger>
  class TriggerTaskPromise<void, Trigger> final : public TriggerTaskPromiseBase<Trigger>
  {
  public:
    ///
    /// Constructor.
    ///
    TriggerTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    TriggerTask<void, Trigger> get_return_object() noexcept;

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
/// Task that fires a trigger when done.
///
/// @tparam Type The return type of the task.
/// @tparam Trigger Trigger used to fire events and wait on.
///
template<typename Type, Trigger Trigger>
class TriggerTask : public details::TaskBase
{
public:
  using promise_type = details::TriggerTaskPromise<Type, Trigger>;
  using handle_type = std::coroutine_handle<promise_type>;

  ///
  /// Constructor.
  ///
  /// @param[in] handle Coroutine handle managed by the task.
  ///
  TriggerTask(handle_type handle) noexcept : TaskBase(handle) {}

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
  [[nodiscard]] decltype(auto) Result() & noexcept
  {
    return Handle<promise_type>().promise().Result();
  }

  ///
  /// Returns the result of the task. Void if Type is void.
  ///
  /// @return Result of the task.
  ///
  [[nodiscard]] decltype(auto) Result() && noexcept
  {
    return std::move(Handle<promise_type>().promise()).Result();
  }
};

namespace details
{
  // Out of line definitions

  template<typename Type, Trigger Trigger>
  TriggerTask<Type, Trigger> TriggerTaskPromise<Type, Trigger>::get_return_object() noexcept
  {
    return TriggerTask<Type, Trigger> { std::coroutine_handle<TriggerTaskPromise<Type, Trigger>>::from_promise(*this) };
  }

  template<Trigger Trigger>
  TriggerTask<void, Trigger> TriggerTaskPromise<void, Trigger>::get_return_object() noexcept
  {
    return TriggerTask<void, Trigger> { std::coroutine_handle<TriggerTaskPromise<void, Trigger>>::from_promise(*this) };
  }
} // namespace details

///
/// Creates a trigger task from an awaitable by co_awaiting it. The trigger task must be started manually.
///
/// When the awaitable is done, the trigger task will fire the trigger.
///
/// @note If the result type is void, will use the WhenReady awaitable if the awaitable supports it.
///
/// @tparam Trigger Trigger type of the trigger task.
/// @tparam Awaitable Awaitable type to create task from.
/// @tparam Result The result type, defaults to awaitable result type.
///
/// @param[in] awaitable Awaitable to used to create the trigger task.
///
/// @return Created trigger task (initially suspended).
///
template<Trigger Trigger, Awaitable Awaitable, typename Result = typename AwaitableTraits<Awaitable>::AwaitResultType>
auto MakeTriggerTask(Awaitable&& awaitable) -> TriggerTask<Result, Trigger>
{
  if constexpr (std::is_void_v<Result>)
  {
    if constexpr (WhenReadyAwaitable<Awaitable>) co_await std::forward<Awaitable>(awaitable).WhenReady();
    else
    {
      co_await std::forward<Awaitable>(awaitable);
    }
  }
  else
  {
    co_yield co_await std::forward<Awaitable>(awaitable);
  }
}

template<typename Type, typename Trigger>
struct IsTriviallyRelocatable<TriggerTask<Type, Trigger>> : std::true_type
{};
} // namespace plex

#endif
