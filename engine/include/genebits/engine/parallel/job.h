#ifndef GENEBITS_ENGINE_PARALLEL_JOB_H
#define GENEBITS_ENGINE_PARALLEL_JOB_H

#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/thread_pool.h"
#include "genebits/engine/util/fast_vector.h"
#include "genebits/engine/util/object_pool.h"

namespace genebits::engine
{
///
/// Polymorphic base of any job.
///
/// A job is essentially a task container.
///
/// @note Directly deriving JobBase can be tricky, it is recommended to abstract over a higher level job.
///
class JobBase
{
public:
  ///
  /// Destructor.
  ///
  virtual ~JobBase() = default;

  ///
  /// Obtains the tasks for this job.
  ///
  /// @note Should always return the same tasks.
  ///
  /// @return List of tasks that belong to this job.
  ///
  virtual TaskList GetTasks() = 0;

  ///
  /// Waits for all tasks for this job to finish.
  ///
  virtual void Wait() = 0;
};

///
/// Fixed size job array.
///
/// Used to group jobs together. The job group never assumes ownership of the jobs. It acts as a container
/// to operate on (example waiting on all jobs).
///
/// @tparam MaxJobs Maximum amount of jobs in this group.
///
template<size_t MaxJobs>
class JobGroup
{
public:
  ///
  /// Combines two job groups together.
  ///
  /// @param[in] other Job group to add jobs from.
  ///
  void Combine(const JobGroup* const other)
  {
    ASSERT(other, "Job group is nullptr");
    ASSERT(count_ + other->count_ <= MaxJobs, "Job handles cannot contain more than 64 jobs");

    std::uninitialized_copy(other->jobs_, other->jobs_ + other->count_, jobs_ + count_);

    count_ += other->count_;
  }

  ///
  /// Waits for all jobs from this group to finish.
  ///
  void Wait()
  {
    // Greedy Algorithm. Wait last jobs added first. May decrease wait calls.

    const auto last = jobs_ - 1;

    for (auto first = last + count_; first != last; --first)
    {
      (*first)->Wait();
    }
  }

  ///
  /// Adds a job to the group.
  ///
  /// @param[in] job Job to add to group.
  ///
  constexpr void Add(JobBase* job)
  {
    ASSERT(count_ < MaxJobs, "Job group full");

    jobs_[count_++] = job;
  }

  ///
  /// Reinitialize the job group with a single job.
  ///
  /// @param[in] job Job to initialize group with.
  ///
  constexpr void Reset(JobBase* job)
  {
    jobs_[0] = job;
    count_ = 1;
  }

  ///
  /// Returns the amount of jobs currently in the group.
  ///
  /// @return Amount of jobs in group.
  ///
  [[nodiscard]] constexpr size_t Count() const
  {
    return count_;
  }

private:
  JobBase* jobs_[MaxJobs];
  size_t count_;
};

class JobScheduler;

///
/// Handle to a group of scheduled jobs.
///
/// Created by a job scheduler and returned to the client in order to operate on scheduled jobs.
///
/// @warning Must always be manually completed to avoid memory leak.
///
class JobHandle
{
public:
  ///
  /// Default constructor.
  ///
  constexpr JobHandle() noexcept : group_(nullptr) {}

  ///
  /// Completes all the jobs in this handle.
  ///
  /// For each job, if not finished, waits until done.
  ///
  void Complete();

  ///
  /// Returns the amount of jobs handled by this JobHandle.
  ///
  /// @return Amount of jobs for this handle.
  ///
  [[nodiscard]] constexpr size_t Count() const noexcept
  {
    return group_->Count();
  }

  ///
  /// Whether or not the job handle has any jobs.
  ///
  /// @return True if job handle has atleast one job, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return group_ != nullptr;
  }

private:
  friend JobScheduler;

  using JobGroupType = JobGroup<64>;

  constexpr JobHandle(JobGroupType* group, JobScheduler* scheduler) noexcept : group_(group), scheduler_(scheduler) {}

private:
  JobGroupType* group_;
  JobScheduler* scheduler_;
};

///
/// Manages the scheduling of jobs.
///
/// Efficiently schedules jobs and manages memory overhead.
///
///
class JobScheduler
{
public:
  ///
  /// Constructs a job scheduler.
  ///
  /// @param[in] thread_pool Thead pool to schedule jobs on.
  ///
  constexpr explicit JobScheduler(ThreadPool& thread_pool) noexcept : thread_pool_(thread_pool) {}

  ///
  /// Creates a new job handle, completes the dependencies and schedules the job in this order.
  ///
  /// Does not add job to a chain, immediately blocks if the dependencies are not finished. This
  /// is much simpler and can be faster if used correctly.
  ///
  /// It is better to complete dependencies using this method than completed the dependencies manually.
  ///
  /// @param[in] job Job to schedule.
  /// @param[in] dependencies Dependencies to complete before scheduling.
  ///
  /// @return JobHandle with scheduled job.
  ///
  [[nodiscard]] JobHandle Schedule(JobBase* job, JobHandle dependencies)
  {
    JobHandle handle = CreateJobHandle(job);

    Complete(dependencies);

    SumbitJobTasks(job);

    return handle;
  }

