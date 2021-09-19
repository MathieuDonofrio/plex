#ifndef GENEBITS_ENGINE_PARALLEL_THREAD_POOL_H
#define GENEBITS_ENGINE_PARALLEL_THREAD_POOL_H

#include <atomic>
#include <mutex>

#include "genebits/engine/parallel/task.h"

namespace genebits::engine
{
///
/// Pool of threads to execute tasks on.
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
  ///
  explicit ThreadPool(const size_t thread_count) : running_(false), threads_(nullptr), thread_count_(thread_count)
  {
    ASSERT(thread_count > 0, "Thread pool cannot have 0 threads");

    CreateWorkers();
  }

  ///
  /// Default constructor.
  ///
  /// Tries to create threads equal to amount of physical processors, sometimes this is not
  /// accurate.
  ///
  /// @note Will also attempt to set the threads affinity so that every thread only runs on one
  /// physical core.
  ///
  ThreadPool() : ThreadPool(GetAmountPhysicalProcessors())
  {
    SetWorkerThreadAffinity();
  }

  ///
  /// Destructor.
  ///
  ~ThreadPool()
  {
    WaitForTasks();
    DestroyWorkers();
  }

  ///
  /// Enqueues a range of tasks to be executed on worker threads of this pool.
  ///
  /// @tparam Iterator Type of iterator.
  ///
  /// @param[in] first Iterator start.
  /// @param[in] last Iterator end.
  ///
  template<typename Iterator>
  void EnqueueAll(Iterator first, Iterator last)
  {
    mutex_.lock();

    while (first != last)
    {
      tasks_.Push(first++);
    }

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    // Source: https://en.cppreference.com/w/cpp/thread/condition_variable
    mutex_.unlock();

    condition_.notify_all();
  }

  ///
  /// Enqueues the task to be executed on a worker thread from this pool.
  ///
  /// @param[in] task Task to enqueue.
  ///
  void Enqueue(Task* task)
  {
    mutex_.lock();

    tasks_.Push(task);

    mutex_.unlock();

    // By waking up one thread each time we schedule a task we guarantee we will have the maximum amount of
    // parallelism while not wasting resources if we don't have to (Ex: we don't need 4 threads for 1 task).
    condition_.notify_one();
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

    // RAII lock on construction & unlock on destruction
    std::unique_lock<std::mutex> lock(mutex_);

    while (running_) // Always locked when doing an iteration
    {
      if (Task* task = tasks_.Front()) // Not empty
      {
        tasks_.Pop();

        // Make sure we are unlocked for task execution
        lock.unlock();

        task->Executor().Invoke();
        task->Finish();

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
  /// Non-intrusively waits until all tasks are finished.
  ///
  /// Spins and sleeps in between iterations.
  ///
  /// @warning Not efficient, should only be used for destruction of the pool.
  ///
  void WaitForTasks()
  {
    while (true)
    {
      mutex_.lock();

      const bool empty = tasks_.Empty();

      mutex_.unlock();

      if (!empty)
      {
        // Sleep a little in between spins to free the CPU
        std::this_thread::sleep_for(std::chrono::microseconds(100));
      }
      else
        return;
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
      threads_[i] = std::thread(&ThreadPool::Run, this);
    }
  }

  ///
  /// Tries to set the worker thread affinity so that every worker can only run on
  /// one physical processor.
  ///
  /// If the amount of worker threads is greater than the amount of physical processors
  /// excess worker threads will we free to run on any processor.
  ///
  /// @warning This does nothing if cpu info is not supported.
  ///
  void SetWorkerThreadAffinity()
  {
    CPUInfo info = GetCPUInfo();

    if (info.supported)
    {
      for (size_t i = 0; i < thread_count_ && i < info.processors.size(); i++)
      {
        SetThreadAffinity(threads_[i].native_handle(), info.processors[i].mask);
      }
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

    ASSERT(tasks_.Empty(), "Tasks left when destroying workers");

    mutex_.unlock();

    condition_.notify_all();

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i].join();
    }

    mutex_.lock();

    ASSERT(tasks_.Empty(), "Tasks where added during destruction of workers");

    delete[] threads_;

    mutex_.unlock();
  }

private:
  std::mutex mutex_;

  bool running_;
  std::condition_variable condition_;

  TaskQueue tasks_;

  std::thread* threads_;
  size_t thread_count_;
};
} // namespace genebits::engine

#endif
