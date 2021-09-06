#ifndef GENEBITS_ENGINE_JOBS_THREAD_POOL_H
#define GENEBITS_ENGINE_JOBS_THREAD_POOL_H

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>

#include "genebits/engine/util/concurrency.h"
#include "genebits/engine/util/delegate.h"

namespace genebits::engine
{
class Task : public Delegate<>
{
public:
  Task() noexcept : Delegate<>(), value_(new std::atomic_bool(false)) {}

  void Wait() const
  {
    while (!GetValue())
    {
      // TODO Smarter sleeping

      // Probably use an exponential backoff with a cap here

      std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
  }

  void SetValue(bool value)
  {
    value_->store(value, std::memory_order_relaxed);
  }

  bool GetValue() const
  {
    return value_->load(std::memory_order_relaxed);
  }

private:
  std::shared_ptr<std::atomic_bool> value_;
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

  void Execute(Task task)
  {
    // Maybe use exchange here and notify workers

    task_count_.fetch_add(1, std::memory_order_relaxed);

    mutex_.lock();

    tasks_.push(std::move(task));

    mutex_.unlock();
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

    for (size_t i = 0; i < thread_count_; i++)
    {
      threads_[i].join();
    }

    delete[] threads_;
  }

  void Worker()
  {
    while (running_)
    {
      if (task_count_.load(std::memory_order_relaxed))
      {
        // TODO better queue

        // Maybe implement lock-free queue

        mutex_.lock();

        Task task = std::move(tasks_.front());

        tasks_.pop();

        mutex_.unlock();

        task_count_.fetch_sub(1, std::memory_order_relaxed);

        task.Invoke();

        task.SetValue(true);
      }
      else
      {
        // TODO smarter sleeping

        // Maybe use a notification based wake up
        // conditional_variable

        std::this_thread::sleep_for(std::chrono::microseconds(1000));
      }
    }
  }

  void WaitForTasks()
  {
    while (task_count_.load(std::memory_order_relaxed))
    {
      std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
  }

private:
  mutable std::mutex mutex_;

  std::atomic_bool running_;

  std::queue<Task> tasks_;

  std::atomic_uint_fast32_t task_count_;

  std::thread* threads_;

  size_t thread_count_;
};
} // namespace genebits::engine

#endif
