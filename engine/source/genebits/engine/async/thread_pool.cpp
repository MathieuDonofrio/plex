#include "genebits/engine/async/thread_pool.h"

#include "genebits/engine/async/exponential_backoff.h"
#include "genebits/engine/os/cpu_info.h"
#include "genebits/engine/os/thread.h"

namespace genebits::engine
{
size_t GetDefaultAmountOfWorkerThreads()
{
  size_t physical_processors = GetAmountPhysicalProcessors();

  // For large amount of cores using logical processors when Hyper-Threading is enabled will probably just consume
  // unnecessarily more resources.
  if (physical_processors <= 16)
  {
    // Using logical core amount does improve performance with Hyper-Threading.
    size_t logical_processors = GetAmountLogicalProcessors();

    if (logical_processors == 2 * physical_processors) // Hyper Threading Enabled
    {
      return logical_processors;
    }
  }

  return physical_processors;
}

ThreadPool::ThreadPool(const size_t thread_count, bool lock_threads)
  : running_(false), threads_(nullptr), thread_count_(thread_count)
{
  ASSERT(thread_count > 0, "Thread pool cannot have 0 threads");

  CreateWorkers();

  if (lock_threads) SetWorkerThreadAffinity();
}

ThreadPool::ThreadPool() : ThreadPool(GetDefaultAmountOfWorkerThreads(), true) {}

ThreadPool::~ThreadPool()
{
  DestroyWorkers();
}

void ThreadPool::RunWorker()
{
  this_thread::SetName("Worker");

  std::unique_lock lock(mutex_);

  Operation* op;

  while (running_ || !queue_.Empty()) // Always locked when doing an iteration
  {
    op = queue_.Front();

    if (op != nullptr)
    {
      do
      {
        queue_.Dequeue();

        // Make sure we are unlocked for task execution
        lock.unlock();

        op->Execute();

        // Spin for a little while until we think we have more work to do.
        // Avoids putting the worker to sleep only to wake up again.
        ExponentialBackoff backoff;

        for (size_t i = 0; i != 16 && !queue_.HasWorkApprox(); i++)
        {
          backoff.Wait();
        }

        lock.lock();

        op = queue_.Front();
      }
      while (op != nullptr);
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

void ThreadPool::Enqueue(Operation* operation)
{
  mutex_.lock();

  ASSERT(running_, "Cannot enqueue operation when thread pool not running");

  queue_.Enqueue(operation);

  // Manual unlocking is done before notifying, to avoid waking up
  // the waiting thread only to block again.
  // https://en.cppreference.com/w/cpp/thread/condition_variable
  mutex_.unlock();

  // Every time we enqueue an operation, we compulsively try to wake up one worker. This guarantees that either all
  // workers are active or one worker per operation. This still allows the possibility for some workers to stay asleep
  // if there is not enough work to be done, reducing resource usage and contention.
  condition_.notify_one();
}

void ThreadPool::CreateWorkers()
{
  ASSERT(!running_, "Thread pool already running");

  running_ = true;

  threads_ = new std::thread[thread_count_];

  for (size_t i = 0; i < thread_count_; i++)
  {
    new (threads_ + i) std::thread(&ThreadPool::RunWorker, this);
  }
}

void ThreadPool::SetWorkerThreadAffinity()
{
  const CPUInfo info = GetCPUInfo();

  const auto core_count = info.processors.size();

  for (size_t i = 0; i < thread_count_; i++)
  {
    SetThreadAffinity(threads_[i].native_handle(), info.processors[i % core_count].mask);
  }
}

void ThreadPool::DestroyWorkers()
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

  ASSERT(queue_.Empty(), "There is still work left");

  mutex_.unlock();
#endif

  delete[] threads_;
}

} // namespace genebits::engine