#ifndef GENEBITS_ENGINE_ASYNC_TASK_H
#define GENEBITS_ENGINE_ASYNC_TASK_H

#include <thread>

#include "genebits/engine/async/awaitable.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/type_traits.h"

namespace plex
{
template<typename Type = void>
class Task;

namespace details
{
  ///
  /// Used as a base class for coroutine tasks.
  ///
  /// Manages a coroutine handle.
  ///
  class TaskBase
  {
  public:
    ///
    /// Constructor.
    ///
    /// @param[in] handle The coroutine handle for the task.
    ///
    explicit TaskBase(std::coroutine_handle<> handle) noexcept : handle_(handle) {}

    ///
    /// Destructor.
    ///
    ~TaskBase() noexcept
    {
      if (handle_) handle_.destroy();
    }

    TaskBase(const TaskBase&) = delete;
    TaskBase& operator=(const TaskBase&) = delete;

    ///
    /// Move constructor.
    ///
    /// @param[in] other Other task base to move.
    ///
    TaskBase(TaskBase&& other) noexcept : handle_(other.handle_)
    {
      other.handle_ = nullptr;
    }

    ///
    /// Move assignment operator.
    ///
    /// @param[in] other Other task base to move.
    ///
    /// @return Reference of this.
    ///
    TaskBase& operator=(TaskBase&& other) noexcept
    {
      TaskBase(std::move(other)).Swap(*this);
      return *this;
    }

    ///
    /// Swaps handles with another task base.
    ///
    /// @param[in] other Other task base.
    ///
    void Swap(TaskBase& other) noexcept
    {
      std::swap(handle_, other.handle_);
    }

  protected:
    ///
    /// Returns the typed coroutine handle.
    ///
    /// @warning Undefined behaviour if the promise is not correct.
    ///
    /// @tparam PromiseType The type of the promise.
    ///
    /// @return The typed coroutine handle.
    ///
    template<typename PromiseType>
    [[nodiscard]] std::coroutine_handle<PromiseType> Handle() const noexcept
    {
      return std::coroutine_handle<PromiseType>::from_address(handle_.address());
    }

    ///
    /// Returns the void coroutine handle.
    ///
    /// @return Void coroutine handle.
    ///
    [[nodiscard]] std::coroutine_handle<> Handle() const noexcept
    {
      return handle_;
    }

  private:
    std::coroutine_handle<> handle_;
  };

  template<typename Type>
  class TaskPromise;

  ///
  /// Base for the task promise.
  ///
  class TaskPromiseBase
  {
  public:
    ///
    /// Final awaiter. Used to resume continuation.
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
      /// Called after suspension. Resumes the continuation of the task.
      ///
      /// @tparam Type Type for task promise.
      ///
      /// @param[in] handle Coroutine of the task.
      ///
      /// @return Continuation handle to resume.
      ///
      template<typename Type>
      std::coroutine_handle<> await_suspend(std::coroutine_handle<TaskPromise<Type>> handle) const noexcept
      {
        return handle.promise().continuation_;
      }

      ///
      /// Does nothing.
      ///
      void await_resume() const noexcept {}
    };

    ///
    /// Constructor
    ///
    constexpr TaskPromiseBase() noexcept = default;

    ///
    /// Initially suspended to make tasks lazily executed. Task must resume from its initial suspension
    /// to be executed.
    ///
    /// @return Always suspend.
    ///
    std::suspend_always initial_suspend() const noexcept
    {
      return {};
    }

    ///
    /// Returns final awaiter to resume continuation.
    ///
    /// @return Final awaiter.
    ///
    FinalAwaiter final_suspend() const noexcept
    {
      return {};
    }

    ///
    /// Sets the continuation for the task.
    ///
    /// @param[in] continuation Task continuation coroutine.
    ///
    void SetContinuation(std::coroutine_handle<> continuation) noexcept
    {
      continuation_ = continuation;
    }

    COROUTINE_UNHANDLED_EXCEPTION

  private:
    std::coroutine_handle<> continuation_;
  };

  ///
  /// Task promise.
  ///
  /// @tparam Type Result type of the task.
  ///
  template<typename Type>
  class TaskPromise final : public TaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    TaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    Task<Type> get_return_object() noexcept;

    ///
    /// Called by co_return to set the return value.
    ///
    /// @param[in] value Value to set.
    ///
    template<typename T>
    requires std::is_convertible_v<T&&, Type>
    void return_value(T&& value) noexcept(std::is_nothrow_constructible_v<T, T&&>)
    {
      new (static_cast<void*>(std::addressof(value_))) Type(std::forward<T>(value));
    }

    ///
    /// Returns the result that was set by return_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value (lvalue).
    ///
    Type& Result() & noexcept
    {
      return value_;
    }

    ///
    /// Returns the result that was set by return_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value (rvalue).
    ///
    Type&& Result() && noexcept
    {
      return std::move(value_);
    }

