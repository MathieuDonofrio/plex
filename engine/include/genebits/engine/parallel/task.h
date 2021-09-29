#ifndef GENEBITS_ENGINE_PARALLEL_TASK_H
#define GENEBITS_ENGINE_PARALLEL_TASK_H

#include "genebits/engine/parallel/threading.h"
#include "genebits/engine/util/delegate.h"

namespace genebits::engine
{
///
/// Delegate alias for executing tasks.
///
/// @see Delegate
///
using TaskExecutor = Delegate<void()>;

///
/// Task to be executed by thread pool.
///
/// @note Aligned on the cache line to avoid cache synchronization.
///
class alignas(std::hardware_destructive_interference_size) Task
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Task() noexcept : next_(nullptr), flag_(false) {}

  ///
  /// Copy constructor.
  ///
  /// @param[in] other Task to copy.
  ///
  Task(const Task& other)
    : executor_(other.executor_), next_(other.next_), flag_(other.flag_.load(std::memory_order_relaxed))
  {}

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  ~Task()
  {
    ASSERT(Finished(), "Task was destroyed but not finished");
  }
#endif

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Task to copy.
  ///
  /// @return Reference to this task.
  ///
  Task& operator=(const Task& other)
  {
    if (&other != this)
    {
      executor_ = other.executor_;
      next_ = other.next_;
      flag_ = other.flag_.load(std::memory_order_relaxed);
    }

    return *this;
  }

  ///
  /// Spins for a fixed amount of specified spins.
  ///
  /// It may be an optimization to spin a little before waiting in certain cases such
  /// as small tasks.
  ///
  /// @tparam Spins Maximum amount of spins.
  ///
  /// @return True if the attempt to poll worked, false otherwise.
  ///
  template<size_t Spins = 512>
  bool TryPoll() const noexcept
  {
    for (size_t i = 0; i < Spins; i++)
    {
      if (Finished()) return true;

      std::this_thread::yield();
    }

    return Finished();
  }

  ///
  /// Spins until task is finished.
  ///
  /// @note Always prefer waiting to polling. Polling is considered dubious by many, however is
  /// can still be an optimization in certain cases.
  ///
  /// @warning High CPU usage when using this.
  ///
  void Poll() const noexcept
  {
    while (!Finished())
    {
      std::this_thread::yield();
    }
  }

  ///
  /// Wait until the task is finished.
  ///
  /// If the task is not finished, this will block until notified.
  ///
  /// @note Does not use up CPU when waiting.
  ///
  void Wait() const noexcept
  {
    while (!Finished())
    {
      flag_.wait(false, std::memory_order_relaxed);
    }
  }

  ///
  /// Sets the state of the task to finished and notifies all waiting threads.
  ///
  void Finish()
  {
    ASSERT(!flag_.load(std::memory_order_relaxed), "Task already finished");

    flag_.store(true, std::memory_order_relaxed);
    flag_.notify_all();
  }

  ///
  /// Returns a reference to the task executor.
  ///
  /// @return Task executor reference.
  ///
  [[nodiscard]] TaskExecutor& Executor() noexcept
  {
    return executor_;
  }

  ///
  /// Returns whether or not this task is finished.
  ///
  /// @return True if task is finished, false otherwise.
  ///
  [[nodiscard]] bool Finished() const noexcept
  {
    return flag_.load(std::memory_order_relaxed);
  }

private:
  friend class TaskQueue;

  TaskExecutor executor_;
  Task* next_;
  std::atomic_bool flag_;
};

///
/// Task with extra data.
///
/// @note If the data is small, there is no memory overhead because of the aligned task.
///
/// @see Task
///
template<typename Data>
class DataTask : public Task
{
public:
  ///
  /// Returns the data associated with this task.
  ///
  /// @return Data of the task.
  ///
  [[nodiscard]] constexpr Data& GetData() noexcept
  {
    return data;
  };

  ///
  /// Returns the data associated with this task.
  ///
  /// @return Data of the task.
  ///
  [[nodiscard]] constexpr const Data& GetData() const noexcept
  {
    return data;
  };

private:
  Data data;
};

///
/// Very low overhead task queue.
///
/// @note Not thread safe.
///
/// @note Does not perform any heap allocations.
///
class TaskQueue
{
public:
  ///
  /// Default constructor.
  ///
  constexpr TaskQueue() : front_(nullptr), back_(nullptr) {}

  TaskQueue(TaskQueue& other) = delete;
  TaskQueue& operator=(TaskQueue& other) = delete;

  ///
  /// Adds a task to the back of the queue.
  ///
  /// @param[in] task Task to add.
  ///
  constexpr void Push(Task* task) noexcept
  {
    ASSERT(!task->next_, "New task cannot have next task");

    if (front_) back_ = back_->next_ = task;
    else
    {
      front_ = back_ = task;
    }
  }

  ///
  /// Removes the task from the front of the queue.
  ///
  constexpr void Pop() noexcept
  {
    ASSERT(front_, "Queue cannot be empty");

    front_ = front_->next_;
  }

  ///
  /// Returns the task at the front of the queue.
  ///
  /// @return Task at the front of the queue, nullptr if the task is empty.
  ///
  [[nodiscard]] constexpr Task* Front() noexcept
  {
    return front_;
  }

  ///
  /// Returns whether or not the queue is empty.
  ///
  /// @return True if the queue is empty, false otherwise.
  ///
  [[nodiscard]] constexpr bool Empty() const noexcept
  {
    return !front_;
  }

private:
  Task* front_;
  Task* back_;
};
} // namespace genebits::engine

#endif
