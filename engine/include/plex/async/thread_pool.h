#ifndef PLEX_ASYNC_THREAD_POOL_H
#define PLEX_ASYNC_THREAD_POOL_H

#include <atomic>
#include <mutex>

#include "plex/async/task.h"

namespace plex
{
///
/// Pool of threads to execute tasks on.
///
/// This kind of thread pool may not be ideal for io tasks. It is designed to work well for compute parallelization.
///
/// @note Idle threads contained by a thread pool do no use up CPU.
///
class ThreadPool
{
public:
  ///
  /// Parametric constructor.
  ///
  /// @param[in] thread_count Amount of worker threads to create pool with.
  /// @param[in] lock_threads Locks every thread to a physical processor.
  ///
  ThreadPool(const size_t thread_count, bool lock_threads);

  ///
  /// Default constructor.
  ///
  /// Tries to create threads equal to amount of physical processors, sometimes this is not
  /// accurate.
  ///
  /// Every thread is locked to a physical processor.
  ///
  ThreadPool();

  ///
  /// Destructor.
  ///
  ~ThreadPool();

  ///
  /// Returns an awaiter that will schedule the awaiting coroutine to be later resumed by the thread pool.
  ///
  /// @return Thread pool awaiter.
  ///
  auto Schedule()
  {
    return Operation { this };
  }

  ///
  /// Returns amount of worker threads contained by this thread pool.
  ///
  /// @return Amount of worker threads in the pool.
  ///
  [[nodiscard]] constexpr size_t ThreadCount() const noexcept
  {
    return thread_count_;
  }

private:
  class WorkQueue;

  ///
  /// Represents a queued operation for the thread pool. Contains the handle to the coroutine.
  ///
  class Operation
  {
  public:
    ///
    /// Constructor.
    ///
    /// @param[in] pool
    ///
    constexpr Operation(ThreadPool* pool) noexcept : pool_(pool), next_(nullptr) {}

    bool await_ready() const noexcept
    {
      return false;
    }

    ///
    /// Does nothing.
    ///
    void await_resume() const noexcept {}

    ///
    /// Called after suspension. Enqueues the
    ///
    /// @tparam Type Type for task promise.
    ///
    /// @param[in] handle Coroutine of the task.
    ///
    /// @return Continuation handle to resume.
    ///
    void await_suspend(std::coroutine_handle<> awaiting) noexcept
    {
      handle_ = awaiting;

      pool_->Enqueue(this);
    }

    ///
    /// Resumes the operation coroutine handle.
    ///
    void Execute() const
    {
      handle_.resume();
    }

  private:
    friend class WorkQueue;

    ThreadPool* pool_;

    std::coroutine_handle<> handle_;
    Operation* next_;
  };

  class WorkQueue
  {
  public:
    ///
    /// Default constructor.
    ///
    constexpr WorkQueue() noexcept : head_(nullptr), tail_(&head_) {}

    WorkQueue(WorkQueue& other) = delete;
    WorkQueue& operator=(WorkQueue& other) = delete;

    ///
    /// Adds an operation to the back of the queue.
    ///
    /// @param[in] task Task to add.
    ///
    void Enqueue(Operation* task) noexcept
    {
      ASSERT(!task->next_, "Task next must be nullptr");

      tail_.load(std::memory_order_acquire)->next_ = task;
      tail_ = task;
    }

    ///
    /// Removes the operation from the front of the queue.
    ///
    void Dequeue() noexcept
    {
      ASSERT(!Empty(), "Queue cannot be empty");

      head_.next_ = head_.next_->next_;

      if (head_.next_ == nullptr) tail_.store(&head_, std::memory_order_release);
    }

    ///
    /// Returns the operation at the front of the queue.
    ///
    /// @warning Will return the sentinel node if list is empty.
    ///
    /// @return Operation at the front of the queue, nullptr if the queue is empty.
    ///
    [[nodiscard]] constexpr Operation* Front() const noexcept
    {
      return head_.next_;
    }

    ///
    /// Returns whether or not the queue is empty.
    ///
    /// @return True if the queue is empty, false otherwise.
    ///
    [[nodiscard]] constexpr bool Empty() const noexcept
    {
      return !head_.next_;
    }

    ///
    /// Returns approximately whether or not the queue has work.
    ///
    /// @return True if the queue might have work, false otherwise.
    ///
    [[nodiscard]] bool HasWorkApprox() const noexcept
    {
      return tail_.load(std::memory_order_acquire) != &head_;
    }

  private:
    Operation head_; // Sentinel Node
    std::atomic<Operation*> tail_;
  };

private:
  ///
  /// Executed by every worker thread to enter work loop.
  ///
  /// Loops and waits for work to be processed until flagged to finish.
  ///
  void RunWorker();

  ///
  /// Enqueues the operation to be executed on a worker thread.
  ///
  /// @param[in] operation Operation to enqueue.
  ///
  void Enqueue(Operation* operation);

  ///
  /// Creates and initializes all the worker threads.
  ///
  void CreateWorkers();

  ///
  /// Tries to set the worker thread affinity so that every worker can only run on
  /// one physical processor.
  ///
  /// If the amount of worker threads is greater than the amount of physical processors,
  /// multiple worker threads can be assigned per physical processor.
  ///
  void SetWorkerThreadAffinity();

  ///
  /// Destroys all the worker threads.
  ///
  /// @warning There should not be any work being scheduled while destroying workers.
  ///
  void DestroyWorkers();

private:
  std::mutex mutex_;

  bool running_;
  std::condition_variable condition_;

  WorkQueue queue_;

  std::thread* threads_;
  size_t thread_count_;
};
} // namespace plex

#endif
