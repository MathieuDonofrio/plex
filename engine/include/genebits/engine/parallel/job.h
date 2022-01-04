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

  virtual void Wait() = 0;
};

class JobGroup
{
public:
  static constexpr size_t cMaxJobs = 64;

  void Combine(JobGroup* other)
  {
    ASSERT(other, "Job group is nullptr");
    ASSERT(count_ + other->count_ <= cMaxJobs, "Job handles cannot contain more than 64 jobs");

    std::uninitialized_copy(other->jobs_, other->jobs_ + other->count_, jobs_ + count_);

    count_ += other->count_;
  }

  void Wait()
  {
    const auto last = jobs_ - 1;

    for (auto first = last + count_; first != last; --first)
    {
      (*first)->Wait();
    }
  }

  constexpr void PushBack(JobBase* job)
  {
    ASSERT(count_ < cMaxJobs, "Job group full");

    jobs_[count_++] = job;
  }

  constexpr void ClearAndPushBack(JobBase* job)
  {
    jobs_[0] = job;
    count_ = 1;
  }

  constexpr size_t Count() const
  {
    return count_;
  }

private:
  JobBase* jobs_[cMaxJobs];
  size_t count_;
};

class JobScheduler;

class JobHandle
{
public:
  constexpr JobHandle() noexcept : group_(nullptr) {}

  constexpr JobHandle(JobGroup* group, JobScheduler* scheduler) noexcept : group_(group), scheduler_(scheduler) {}

  void Combine(JobHandle handle);

  void Complete();

  [[nodiscard]] constexpr size_t Count() const noexcept
  {
    return group_->Count();
  }

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return group_ != nullptr;
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

  ~JobScheduler()
  {
    DestroyJobGroups();
  }

  [[nodiscard]] JobHandle Schedule(JobBase* job, JobHandle dependencies)
  {
    JobHandle handle = CreateJobHandle(job);

    Complete(dependencies);

    SumbitJobTasks(job);

    return handle;
  }

  [[nodiscard]] JobHandle Schedule(JobBase* job)
  {
    JobHandle handle = CreateJobHandle(job);

    SumbitJobTasks(job);

    return handle;
  }

  JobHandle CombineJobHandles(JobHandle handle, JobHandle other)
  {
    handle.group_->Combine(other.group_);

    DestroyJobHandle(other);

    return handle;
  }

  void Complete(JobHandle handle)
  {
    if (!handle) return;

    handle.group_->Wait();

    DestroyJobHandle(handle);
  }

private:
  JobHandle CreateJobHandle(JobBase* job)
  {
    JobGroup* group = GetJobGroup();

    group->ClearAndPushBack(job);

    return JobHandle { group, this };
  }

  void DestroyJobHandle(JobHandle handle)
  {
    recycled_job_groups_.PushBack(handle.group_);
  }

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

  void SumbitJobTasks(JobBase* job)
  {
    TaskList tasks = job->GetTasks();

    pool_.EnqueueAll(tasks.first, tasks.last);
  }

  void DestroyJobGroups()
  {
    for (JobGroup* group : recycled_job_groups_)
    {
      delete group;
    }
  }

private:
  ThreadPool& pool_;

  FastVector<JobGroup*> recycled_job_groups_;
};

template<typename Function>
concept BasicJobFunction = requires(Function function)
{
  function();
};

template<BasicJobFunction Function>
class BasicJob : public JobBase
{
public:
  constexpr explicit BasicJob(Function executor) noexcept : task_(std::move(executor))
  {
    task_.Executor().template Bind([this]() { task_.executor_(); });
  }

  void Wait() noexcept final
  {
    task_.Wait();
  }

  constexpr TaskList GetTasks() noexcept final
  {
    return { &task_, &task_ + 1 };
  }

private:
  class DataTask : public Task
  {
  public:
    constexpr DataTask(Function&& executor) : executor_(executor) {}

  private:
    Function executor_;
  };

  DataTask task_;
};

template<BasicJobFunction Function>
requires DelegateInvocable<Function, void>
class BasicJob<Function> : public JobBase
{
public:
  constexpr explicit BasicJob(Function executor) noexcept
  {
    task_.Executor().Bind(std::move(executor));
  }

  void Wait() noexcept final
  {
    task_.Wait();
  }

  constexpr TaskList GetTasks() noexcept final
  {
    return { &task_, &task_ + 1 };
  }

private:
  Task task_;
};

inline void JobHandle::Combine(JobHandle handle)
{
  scheduler_->CombineJobHandles(*this, handle);
}

inline void JobHandle::Complete()
{
  scheduler_->Complete(*this);
}

} // namespace genebits::engine

#endif
