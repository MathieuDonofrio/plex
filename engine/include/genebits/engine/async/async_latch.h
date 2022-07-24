#ifndef PLEX_ASYNC_ASYNC_LATCH_H
#define PLEX_ASYNC_ASYNC_LATCH_H

#include <atomic>

#include "genebits/engine/async/awaitable.h"
#include "genebits/engine/debug/assertion.h"

namespace plex
{
///
/// Synchronization primitive that allows one or more coroutines to wait until a count down is reached.
///
/// The latch is initialized with a given count. Awaiting the latch will suspend the awaiting coroutine and later resume
/// it when the count reaches zero. The count of the latch is decremented by invoking the CountDown() method.
///
/// A latch can be used to wait until a given amount of coroutines are complete.
///
/// @note Latches are a one-shot, once the latch was release it cannot be reused.
///
class AsyncLatch
{
public:
  ///
  /// Constructor.
  ///
  /// @param[in] amount The number of times CountDown() must be invoked before release.
  ///
  AsyncLatch(std::size_t amount) noexcept
    : counter_(static_cast<uint_fast32_t>(amount)), state_(amount ? nullptr : static_cast<void*>(this))
  {}

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  ~AsyncLatch() noexcept
  {
    ASSERT(state_.load(std::memory_order_relaxed) == nullptr || state_.load(std::memory_order_relaxed) == this,
      "The latch was not released and there are waiters");
  }
#endif

  ///
  /// Decrements the internal counter of the latch and releases all waiters if the counter reaches 0.
  ///
  /// Latches are one time use. Waiters cannot be resumed more than once. However, you can call CountDown() more times
  /// than necessary.
  ///
  void CountDown() noexcept
  {
    if (counter_.fetch_sub(1, std::memory_order_acq_rel) == 1)
    {
      auto* const old_state = Release();

      ResumeWaiters(static_cast<Operation*>(old_state));
    }
  }

  ///
  /// Whether or not the latch has been released.
  ///
  /// Co_awaiting a latch that is already released never results in a suspension.
  ///
  /// @return True if the latch is released, false otherwise.
  ///
  bool IsReleased() const noexcept
  {
    return state_.load(std::memory_order_acquire) == static_cast<const void*>(this);
  }

  ///
  /// Adds the coroutine to the list of waiters and awaits until the latch is released.
  ///
  /// @return Latch awaitable.
  ///
  auto operator co_await() const noexcept
  {
    return Operation { *this };
  }

private:
  ///
  /// Represents a latch operation. Serves as both node in a list and the awaitable of the await operation.
  ///
  class Operation
  {
  public:
    ///
    /// Constructor.
    ///
    /// @param[in] latch Latch for this operation.
    ///
    explicit Operation(const AsyncLatch& latch) : latch_(latch) {}

    ///
    /// Called before suspending to check if we should avoid suspending.
    ///
    /// @return True if the latch is already released.
    ///
    bool await_ready() const noexcept
    {
      return latch_.IsReleased();
    }

    ///
    /// Called after suspension. Attempts to push the awaiter to the front of the list contained as the latch state.
    /// Will resume the current coroutine if the latch was released during the push, otherwise, will return control to
    /// the caller.
    ///
    /// @param[in] awaiter Awaiting coroutine.
    ///
    /// @return False if the latch was released, otherwise true.
    ///
    bool await_suspend(std::coroutine_handle<> awaiter) noexcept
    {
      awaiter_ = awaiter;

      const void* const set_value = static_cast<const void*>(&latch_);

      void* old_state = latch_.state_.load(std::memory_order_acquire);

      // Try to add waiter to the list.
      do
      {
        if (old_state == set_value)
        {
          // State was now set, no need to suspend.
          return false;
        }

        next_ = static_cast<Operation*>(old_state);
      }
      while (!latch_.state_.compare_exchange_weak(
        old_state, static_cast<void*>(this), std::memory_order_release, std::memory_order_acquire));

      return true;
    }

    ///
    /// Does nothing.
    ///
    void await_resume() const noexcept {}

  private:
    friend class AsyncLatch;

    const AsyncLatch& latch_;

    Operation* next_;
    std::coroutine_handle<> awaiter_;
  };

  ///
  /// Releases the latch.
  ///
  /// Sets the state to 'this' and returns the old state. If the old state is equal to 'this' then the latch was already
  /// released.
  ///
  /// @return Old state.
  ///
  void* Release()
  {
    void* const new_state = this;

    // Needs release so that writes are visible to waiters.
    // Needs acquire to see prior writes to the waiting coroutines state and contents of the list.
    void* old_state = state_.exchange(new_state, std::memory_order_acq_rel);

    return old_state;
  }

  ///
  /// Resumes all the waiters from an operation list in sequence.
  ///
  /// @param[in] list List of operations (waiters).
  ///
  static void ResumeWaiters(const Operation* list) noexcept
  {
    while (list != nullptr)
    {
      auto* next = list->next_;
      list->awaiter_.resume();
      list = next;
    }
  }

private:
  std::atomic_uint_fast32_t counter_;

  ///
  /// States:
  /// - Released: this
  /// - Not Released: nullptr
  /// - Not Released & Waiters: Other Pointer (Linked-list of waiters)
  ///
  mutable std::atomic<void*> state_;
};
} // namespace plex

#endif
