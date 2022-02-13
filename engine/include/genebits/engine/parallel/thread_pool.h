#ifndef GENEBITS_ENGINE_PARALLEL_THREAD_POOL_H
#define GENEBITS_ENGINE_PARALLEL_THREAD_POOL_H

#include <atomic>
#include <deque>
#include <mutex>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/threading.h"

namespace genebits::engine
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
  ThreadPool(const size_t thread_count, bool lock_threads)
    : running_(false), threads_(nullptr), thread_count_(thread_count)
  {
    ASSERT(thread_count > 0, "Thread pool cannot have 0 threads");

    CreateWorkers();

    if (lock_threads) SetWorkerThreadAffinity();
  }

  ///
  /// Default constructor.
  ///
  /// Tries to create threads equal to amount of physical processors, sometimes this is not
  /// accurate.
  ///
  /// Every thread is locked to a physical processor.
  ///
  ThreadPool() : ThreadPool(GetAmountPhysicalProcessors(), true) {}

  ///
  /// Destructor.
  ///
  ~ThreadPool()
  {
    DestroyWorkers();
  }

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
  ///
  /// Runnable function executed by every worker thread.
  ///
  /// Loops and waits for tasks to execute until flagged to finish.
  ///
  void Run()
  {
    this_thread::SetName("Worker");

    std::unique_lock lock(mutex_);

    Operation* op;

    while (running_ || !queue_.Empty()) // Always locked when doing an iteration
    {
      op = queue_.Front();

      if (op != nullptr)
      {
        queue_.Dequeue();

        // Make sure we are unlocked for task execution
        lock.unlock();

        op->Execute();

        // Lock before running another iteration
        lock.lock();
      }
      else
      {
        // Unlocks, sleeps then locks again when woke.

        // It is possible that we are sleeping and there are tasks in the queue. This is because
        // the scheduler is not required to wake up more threads than it needs to execute the tasks.
        // See the schedule method.
        condition_.wait(lock);
      }
    }
  }

  ///
  /// Creates and initializes all the worker threads.
  ///
  /// @warning Undefined behaviour if the thread pool already created workers.
  ///
  void CreateWorkers()
  {
    ASSERT(!running_, "Thread pool already running");

    running_ = true;

    threads_ = new std::thread[thread_count_];

    for (size_t i = 0; i < thread_count_; i++)
    {
      new (threads_ + i) std::thread(&ThreadPool::Run, this);
    }
  }

  ///
  /// Tries to set the worker thread affinity so that every worker can only run on
  /// one physical processor.
  ///
  /// If the amount of worker threads is greater than the amount of physical processors,
  /// multiple worker threads can be assigned per physical processor.
  ///
  /// @note This does nothing if cpu info is not supported.
  ///
  void SetWorkerThreadAffinity()
  {
    const CPUInfo info = GetCPUInfo();

    const auto core_count = info.processors.size();

    for (size_t i = 0; i < thread_count_; i++)
    {
      SetThreadAffinity(threads_[i].native_handle(), info.processors[i % core_count].mask);
    }
  }

  ///
  /// Destroys all the worker threads.
  ///
  /// @warning All tasks must be finished before and while destroying workers.
  ///
  void DestroyWorkers()
  {
    mutex_.lock();

    running_ = false;

    mutex_.unlock();

    condition_.notify_all();

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i].join();
    }

#ifndef NDEBUG
    mutex_.lock();

    ASSERT(queue_.Empty(), "There are still tasks");

    mutex_.unlock();
#endif

    delete[] threads_;
  }

private:
  class WorkQueue;

  class Operation
  {
  public:
    constexpr Operation(ThreadPool* pool) noexcept : pool_(pool), next_(nullptr) {}

    bool await_ready() const noexcept
    {
      return false;
    }

    void await_resume() const noexcept {}

    void await_suspend(std::coroutine_handle<> awaiting) noexcept
    {
      handle_ = awaiting;

      pool_->Enqueue(this);
    }

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
    constexpr void Enqueue(Operation* task) noexcept
    {
      ASSERT(!task->next_, "Task next must be nullptr");

      tail_->next_ = task;
      tail_ = task;
    }

    ///
    /// Removes the operation from the front of the queue.
    ///
    constexpr void Dequeue() noexcept
    {
      ASSERT(!Empty(), "Queue cannot be empty");

      head_.next_ = head_.next_->next_;

      if (head_.next_ == nullptr) tail_ = &head_;
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

  private:
    Operation head_; // Sentinel Node
    Operation* tail_;
  };

  ///
  /// Enqueues the task to be executed on a worker thread from this pool.
  ///
  /// @param[in] task Task to enqueue.
  ///
  void Enqueue(Operation* operation)
  {
    mutex_.lock();

    ASSERT(running_, "Cannot enqueue task when thread pool not running");

    queue_.Enqueue(operation);

    mutex_.unlock();

    // By waking up one thread each time we schedule a task we guarantee we will have the maximum amount of
    // parallelism while not wasting resources if we don't have to (Ex: we don't need 4 threads for 1 task).
    condition_.notify_one();
  }

private:
  std::mutex mutex_;

  bool running_;
  std::condition_variable condition_;

  WorkQueue queue_;

  std::thread* threads_;
  size_t thread_count_;
};
} // namespace genebits::engine

#endif
