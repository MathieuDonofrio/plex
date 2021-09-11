#ifndef GENEBITS_ENGINE_THREADING_TASK_H
#define GENEBITS_ENGINE_THREADING_TASK_H

#include "genebits/engine/threading/thread.h"
#include "genebits/engine/util/delegate.h"

namespace genebits::engine
{
using TaskExecutor = Delegate<>;

// Aligned on a cache line to avoid cache synchronization
class alignas(std::hardware_destructive_interference_size) Task
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

  bool TryPoll(size_t spins = 512) const noexcept
  {
    for (;;)
    {
      if (Finished()) return true;

      if (spins-- == 0) return false;

      this_thread::Pause();
    }
  }

  void Poll() const noexcept
  {
    if (!Finished())
    {
      ExponentialBackoff backoff;

      while (!Finished())
      {
        backoff.Wait();
      }
    }
  }

  void Wait() const noexcept
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

  [[nodiscard]] constexpr Task* Front() noexcept
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
} // namespace genebits::engine

#endif
