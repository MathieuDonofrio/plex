#ifndef PLEX_ASYNC_SHARED_TASK_H
#define PLEX_ASYNC_SHARED_TASK_H

#include <thread>

#include "plex/async/awaitable.h"
#include "plex/debug/assertion.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
template<typename Type = void>
class SharedTask;

namespace details
{
  template<typename Type>
  class SharedTaskPromise;

  ///
  /// Base for the shared task promise.
  ///
  class SharedTaskPromiseBase
  {
  public:
    ///
    /// Shared task node. Contains a continuation and the next node.
    ///
    struct Node
    {
      std::coroutine_handle<> continuation;
      Node* next;
    };

    ///
    /// Final awaiter. Used to resume all continuations.
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
      /// Called after suspension. Resumes all the continuations of the task sequentially.
      ///
      /// @tparam Type Type for task promise.
      ///
      /// @param[in] handle Coroutine of the task.
      ///
      /// @return Continuation handle to resume.
      ///
      template<typename Type>
      void await_suspend(std::coroutine_handle<SharedTaskPromise<Type>> handle) noexcept
      {
        SharedTaskPromise<Type>& promise = handle.promise();

        void* const ready_value = &promise; // The 'this' state

        // Needs release so that writes are visible to waiters.
        // Needs acquire to see prior writes to the waiting coroutines state and contents of the list.
        void* list = promise.state_.exchange(ready_value, std::memory_order_acq_rel);

        if (list != nullptr) ResumeWaiters(static_cast<Node*>(list));
      }

      ///
      /// Does nothing.
      ///
      void await_resume() const noexcept {}

      ///
      /// Resumes all the waiters from the list in sequence.
      ///
      /// @param[in] list List of waiters.
      ///
      static void ResumeWaiters(const Node* list)
      {
        // We are guaranteed to have at least one waiter.

        while (list->next != nullptr)
        {
          // Pointer must be read before resuming because the resumed coroutine may destroy the waiter value.
          Node* next = list->next;
          list->continuation.resume();
          list = next;
        }

        // Last waiter is resumed here to be compiled as a tail call.
        list->continuation.resume();
      }
    };

    ///
    /// Constructor
    ///
    constexpr SharedTaskPromiseBase() noexcept : ref_count_(0), state_(&this->state_) {};

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
    /// Returns final awaiter to resume all continuations.
    ///
    /// @return Final awaiter.
    ///
    FinalAwaiter final_suspend() const noexcept
    {
      return {};
    }

    ///
    /// Tries to add the awaiter to the list of waiters (continuations). Does nothing if the coroutine is already done.
    ///
    /// @param[in] awaiter Node pointer to the awaiter. (Continuation will be override)
    /// @param[in] coroutine Coroutine handle of this promise.
    ///
    /// @return True if the awaiter was added to the list of continuations, false otherwise.
    ///
    bool TryAwait(Node* awaiter, std::coroutine_handle<> coroutine)
    {
      constexpr void* started_no_waiters_value = nullptr;
      const void* const not_started_value = &this->state_;
      const void* const ready_value = this;

      void* old_list = state_.load(std::memory_order_acquire);

      // Resuming after adding the waiter may lead to stack-overflow if the awaiting coroutine has awaited multiple
      // synchronously completing tasks in a row. This can be fixed by starting the coroutine before the first awaiter
      // is added to the list.
      if (old_list == not_started_value
          && state_.compare_exchange_strong(old_list, started_no_waiters_value, std::memory_order_relaxed))
      {
        // Coroutine was not started
        coroutine.resume();
        old_list = state_.load(std::memory_order_acquire);
      }

      // Try to add the waiter to the list.
      do
      {
        if (old_list == ready_value)
        {
          // Coroutine already ready, no need to suspend.
          return false;
        }

        awaiter->next = static_cast<Node*>(old_list);
      }
      while (!state_.compare_exchange_weak(
        old_list, static_cast<void*>(awaiter), std::memory_order_release, std::memory_order_acquire));

      return true;
    }

    ///
    /// Returns whether or not the state is in ready state. This implies that the task is finished and the continuations
    /// are resumed or in the process of being resumed.
    ///
    /// @return True if ready, false otherwise.
    ///
    bool IsReady() const noexcept
    {
      const void* const ready_value = this;
      return state_.load(std::memory_order_acquire) == ready_value;
    }

    ///
    /// Increments the reference count.
    ///
    void AddRef() noexcept
    {
      ref_count_.fetch_add(1, std::memory_order_relaxed);
    }

    ///
    /// Decrements the reference count.
    ///
    /// @return True if there are not more references to the task, false otherwise.
    ///
    bool DropRef() noexcept
    {
      return ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 0;
    }

