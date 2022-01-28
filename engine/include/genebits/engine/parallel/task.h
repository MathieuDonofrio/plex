#ifndef GENEBITS_ENGINE_PARALLEL_TASK_H
#define GENEBITS_ENGINE_PARALLEL_TASK_H

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/parallel/awaitable.h"

namespace genebits::engine
{
#ifndef NDEBUG
///
/// Used to define the default unhandled_exception for coroutine promises.
///
#define COROUTINE_UNHANDLED_EXCEPTION                 \
  void unhandled_exception() noexcept                 \
  {                                                   \
    ERROR("Unhandled exception thrown in coroutine"); \
  }
#else
#define COROUTINE_UNHANDLED_EXCEPTION
#endif

template<typename Type>
class Task;

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
  explicit TaskBase(std::coroutine_handle<> handle) : handle_(handle) {}

  ///
  /// Destructor.
  ///
  ~TaskBase()
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

protected:
  ///
  /// Swaps handles with another task base.
  ///
  /// @param[in] task Other task base.
  ///
  void Swap(TaskBase& task)
  {
    std::swap(handle_, task.handle_);
  }

  ///
  /// Returns the void coroutine handle.
  ///
  /// @return Void coroutine handle.
  ///
  std::coroutine_handle<> Handle() const
  {
    return handle_;
  }

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
  std::coroutine_handle<PromiseType> Handle() const
  {
    return std::coroutine_handle<PromiseType>::from_address(handle_.address());
  }

private:
  std::coroutine_handle<> handle_;
};

namespace details
{
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
      /// @tparam Promise Must be TaskPromise.
      ///
      /// @param[in] handle
      ///
      /// @return Continuation handle to resume.
      ///
      template<typename Promise>
      std::coroutine_handle<> await_suspend(std::coroutine_handle<Promise> handle) noexcept
      {
        return handle.promise().continuation_;
      }

      ///
      /// Does nothing.
      ///
      void await_resume() noexcept {}
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
    template<std::convertible_to<Type> T>
    void return_value(T&& value) noexcept(std::is_nothrow_constructible_v<T, T&&>)
    {
      new (std::addressof(value_)) T(std::forward<T>(value));
    }

    ///
    /// Returns the result that was set by return_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value (lvalue).
    ///
    Type& result() &
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
    Type&& result() &&
    {
      return std::move(value_);
    }

  private:
    Type value_; // Uninitialized memory
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
    Type& result() noexcept
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
    /// Called by co_return. Does nothing.
    ///
    void return_void() noexcept {}

    ///
    /// Does nothing.
    ///
    void result() noexcept {}
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
    TaskAwaiterBase(std::coroutine_handle<TaskPromise<Type>> handle) : handle_(handle) {}

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
template<typename Type = void>
class Task : public TaskBase
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
  explicit Task(handle_type handle) : TaskBase(handle) {}

  ///
  /// Starts the task and awaits it. The current coroutine will be resumed when the task is done.
  ///
  /// @warning The coroutine will be resumed on the thread that executed the task.
  ///
  auto operator co_await() const& noexcept
  {
    struct TaskAwaiter final : public details::TaskAwaiterBase<Type>
    {
      decltype(auto) await_resume() noexcept
      {
        return this->handle_.promise().result();
      }
    };

    return TaskAwaiter { Handle<promise_type>() };
  }

  ///
  /// Starts the task and awaits it. The current coroutine will be resumed when the task is done.
  ///
  /// @warning The coroutine will be resumed on the thread that executed the task.
  ///
  auto operator co_await() const&& noexcept
  {
    struct TaskAwaiter : public details::TaskAwaiterBase<Type>
    {
      decltype(auto) await_resume() noexcept
      {
        return std::move(this->handle_.promise()).result();
      }
    };

    return TaskAwaiter { Handle<promise_type>() };
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

} // namespace genebits::engine

#endif
