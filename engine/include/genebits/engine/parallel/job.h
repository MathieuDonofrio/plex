#ifndef GENEBITS_ENGINE_PARALLEL_JOB_H
#define GENEBITS_ENGINE_PARALLEL_JOB_H

#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/thread_pool.h"
#include "genebits/engine/util/fast_vector.h"

namespace genebits::engine
{
class JobBase
{
public:
  virtual ~JobBase() = default;

  virtual TaskList GetTasks() = 0;
};

struct JobGroup
{
  JobBase* jobs[64];
  size_t count;
};

class JobScheduler;

class JobHandle
{
public:
  constexpr JobHandle() noexcept : group_(nullptr) {}

  constexpr JobHandle(JobGroup* group, JobScheduler* scheduler) noexcept : group_(group), scheduler_(scheduler) {}

  void Complete();

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return group_;
  }

private:
  friend JobScheduler;

  JobGroup* group_;
  JobScheduler* scheduler_;
};

class JobScheduler
{
public:
  constexpr explicit JobScheduler(ThreadPool& pool) noexcept : pool_(pool) {}

  [[nodiscard]] JobHandle Schedule(JobBase* job)
  {
    TaskList tasks = job->GetTasks();

    pool_.EnqueueAll(tasks.first, tasks.last);

    JobGroup* group = GetJobGroup();

    group->jobs[0] = job;
    group->count = 1;

    return JobHandle { group, this };
  }

  void Complete(JobHandle handle)
  {
    // Greedy complete. Start with last added task.

    JobBase** end = handle.group_->jobs - 1;
    JobBase** begin = end + handle.group_->count;

    for (; begin != end; --begin)
    {
      TaskList tasks = (*begin)->GetTasks();

      for (; tasks.first != tasks.last; ++tasks.first)
      {
        tasks.first->Wait();
      }
    }

    recycled_job_groups_.PushBack(handle.group_);
  }

  JobHandle CombineJobHandles(JobHandle h1, JobHandle h2)
  {
    std::uninitialized_copy(h2.group_->jobs, h2.group_->jobs + h2.group_->count, h1.group_->jobs + h1.group_->count);

    h1.group_->count += h2.group_->count;

    recycled_job_groups_.PushBack(h2.group_);

    return h1;
  }

private:
  JobGroup* GetJobGroup()
  {
    if (!recycled_job_groups_.Empty())
    {
      auto back = recycled_job_groups_.back();
      recycled_job_groups_.PopBack();

      return back;
    }

    return new JobGroup();
  }

private:
  ThreadPool& pool_;

  FastVector<JobGroup*> recycled_job_groups_; // TODO Handle mem leak
};

inline void JobHandle::Complete()
{
  scheduler_->Complete(*this);
}

} // namespace genebits::engine

#endif