  ///
  /// Creates a new job handle, and schedules the job.
  ///
  /// Immediately schedules the job.
  ///
  /// @param[in] job Job to schedule.
  ///
  /// @return JobHandle with scheduled job.
  ///
  [[nodiscard]] JobHandle Schedule(JobBase* job)
  {
    JobHandle handle = CreateJobHandle(job);

    SumbitJobTasks(job);

    return handle;
  }

  ///
  /// Combines two job handles together. All jobs from both handles will be merged into one.
  ///
  /// @param[in] handle Main handle.
  /// @param[in] other Other handle.
  ///
  /// @return Combined handle.
  ///
  JobHandle CombineJobHandles(JobHandle handle, JobHandle other)
  {
    handle.group_->Combine(other.group_);

    job_group_pool_.Release(other.group_);

    return handle;
  }

  ///
  /// Completes the jobs for the handle.
  ///
  /// @param[in] handle JobHandle to complete
  ///
  void Complete(JobHandle handle)
  {
    if (!handle.group_) return;

    handle.group_->Wait();

    job_group_pool_.Release(handle.group_);
  }

private:
  ///
  /// Creates a job handle.
  ///
  /// Minimizes heap allocations by using an object pool.
  ///
  /// @param[in] job Job to create handle for.
  ///
  /// @return Handle with job.
  ///
  JobHandle CreateJobHandle(JobBase* job)
  {
    JobHandle::JobGroupType* group = job_group_pool_.AcquireUninitialized();

    group->Reset(job); // Will initialize properly

    return JobHandle { group, this };
  }

  ///
  /// Submits all the tasks for the job.
  ///
  /// @param[in] job Job to sumbit tasks for.
  ///
  void SumbitJobTasks(JobBase* job)
  {
    thread_pool_.EnqueueAll(job->GetTasks());
  }

private:
  ThreadPool& thread_pool_;
  ObjectPool<JobHandle::JobGroupType> job_group_pool_;
};

// Job handle should not have any behaviour it should delegate calls to scheduler.
// Therefore, delegated methods must be implemented after scheduler has been declared.

inline void JobHandle::Complete()
{
  scheduler_->Complete(*this);
}

///
/// Concept for basic job executor.
///
/// Any job that inherits basic job must implement this concept.
///
/// @tparam Job Job type to check.
///
template<typename Job>
concept BasicJobExecutor = requires(Job job)
{
  job();
};

///
/// Simple job that only has one task.
///
/// This job barely has any overhead over a task.
///
/// Inherit this job and implement BasicJobExecutor concept.
///
/// @tparam[in] Job Implementation type.
///
template<typename Job>
class BasicJob : public JobBase, private Task
{
public:
  // We can inherit from Task since there is never more than one task to iterate on, so
  // we don't care about the size of the job.

  ///
  /// Default constructor.
  ///
  constexpr BasicJob() : Task()
  {
    static_assert(std::is_base_of_v<BasicJob, Job> && BasicJobExecutor<Job>);

    Executor().template Bind<Job, &Job::operator()>(static_cast<Job*>(this));
  }

  ///
  /// Waits for job to finish.
  ///
  void Wait() noexcept final
  {
    Task::Wait();
  }

  ///
  /// Returns the task list for this job.
  ///
  /// Always one job.
  ///
  /// @return List of tasks for this job.
  ///
  constexpr TaskList GetTasks() noexcept final
  {
    return { static_cast<Task*>(this), 1 };
  }
};

///
/// Lambda adapter for a basic job.
///
/// @tparam[in] Functor Functor type to make basic job with.
///
template<typename Functor>
class BasicLambdaJob : public BasicJob<BasicLambdaJob<Functor>>
{
public:
  ///
  /// Constructor.
  ///
  /// @param[in] functor Functor for job.
  ///
  BasicLambdaJob(Functor functor) : BasicJob<BasicLambdaJob<Functor>>(), functor_(std::move(functor)) {}

  ///
  /// Delegates calls to functor.
  ///
  void operator()()
  {
    functor_();
  }

private:
  Functor functor_;
};

///
/// Concept for parallel for job executor.
///
/// Any job that inherits parallel for job must implement this concept.
///
/// @tparam Job Job type to check.
///
template<typename Job>
concept ParallelForJobExecutor = requires(Job job, size_t index)
{
  job(index);
};

///
/// Parallel for job. Splits work into multiple tasks.
///
/// Good for large quantiles of work, but does have significant overhead. It is recommended
/// to start with a basic job and move to ParallelForJob if benefits are obvious.
///
/// Inherit this job and implement BasicJobExecutor concept.
///
/// @note Amount of tasks can never exceed 8.
///
/// @tparam[in] Job Implementation type.
///
template<typename Job>
class ParallelForJob : public JobBase
{
public:
  ///
  /// Constructor.
  ///
  /// @param[in] amount Amount of iterations of work.
  /// @param[in] batches Preferred amount of batches (1 batch = 1 task).
  ///
  ParallelForJob(size_t amount, size_t batches)
  {
    static_assert(std::is_base_of_v<ParallelForJob<Job>, Job> && ParallelForJobExecutor<Job>);

    CreateBatchTasks(amount, batches);
  }

