#ifndef GENEBITS_ENGINE_JOBS_THREAD_POOL_H
#define GENEBITS_ENGINE_JOBS_THREAD_POOL_H

#include <atomic>
#include <mutex>

#include "genebits/engine/util/concurrency.h"
#include "genebits/engine/util/delegate.h"

namespace genebits::engine
{
using TaskExecutor = Delegate<>;

class Task
{
public:
  constexpr Task() noexcept : next_(nullptr), flag_(false) {}

  Task(const Task& other)
    : executor_(other.executor_), next_(other.next_), flag_(other.flag_.load(std::memory_order_relaxed))
  {}

  Task& operator=(const Task& other)
  {
    executor_ = other.executor_;
    next_ = other.next_;
    flag_ = other.flag_.load(std::memory_order_relaxed);

    return *this;
  }

  void Complete() const noexcept
  {
    while (!Finished())
    {
      flag_.wait(false, std::memory_order_relaxed);
    }
  }

  void Finish()
  {
    ASSERT(!flag_.load(std::memory_order_relaxed), "Task already finished");

    flag_.store(true, std::memory_order_relaxed);
    flag_.notify_all();
  }

  [[nodiscard]] TaskExecutor& Executor() noexcept
  {
    return executor_;
  }

  [[nodiscard]] bool Finished() const noexcept
  {
    return flag_.load(std::memory_order_relaxed);
  }

private:
  friend class TaskQueue;

  Delegate<> executor_;
  Task* next_;
  std::atomic_bool flag_;
};

class TaskQueue
{
public:
  constexpr TaskQueue() : front_(nullptr), back_(nullptr) {}

  TaskQueue(TaskQueue& other) = delete;
  TaskQueue& operator=(TaskQueue& other) = delete;

  constexpr void Pop() noexcept
  {
    ASSERT(front_, "Queue cannot be empty");

    front_ = front_->next_;
  }

  constexpr void Push(Task* task) noexcept
  {
    ASSERT(!task->next_, "New task cannot have next task");

    if (front_) back_ = back_->next_ = task;
    else
    {
      front_ = back_ = task;
    }
  }

  constexpr Task* Front() noexcept
  {
    return front_;
  }

  constexpr bool Empty() const noexcept
  {
    return !front_;
  }

private:
  Task* front_;
  Task* back_;
};

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

  void Schedule(Task* task)
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
        // Sleep a little to free CPU
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
