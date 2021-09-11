#ifndef GENEBITS_ENGINE_THREADING_THREAD_POOL_H
#define GENEBITS_ENGINE_THREADING_THREAD_POOL_H

#include <atomic>
#include <mutex>

#include "genebits/engine/threading/task.h"

namespace genebits::engine
{
class ThreadPool
{
public:
  explicit ThreadPool(const size_t thread_count = std::thread::hardware_concurrency())
    : running_(false), threads_(nullptr), thread_count_(thread_count)
  {
    CreateWorkers();
  }

  ~ThreadPool()
  {
    WaitForTasks();
    DestroyWorkers();
  }

  void Enqueue(Task* task)
  {
    mutex_.lock();

    tasks_.Push(task);

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    // Source: https://en.cppreference.com/w/cpp/thread/condition_variable
    mutex_.unlock();

    // By waking up one thread each time we schedule a task we guarantee we will have the maximum amount of
    // parallelism while not wasting resources if we don't have to (Ex: we don't need 4 threads for 1 task).
    condition_.notify_one();
  }

  [[nodiscard]] constexpr size_t ThreadCount() const noexcept
  {
    return thread_count_;
  }

private:
  void Run()
  {
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

  void DestroyWorkers()
  {
    mutex_.lock();

    running_ = false;

    ASSERT(tasks_.Empty(), "There are still tasks left");

    mutex_.unlock();

    condition_.notify_all();

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i].join();
    }

    delete[] threads_;
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