  ///
  /// Constructor.
  ///
  /// Uses maximum amount of batches.
  ///
  /// @param[in] amount Amount of iterations of work.
  ///
  ParallelForJob(size_t amount) : ParallelForJob(amount, cMaxBatches) {}

  ///
  /// Waits for job to finish.
  ///
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

  ///
  /// Returns the task list for this job.
  ///
  /// @return List of tasks for this job.
  ///
  constexpr TaskList GetTasks() noexcept final
  {
    return { static_cast<Task*>(tasks_), task_count_ };
  }

private:
  static constexpr size_t cMaxBatches = 8;

  ///
  /// Custom task for this job.
  ///
  /// Contains information about section of work.
  ///
  struct ParallelForJobTask : public Task
  {
    size_t start;
    size_t end;
  };

  // Since we cast to Task for TaskList this is necessary for iteration.
  // Reminder: Task is cache line aligned, so there is free space when inheriting it.
  static_assert(sizeof(ParallelForJobTask) == sizeof(Task), "ParallelForJobTask to large.");

  ///
  /// Meta programming utility for building tasks. Allows us to assign executors with
  /// compile time task index so that every task is aware of how to obtain its own data.
  ///
  /// @tparam N Amount of tasks.
  ///
  template<int N>
  struct BatchTaskBuilder
  {
    ///
    /// Builds all the tasks.
    ///
    /// @param[in] job This job.
    /// @param[in] batches Amount of batches (used tasks).
    /// @param[in] batch_size Size of every batch.
    /// @param[in] batch_remainder Remainder to be distributed among batches.
    ///
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

  ///
  /// Creates batch tasks based on the amount of iterations needed and the preferred amount of batches.
  ///
  /// Amount of batches cannot exceed maximum amount of batches and cannot be bigger than the amount of
  /// iterations.
  ///
  /// @param[in] amount_iterations Amount of iterations to split between batches.
  /// @param[in] preferred_batches Preferred amount of batches (Tasks) to create.
  ///
  void CreateBatchTasks(size_t amount_iterations, size_t preferred_batches)
  {
    static const size_t processors = GetAmountPhysicalProcessors();

    const size_t batch_hint = processors == 0 ? preferred_batches : std::min(processors, preferred_batches);

    const size_t batches = amount_iterations < batch_hint ? amount_iterations : batch_hint;
    const size_t batch_size = amount_iterations / batches;
    const size_t batch_remainder = amount_iterations % batches;

    task_count_ = batches;

    BatchTaskBuilder<cMaxBatches>().BuildAll(this, batches, batch_size, batch_remainder);
  }

  ///
  /// Creates a single batch task for the task index.
  ///
  /// @tparam TaskIndex Compile time task index to construct executor with.
  ///
  /// @param[in] batches Total amount of batches.
  /// @param[in] batch_size Size of every batch.
  /// @param[in] batch_remainder Remainder to be distributed among batches.
  ///
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

  ///
  /// Executor for the task with the index.
  ///
  /// @tparam TaskIndex Task index for executor.
  ///
  template<size_t TaskIndex>
  void TaskExecute()
  {
    ParallelForJobTask& task_ = tasks_[TaskIndex];

    Job* job = static_cast<Job*>(this);

    for (size_t i = task_.start; i != task_.end; ++i)
    {
      job->operator()(i);
    }
  }

private:
  ParallelForJobTask tasks_[cMaxBatches];
  size_t task_count_;
};

///
/// Lambda adapter for a parallel for job.
///
/// @tparam[in] Functor Functor type to make parallel for job with.
///
template<typename Functor>
class ParallelForLambdaJob : public ParallelForJob<ParallelForLambdaJob<Functor>>
{
public:
  ///
  /// Constructor.
  ///
  /// @param[in] functor Functor for job.
  /// @param[in] amount Amount of iterations of work.
  /// @param[in] batches Preferred amount of batches (1 batch = 1 task).
  ///
  ParallelForLambdaJob(Functor functor, size_t amount, size_t batches)
    : ParallelForJob<ParallelForLambdaJob<Functor>>(amount, batches), functor_(std::move(functor))
  {}

  ///
  /// Constructor.
  ///
  /// Uses maximum amount of batches.
  ///
  /// @param[in] functor Functor for job.
  /// @param[in] amount Amount of iterations of work.
  ///
  ParallelForLambdaJob(Functor functor, size_t amount)
    : ParallelForJob<ParallelForLambdaJob<Functor>>(amount), functor_(std::move(functor))
  {}

  ///
  /// Delegates call to functor.
  ///
  /// @param[in] index Iteration index.
  ///
  void operator()(size_t index)
  {
    functor_(index);
  }

private:
  Functor functor_;
};

} // namespace genebits::engine

#endif
