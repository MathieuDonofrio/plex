#ifndef GENEBITS_ENGINE_JOBS_THREAD_POOL_H
#define GENEBITS_ENGINE_JOBS_THREAD_POOL_H

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>

#include "genebits/engine/jobs/concurrent_ring_buffer.h"
#include "genebits/engine/util/concurrency.h"
#include "genebits/engine/util/delegate.h"

namespace genebits::engine
{
class ThreadPool;

class Task : public Delegate<>
{
public:
  Task() noexcept : Delegate<>(), state_(false) {}

  void Wait() const noexcept
  {
    if (!Finished()) state_.wait(false, std::memory_order_relaxed);
  }

  bool Finished() const noexcept
  {
    return state_.load(std::memory_order_relaxed);
  }

private:
  friend class ThreadPool;

  void Notify() noexcept
  {
    state_.store(true, std::memory_order_relaxed);
    state_.notify_all();
  }

private:
  std::atomic_bool state_;
};

class ThreadPool
{
public:
  explicit ThreadPool(const size_t thread_count = std::thread::hardware_concurrency()) : thread_count_(thread_count)
  {
    CreateWorkers();
  }

  ~ThreadPool()
  {
    WaitForTasks();
    DestroyWorkers();
  }

  void Execute(Task* task)
  {
    // Maybe use exchange here and notify workers

    mutex_.lock();

    tasks_.push(task);

    task_count_.fetch_add(1, std::memory_order_relaxed);

    mutex_.unlock();

    flag_.store(true, std::memory_order_relaxed);
    flag_.notify_one();
  }

  [[nodiscard]] constexpr size_t ThreadCount() const noexcept
  {
    return thread_count_;
  }

  [[nodiscard]] size_t TasksCount() const noexcept
  {
    return task_count_.load(std::memory_order_relaxed);
  }

private:
  void CreateWorkers()
  {
    threads_ = new std::thread[thread_count_];

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i] = std::thread(&ThreadPool::Worker, this);
    }

    running_.store(true, std::memory_order_relaxed);
  }

  void DestroyWorkers()
  {
    running_.store(false, std::memory_order_relaxed);

    flag_.store(true, std::memory_order_relaxed);
    flag_.notify_all();

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i].join();
    }

    delete[] threads_;
  }

  void Worker()
  {
    while (running_.load(std::memory_order_relaxed))
    {
      if (task_count_.load(std::memory_order_relaxed))
      {
        if (mutex_.try_lock())
        {
          if (task_count_.load(std::memory_order_relaxed))
          {
            Task* task = tasks_.front();

            tasks_.pop();

            task_count_.fetch_sub(1, std::memory_order_relaxed);

            mutex_.unlock();

            task->Invoke();

            task->Notify();
          }
          else
          {
            mutex_.unlock();
          }
        }
        else
        {
          this_thread::Pause();
        }
      }
      else
      {
        // There is no work left for any worker. Make this worker wait until a new task is executed.

        flag_.store(false, std::memory_order_relaxed);
        flag_.wait(false, std::memory_order_relaxed);
      }
    }
  }

  void WaitForTasks()
  {
    uint_fast32_t task_count;

    while ((task_count = task_count_.load(std::memory_order_relaxed)) > thread_count_)
    {
      // If there are a lot of tasks, sleep.

      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    if (task_count)
    {
      // If there are just a few tasks, spin with exponential backoff.
      // This can consume a good amount of CPU, so we don't want to spin all the time.

      ExponentialBackoff backoff;

      while (task_count_.load(std::memory_order_relaxed))
      {
        backoff.Wait();
      }
    }
  }

private:
  std::mutex mutex_;

  std::atomic_bool running_;

  std::atomic_bool flag_;

  std::queue<Task*> tasks_;

  std::atomic_uint_fast32_t task_count_;

  std::thread* threads_;

  size_t thread_count_;
};
} // namespace genebits::engine

#endif