    COROUTINE_UNHANDLED_EXCEPTION

  private:
    std::atomic_uint_fast32_t ref_count_;

    // State cannot be the first variable with an address because its address would be the same as 'this'. We need a
    // different address to be able to use it as a unique state value.

    ///
    /// States:
    /// - Ready: this
    /// - Not Started: &this->state_
    /// - Started & No waiters: nullptr
    /// - Started & Waiters: Other Pointer (Linked-list of waiters)
    ///
    std::atomic<void*> state_;
  };

  ///
  /// Task promise.
  ///
  /// @tparam Type Result type of the task.
  ///
  template<typename Type>
  class SharedTaskPromise final : public SharedTaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    SharedTaskPromise() noexcept = default;

    ///
    /// Destructor.
    ///
    ~SharedTaskPromise()
    {
      if (IsReady()) reinterpret_cast<Type*>(&storage_)->~Type();
    }

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SharedTask<Type> get_return_object() noexcept;

    ///
    /// Called by co_return to set the return value.
    ///
    /// @param[in] value Value to set.
    ///
    template<typename T>
    requires std::is_convertible_v<T&&, Type>
    void return_value(T&& value) noexcept(std::is_nothrow_constructible_v<T, T&&>)
    {
      new (&storage_) Type(std::forward<T>(value));
    }

    ///
    /// Returns the result that was set by return_value.
    ///
    /// @warning Undefined behaviour if the result is accessed before the value was set.
    ///
    /// @return Result value (lvalue).
    ///
    Type& Result() noexcept
    {
      return *reinterpret_cast<Type*>(&storage_);
    }

  private:
    alignas(Type) char storage_[sizeof(Type)]; // Uninitialized memory
  };

  ///
  /// Task promise specialization for reference types.
  ///
  /// @tparam Type Task result type.
  ///
  template<typename Type>
  class SharedTaskPromise<Type&> final : public SharedTaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    SharedTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SharedTask<Type&> get_return_object() noexcept;

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
  class SharedTaskPromise<void> final : public SharedTaskPromiseBase
  {
  public:
    ///
    /// Constructor.
    ///
    SharedTaskPromise() noexcept = default;

    ///
    /// Obtains the task from the promise.
    ///
    /// @return Task for the promise.
    ///
    SharedTask<void> get_return_object() noexcept;

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
  /// Shared task awaiter base.
  ///
  /// @tparam Type Task result type.
  ///
  template<typename Type>
  class SharedTaskAwaiterBase
  {
  public:
    ///
    /// Constructor.
    ///
    /// @param handle Task coroutine handle.
    ///
    SharedTaskAwaiterBase(std::coroutine_handle<SharedTaskPromise<Type>> handle) noexcept : handle_(handle) {}

    ///
    /// Called before suspending to check if we should avoid suspending. If the coroutine is already done, then we dont
    /// need to suspend.
    ///
    /// @return True if the coroutine is done, false otherwise.
    ///
    bool await_ready() const noexcept
    {
      return !handle_ || handle_.promise().IsReady();
    }

    ///
    /// Called after suspension. Attempts to add the awaiter to the list of waiters, if successful will return control
    /// to the caller, otherwise, the task is done and the current coroutine is resumed synchronously.
    ///
    /// @param[in] awaiting The awaiting coroutine.
    ///
    /// @return Continuation handle to resume.
    ///
    bool await_suspend(std::coroutine_handle<> awaiting) noexcept
    {
      awaiter_.continuation = awaiting;
      return handle_.promise().TryAwait(&awaiter_, handle_);
    }

  protected:
    std::coroutine_handle<SharedTaskPromise<Type>> handle_;
    SharedTaskPromiseBase::Node awaiter_;
  };
} // namespace details

///
/// A shared task represents a shared asynchronous computation. It is one of the basic building block of the async
/// coroutine workflow.
///
/// Shared tasks are lazily executed. The execution of the coroutine does not start until the task is awaited.
///
/// Shared tasks can be copied and allow multiple coroutines to await it. Waiters will be added to a queue to be resumed
/// once the task is complete. If the task is already complete the awaiter will continue synchronously.
///
/// @note Inspired by cppcoro implementation: https://github.com/lewissbaker/cppcoro
///
/// @tparam Type The result/return type of the task.
///
template<typename Type>
class SharedTask
{
public:
  using promise_type = details::SharedTaskPromise<Type>;
  using handle_type = std::coroutine_handle<promise_type>;
  using value_type = Type;

  ///
  /// Default constructor.
  ///
  SharedTask() noexcept : handle_(nullptr) {}