  private:
    Type value_;
  };

  ///
  /// Task promise specialization for reference types.
  ///
  /// @tparam Type Task result type.
  ///
  template<typename Type>
  class TaskPromise<Type&> final : public TaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    TaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    Task<Type&> get_return_object() noexcept;

    ///
    /// Called by co_return to set the return value.
    ///
    /// @param[in] value Value to set.
    ///
    void return_value(Type& value) noexcept
    {
      value_ = std::addressof(value);
    }

    ///
    /// Returns the result that was set by return_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value.
    ///
    Type& Result() noexcept
    {
      return *value_;
    }

  private:
    Type* value_;
  };

  ///
  /// Task promise specialization for void.
  ///
  template<>
  class TaskPromise<void> final : public TaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    TaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    Task<void> get_return_object() noexcept;

    ///
    /// Does nothing.
    ///
    void return_void() const noexcept {}

    ///
    /// Does nothing.
    ///
    void Result() const noexcept {}
  };

  ///
  /// Task awaiter base.
  ///
  /// @tparam Type Task result type.
  ///
  template<typename Type>
  class TaskAwaiterBase
  {
  public:
    ///
    /// Constructor.
    ///
    /// @param handle Task coroutine handle.
    ///
    TaskAwaiterBase(std::coroutine_handle<TaskPromise<Type>> handle) noexcept : handle_(handle) {}

    ///
    /// Called before suspending to check if we should avoid suspending. If the coroutine is already done, then we dont
    /// need to suspend.
    ///
    /// @return True if the coroutine is done, false otherwise.
    ///
    bool await_ready() const noexcept
    {
      return !handle_ || handle_.done();
    }

    ///
    /// Called after suspension. Sets the continuation of the task to the awaiting coroutine and resumes the task
    /// coroutine.
    ///
    /// @param[in] awaiting The awaiting coroutine.
    ///
    /// @return Continuation handle to resume.
    ///
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) noexcept
    {
      handle_.promise().SetContinuation(awaiting);
      return handle_;
    }

  protected:
    std::coroutine_handle<TaskPromise<Type>> handle_;
  };
} // namespace details

///
/// A task represents an asynchronous computation. The basic building block of the async coroutine workflow.
///
/// Tasks are lazily executed. The execution of the coroutine does not start until the task is awaited.
///
/// @note Inspired by cppcoro implementation: https://github.com/lewissbaker/cppcoro
///
/// @tparam Type The result/return type of the task.
///
template<typename Type>
class Task : public details::TaskBase
{
public:
  using promise_type = details::TaskPromise<Type>;
  using handle_type = std::coroutine_handle<promise_type>;
  using value_type = Type;

  ///
  /// Constructor.
  ///
  /// @param[in] handle Coroutine handle managed by the task.
  ///
  explicit Task(handle_type handle) noexcept : TaskBase(handle) {}

  ///
  /// Starts the task and awaits it. The current coroutine will be resumed when the task is done.
  ///
  /// @note The coroutine will be resumed on the thread that executed the task.
  ///
  auto operator co_await() const& noexcept
  {
    struct TaskAwaiter final : public details::TaskAwaiterBase<Type>
    {
      decltype(auto) await_resume() noexcept
      {
        return this->handle_.promise().Result();
      }
    };

    return TaskAwaiter { Handle<promise_type>() };
  }

  ///
  /// Starts the task and waits until it is done.
  ///
  /// The current coroutine will be resumed when the task is done.
  ///
  /// @note The coroutine will be resumed on the thread that executed the task.
  ///
  auto operator co_await() const&& noexcept
  {
    struct TaskAwaiter : public details::TaskAwaiterBase<Type>
    {
      decltype(auto) await_resume() noexcept
      {
        return std::move(this->handle_.promise()).Result();
      }
    };

    return TaskAwaiter { Handle<promise_type>() };
  }

  ///
  /// Returns an awaitable that when awaited, starts the task and waits until it is done.
  ///
  /// The current coroutine will be resumed when the task is done.
  ///
  /// @note The coroutine will be resumed on the thread that executed the task.
  ///
  auto WhenReady() const noexcept
  {
    struct TaskAwaiter : public details::TaskAwaiterBase<Type>
    {
      void await_resume() const noexcept {}
    };

    return TaskAwaiter { Handle<promise_type>() };
  }

  ///
  /// Spins until the task is done.
  ///
  /// Normally used in combination with ejection.
  ///
  /// @note Prefer using proper synchronization for waiting.
  ///
  /// @warning May be unsafe in many cases, use carefully. Also has high CPU usage.
  ///
  /// @warning No memory order guarantees.
  ///
  auto Poll()
  {
    while (!IsReady())
    {
      std::this_thread::yield();
    }

    return Handle<promise_type>().promise().Result();
  }

  ///
  /// Eagerly starts the task (resume) without setting a continuation
  ///
  /// @warning There is no way to reattach a continuation safely, so if the task is executed asynchronously, the only
  /// way to wait for the task to finish is by polling.
  ///
  /// @warning Remember that the coroutine will be destroyed in the destructor. So if we eject the task and the
  /// destructor of the task is called there is undefined behaviour.
  ///
  /// @note This is a dangerous feature and should only be used in extremely rare cases or tests.
  ///
  void Eject()
  {
    Handle<promise_type>().promise().SetContinuation(std::noop_coroutine());
    Handle().resume();
  }

  ///
  /// Returns whether or not the task is done. Tasks that are done will not suspend or block when awaited.
  ///
  /// @return True if task is done, false otherwise.
  ///
  [[nodiscard]] bool IsReady() const noexcept
  {
    return !Handle() || Handle<promise_type>().done();
  }
};

namespace details
{
  // Out of line definitions

  template<typename Type>
  Task<Type> TaskPromise<Type>::get_return_object() noexcept
  {
    return Task<Type> { std::coroutine_handle<TaskPromise<Type>>::from_promise(*this) };
  }

  template<typename Type>
  Task<Type&> TaskPromise<Type&>::get_return_object() noexcept
  {
    return Task<Type&> { std::coroutine_handle<TaskPromise<Type&>>::from_promise(*this) };
  }

  inline Task<void> TaskPromise<void>::get_return_object() noexcept
  {
    return Task<void> { std::coroutine_handle<TaskPromise<void>>::from_promise(*this) };
  }
} // namespace details

template<typename Type>
struct IsTriviallyRelocatable<Task<Type>> : std::true_type
{};
} // namespace plex

#endif
