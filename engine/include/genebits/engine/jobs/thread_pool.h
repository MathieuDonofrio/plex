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
  Task() noexcept : Delegate<>(), state_(std::make_shared<TaskState>()) {}

  void Wait() const
  {
    ExponentialBackoff backoff;

    while (!Finished())
    {
      backoff.Wait();
    }
  }

  void Notify()
  {
    state_->value.store(true, std::memory_order_relaxed);
  }

  bool Finished() const
  {
    return state_->value.load(std::memory_order_relaxed);
  }

private:
  struct TaskState
  {
    std::atomic_bool value;
  };

  std::shared_ptr<TaskState> state_;
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

    tasks_mutex_.lock();

    tasks_.push(std::move(task));

    task_count_.fetch_add(1, std::memory_order_relaxed);

    tasks_mutex_.unlock();

    condition_variable_.notify_one();
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

    condition_variable_.notify_all();

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

        tasks_mutex_.lock();

        if (task_count_.load(std::memory_order_relaxed))
        {
          Task task = std::move(tasks_.front());

          tasks_.pop();

          task_count_.fetch_sub(1, std::memory_order_relaxed);

          tasks_mutex_.unlock();

          task.Invoke();

          task.Notify();
        }
        else
        {
          tasks_mutex_.unlock();
        }
      }
      else
      {
        std::unique_lock<std::mutex> lock(notifier_mutex_);

        condition_variable_.wait(lock);
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
  std::mutex tasks_mutex_;

  std::mutex notifier_mutex_;

  std::condition_variable condition_variable_;

  std::atomic_bool running_;

  std::queue<Task> tasks_;

  std::atomic_uint_fast32_t task_count_;

  std::thread* threads_;

  size_t thread_count_;
};
} // namespace genebits::engine

#endif