  ///
  /// Constructor.
  ///
  /// @param[in] handle Coroutine handle managed by the task.
  ///
  explicit SharedTask(handle_type handle) noexcept : handle_(handle) {}

  ///
  /// Destructor.
  ///
  ~SharedTask()
  {
    if (handle_ && handle_.promise().DropRef()) handle_.destroy();
  }

  ///
  /// Move constructor.
  ///
  /// @param[in] other Other shared task to move.
  ///
  SharedTask(SharedTask&& other) noexcept : handle_(other.handle_)
  {
    other.handle_ = nullptr;
  }

  ///
  /// Copy constructor.
  ///
  /// @param[in] other Other shared task to copy.
  ///
  SharedTask(const SharedTask& other) noexcept : handle_(other.handle_)
  {
    if (handle_) handle_.promise().AddRef();
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Other shared task to move.
  ///
  /// @return Reference of this.
  ///
  SharedTask& operator=(SharedTask&& other) noexcept
  {
    SharedTask(std::move(other)).Swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Other shared task to move.
  ///
  /// @return Reference of this.
  ///
  SharedTask& operator=(const SharedTask& other) noexcept
  {
    SharedTask(other).Swap(*this);
    return *this;
  }

  ///
  /// Starts the task if it was not started and adds the coroutine to the list of waiters if the task is not done.
  ///
  /// The current coroutine will be resumed when the task is done or if it was already done, in that case, the caller is
  /// resumed synchronously.
  ///
  /// @note The coroutine will be resumed on the thread that executed the task.
  ///
  auto operator co_await() const noexcept
  {
    struct SharedTaskAwaiter final : public details::SharedTaskAwaiterBase<Type>
    {
      decltype(auto) await_resume() noexcept
      {
        return this->handle_.promise().Result();
      }
    };

    return SharedTaskAwaiter { handle_ };
  }

  ///
  /// Returns an awaitable that when awaited, starts the task if it was not started and adds the coroutine to the list
  /// of waiters if the task is not done.
  ///
  /// The current coroutine will be resumed when the task is done or if it was
  /// already done, in that case, the caller is resumed synchronously.
  ///
  /// If the task has a non-null result, the result will be ignored.
  ///
  /// @note The coroutine will be resumed on the thread that executed the task.
  ///
  auto WhenReady() const noexcept
  {
    struct SharedTaskAwaiter : public details::SharedTaskAwaiterBase<Type>
    {
      void await_resume() const noexcept {}
    };

    return SharedTaskAwaiter { handle_ };
  }

  ///
  /// Swaps contents with another shared task.
  ///
  /// @param[in] other Other shared task.
  ///
  void Swap(SharedTask<Type>& other)
  {
    std::swap(handle_, other.handle_);
  }

  ///
  /// Returns whether or not the task is done. Tasks that are done will not suspend or block when awaited.
  ///
  /// @return True if task is done, false otherwise.
  ///
  [[nodiscard]] bool IsReady() const noexcept
  {
    return !handle_ || handle_.done();
  }

private:
  handle_type handle_;
};

namespace details
{
  // Out of line definitions

  template<typename Type>
  SharedTask<Type> SharedTaskPromise<Type>::get_return_object() noexcept
  {
    return SharedTask<Type> { std::coroutine_handle<SharedTaskPromise<Type>>::from_promise(*this) };
  }

  template<typename Type>
  SharedTask<Type&> SharedTaskPromise<Type&>::get_return_object() noexcept
  {
    return SharedTask<Type&> { std::coroutine_handle<SharedTaskPromise<Type&>>::from_promise(*this) };
  }

  inline SharedTask<void> SharedTaskPromise<void>::get_return_object() noexcept
  {
    return SharedTask<void> { std::coroutine_handle<SharedTaskPromise<void>>::from_promise(*this) };
  }
} // namespace details

///
/// Creates a shared task from an awaitable by co_awaiting it.
///
/// @note If the result type is void, will use the WhenReady awaitable if the awaitable supports it.
///
/// @tparam Awaitable Awaitable type to create task from.
/// @tparam Result The result type, defaults to awaitable result type.
///
/// @param[in] awaitable Awaitable to used to create the trigger task.
///
/// @return Created shared task (initially suspended).
///
template<Awaitable Awaitable, typename Result = typename AwaitableTraits<Awaitable>::AwaitResultType>
auto MakeSharedTask(Awaitable&& awaitable) -> SharedTask<Result>
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
    co_return co_await std::forward<Awaitable>(awaitable);
  }
}

template<typename Type>
struct IsTriviallyRelocatable<SharedTask<Type>> : std::true_type
{};
} // namespace plex

#endif
