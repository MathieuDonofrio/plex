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

template<typename Functor>
class BasicJob : public JobBase, private Task
{
public:
  constexpr BasicJob() : Task()
  {
    static_assert(std::is_base_of_v<BasicJob, Functor>);

    Executor().template Bind<Functor, &Functor::operator()>(static_cast<Functor*>(this));
  }

  void Wait() noexcept final
  {
    Task::Wait();
  }

  constexpr TaskList GetTasks() noexcept final
  {
    return { static_cast<Task*>(this), static_cast<Task*>(this) + 1 };
  }
};

template<typename Functor>
class BasicLambdaJob : public BasicJob<BasicLambdaJob<Functor>>
{
public:
  BasicLambdaJob(Functor functor) : BasicJob<BasicLambdaJob<Functor>>(), functor_(std::move(functor)) {}

  void operator()()
  {
    functor_();
  }

private:
  Functor functor_;
};

template<typename Functor>
class ParallelForJob : public JobBase
{
public:
  ParallelForJob(size_t amount, size_t batches)
  {
    static_assert(std::is_base_of_v<ParallelForJob<Functor>, Functor>);

    CreateBatchTasks(amount, batches);
  }

  ParallelForJob(size_t amount) : ParallelForJob(amount, cMaxBatches) {}

  void Wait() noexcept final
  {
    // Greedy wait. Waits for the last task first.
    // This assumes that the last task was enqueued last and that work is well-balanced between all tasks.

    const auto last = tasks_ - 1;

    for (auto first = last + task_count_; first != last; --first)
    {
      first->Wait();
    }
  }

  constexpr TaskList GetTasks() noexcept final
  {
    return { tasks_, tasks_ + task_count_ };
  }

private:
  static constexpr size_t cMaxBatches = 8;

  struct ParallelForJobTask : public Task
  {
    size_t start;
    size_t end;
  };

  template<int N>
  struct BatchTaskBuilder
  {
    void BuildAll(ParallelForJob* job, size_t batches, size_t batch_size, size_t batch_remainder)
    {
      job->CreateBatchTask<N - 1>(batches, batch_size, batch_remainder);
      BatchTaskBuilder<N - 1>().BuildAll(job, batches, batch_size, batch_remainder);
    }
  };

  template<>
  struct BatchTaskBuilder<0>
  {
    void BuildAll(ParallelForJob*, size_t, size_t, size_t) {}
  };

  void CreateBatchTasks(size_t amount_elements, size_t preferred_batches)
  {
    static const size_t processors = GetAmountPhysicalProcessors();

    const size_t batch_hint = processors == 0 ? preferred_batches : std::min(processors, preferred_batches);

    const size_t batches = amount_elements < batch_hint ? amount_elements : batch_hint;
    const size_t batch_size = amount_elements / batches;
    const size_t batch_remainder = amount_elements % batches;

    task_count_ = batches;

    BatchTaskBuilder<cMaxBatches>().BuildAll(this, batches, batch_size, batch_remainder);
  }

  template<size_t TaskIndex>
  void CreateBatchTask(size_t batches, size_t batch_size, size_t batch_remainder)
  {
    ParallelForJobTask& task_ = tasks_[TaskIndex];

    if (TaskIndex < batches)
    {
      if (TaskIndex < batch_remainder)
      {
        task_.start = TaskIndex * batch_size + TaskIndex;
        task_.end = task_.start + batch_size + 1;
      }
      else
      {
        task_.start = TaskIndex * batch_size + batch_remainder;
        task_.end = task_.start + batch_size;
      }

      task_.Executor().template Bind<ParallelForJob, &ParallelForJob::TaskExecute<TaskIndex>>(this);
    }
    else
    {
      task_.template Finish<false>();
    }
  }

  template<size_t TaskIndex>
  void TaskExecute()
  {
    ParallelForJobTask& task_ = tasks_[TaskIndex];

    for (size_t i = task_.start; i != task_.end; ++i)
    {
      static_cast<Functor*>(this)->operator()(i);
    }
  }

private:
  ParallelForJobTask tasks_[cMaxBatches];
  size_t task_count_;
};

template<typename Functor>
class ParallelForLambdaJob : public ParallelForJob<ParallelForLambdaJob<Functor>>
{
public:
  ParallelForLambdaJob(Functor functor, size_t amount, size_t batches)
    : ParallelForJob<ParallelForLambdaJob<Functor>>(amount, batches), functor_(std::move(functor))
  {}

  ParallelForLambdaJob(Functor functor, size_t amount)
    : ParallelForJob<ParallelForLambdaJob<Functor>>(amount), functor_(std::move(functor))
  {}

  void operator()(size_t index)
  {
    functor_(index);
  }

private:
  Functor functor_;
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
