#ifndef GENEBITS_ENGINE_PARALLEL_JOB_H
#define GENEBITS_ENGINE_PARALLEL_JOB_H

#include "genebits/engine/parallel//thread_pool.h"
#include "genebits/engine/parallel/task.h"
#include "genebits/engine/util/fast_vector.h"

namespace genebits::engine
{
template<typename JobImpl>
concept Job = requires(JobImpl job, ThreadPool& pool, size_t batches)
{
  job.Schedule(pool, batches);

  job.Complete();
};

template<typename JobUpdate>
class MonoJob
{
public:
  ~MonoJob() noexcept
  {
    Complete();
  }

  void Schedule(ThreadPool& pool, size_t = 0) noexcept
  {
    ASSERT(!task_.Finished(), "Job already scheduled");

    task_.Executor().template Bind<JobUpdate, &JobUpdate::Update>(static_cast<JobUpdate*>(this));

    pool.Enqueue(&task_);
  }

  void Complete() noexcept
  {
    task_.Wait();
  }

private:
  Task task_;
};

template<typename JobUpdate, Allocator ChunkAllocator = Mallocator>
class ForJob : private ChunkAllocator
{
public:
  constexpr explicit ForJob(size_t amount) noexcept : amount_(static_cast<uint32_t>(amount)), complete_(false) {}

  ~ForJob() noexcept
  {
    Complete();
    Destroy();
  }

  void Schedule(ThreadPool& pool, size_t batches = 1) noexcept
  {
    ASSERT(!complete_, "Task already complete");
    ASSERT(batches > 0, "Cannot have 0 batches");

    task_count_ = static_cast<uint32_t>(pool.ThreadCount() * batches);

    // We cannot have more tasks than indexes to process
    if (task_count_ > amount_) task_count_ = amount_;

    CreateTasks();

    pool.EnqueueAll(tasks_, tasks_ + task_count_);
  }

  void Complete() noexcept
  {
    if (!complete_)
    {
      // TODO Iterate in reverse to minimize amount of blocking wait calls.
      for (auto it = tasks_; it != tasks_ + task_count_; ++it)
      {
        it->Wait();
      }

      complete_ = true;
    }
  }

private:
  struct ForJobTask : public Task
  {
    constexpr ForJobTask() : Task() {};

    JobUpdate* instance;
    size_t start;
    size_t end;
  };

  static_assert(sizeof(ForJobTask) == std::hardware_destructive_interference_size);

  void CreateTasks()
  {
    const Block block = ChunkAllocator::Allocate(sizeof(ForJobTask) * task_count_);

    tasks_ = reinterpret_cast<ForJobTask*>(block.ptr);

    const auto base_chunk_size = amount_ / task_count_;
    const auto remainder = amount_ - (base_chunk_size * task_count_);

    for (uint16_t i = 0; i != task_count_; i++)
    {
      CreateTask(i, base_chunk_size, remainder);
    }
  }

  void CreateTask(size_t index, size_t base_chunk_size, size_t remainder)
  {
    ForJobTask* task = &tasks_[index];

    new (task) ForJobTask();

    task->instance = static_cast<JobUpdate*>(this);

    if (index < remainder)
    {
      task->start = index * base_chunk_size + index;
      task->end = task->start + base_chunk_size + 1;
    }
    else
    {
      task->start = index * base_chunk_size + remainder;
      task->end = task->start + base_chunk_size;
    }

    const auto executor = [task]()
    {
      auto end = task->end;

      for (size_t j = task->start; j != end; ++j)
      {
        task->instance->Update(j);
      }
    };

    task->Executor().Bind(executor);
  }

  void Destroy()
  {
    ChunkAllocator::Deallocate(Block { reinterpret_cast<char*>(tasks_), amount_ * sizeof(ForJobTask) });
  }

private:
  ForJobTask* tasks_;
  uint32_t task_count_;
  uint32_t amount_;
  bool complete_;
};

} // namespace genebits::engine

#endif
