#ifndef GENEBITS_ENGINE_ECS_SYSTEM_H
#define GENEBITS_ENGINE_ECS_SYSTEM_H

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/registry.h"
#include "genebits/engine/parallel/job.h"
#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/thread_pool.h"
#include "genebits/engine/util/allocator.h"

#include <iostream>
#include <list>
#include <set>

namespace genebits::engine
{
struct SystemDataAccess
{
  ComponentId id;
  bool read_only;
};

using SystemDataAccessList = FastVector<SystemDataAccess>;

template<typename... Components>
class System;

class SystemGroup;

class Phase;

class SystemBase
{
public:
  SystemBase() : registry_(nullptr), scheduler_(nullptr) {};

  virtual ~SystemBase() = default;

  SystemBase(const SystemBase&) = delete;
  SystemBase& operator=(const SystemBase&) = delete;
  SystemBase(SystemBase&&) = delete;
  SystemBase& operator=(SystemBase&&) = delete;

  void Run(JobHandle dependencies)
  {
    ASSERT(Initialized(), "System not initialized");

    OnUpdate(dependencies);
  }

  void ForceComplete()
  {
    ASSERT(Initialized(), "System not initialized");

    scheduler_->Complete(handle_);
  }

  virtual SystemDataAccessList GetDataAccess() = 0;

protected:
  template<typename... Components>
  friend class System;
  friend SystemGroup;
  friend Phase;

  virtual void OnUpdate(JobHandle dependencies) = 0;

  [[nodiscard]] constexpr bool Initialized() const noexcept
  {
    return registry_ && scheduler_;
  }

private:
  Registry* registry_;
  JobScheduler* scheduler_;

  JobHandle handle_;
};

template<typename... Components>
class System : public SystemBase
{
public:
  SystemDataAccessList GetDataAccess() final
  {
    SystemDataAccessList access;

    access.Reserve(sizeof...(Components));

    (access.PushBack({ GetComponentId<std::remove_cvref_t<Components>>(), std::is_const_v<Components> }), ...);

    return access;
  }

  template<typename JobType>
  JobHandle ScheduleDefered(JobType&& job, JobHandle dependencies)
  {
    ASSERT(Initialized(), "System not initialized");

    handle_ = scheduler_->Schedule(std::forward<JobType>(job), dependencies);

    return handle_;
  }

  PolyView<Components...> GetView()
  {
    ASSERT(Initialized(), "System not initialized");

    return registry_->template View<Components...>();
  }

  [[nodiscard]] constexpr const JobScheduler& GetScheduler() noexcept
  {
    ASSERT(Initialized(), "System not initialized");

    return *scheduler_;
  }
};

class SystemGroup
{
public:
  SystemGroup() = default;

  SystemGroup(const SystemGroup&) = delete;
  SystemGroup& operator=(const SystemGroup&) = delete;
  SystemGroup(SystemGroup&&) = delete;
  SystemGroup& operator=(SystemGroup&&) = delete;

  void InitializeSystems(Registry& registry, JobScheduler& job_scheduler)
  {
    for (SystemBase* system : registered_systems_)
    {
      system->registry_ = &registry;
      system->scheduler_ = &job_scheduler;
    }
  }

  void Add(SystemBase* system)
  {
    registered_systems_.PushBack(system);
  }

  [[nodiscard]] constexpr size_t Count() const noexcept
  {
    return registered_systems_.Size();
  }

  [[nodiscard]] constexpr SystemBase** RawSystems() noexcept
  {
    return registered_systems_.data();
  }

private:
  FastVector<SystemBase*> registered_systems_;
};

class Phase
{
public:
  void Run()
  {
    for (auto it = compiled_.begin(); it != compiled_.end(); ++it)
    {
      SystemBase& system = *(it->system);

      ASSERT(system.Initialized(), "System not initialized");

      JobHandle dependencies;

      for (SystemBase* dependency : it->sync)
      {
        dependencies = system.scheduler_->CombineJobHandles(dependencies, dependency->handle_);
      }

      system.Run(dependencies);

      // TODO Assert dependencies where completed ?
    }
  }

  void ForceComplete()
  {
    for (auto it = compiled_.begin(); it != compiled_.end(); ++it)
    {
      ASSERT(it->system->Initialized(), "System not initialized");

      it->system->ForceComplete();
    }
  }

  static Phase Compile(SystemBase** systems, size_t dependencies, size_t total);

  static Phase Compile(SystemGroup& group, std::initializer_list<SystemGroup*> dependencies);

  static Phase Compile(SystemGroup& group);

public:
  struct CompiledSystem
  {
    SystemBase* system;
    FastVector<SystemBase*> sync;
  };

  [[nodiscard]] constexpr CompiledSystem* begin() noexcept
  {
    return compiled_.begin();
  }

  [[nodiscard]] constexpr CompiledSystem* end() noexcept
  {
    return compiled_.end();
  }

private:
  constexpr Phase(FastVector<CompiledSystem>&& compiled) : compiled_(std::move(compiled)) {}

private:
  FastVector<CompiledSystem> compiled_;
};

///
/// Simple entities job that only has one task.
///
/// This job barely has any overhead over a task.
///
/// Inherit this job and implement EntitiesJob concept.
///
/// @tparam[in] Job Implementation type.
///
template<typename Job, typename... Components>
class EntitiesJob : public JobBase, private Task
{
public:
  // We can inherit from Task since there is never more than one task to iterate on, so
  // we don't care about the size of the job.

  ///
  /// Default constructor.
  ///
  EntitiesJob(PolyView<Components...> view) : Task(), view_(view)
  {
    static_assert(std::is_base_of_v<EntitiesJob, Job>);

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

  ///
  /// Returns the view for this job.
  ///
  /// @return Entities view.
  ///
  PolyView<Components...> GetView() noexcept
  {
    return view_;
  }

private:
  ///
  /// Iterates every entity in the view, unpacks and calls the job operator.
  ///
  void UpdateAll()
  {
    for (auto mono_view : view_)
    {
      for (auto& data : mono_view)
      {
        EntityApply<Job, Components...>(&Job::operator(), data);
      }
    }
  }

private:
  PolyView<Components...> view_;
};
} // namespace genebits::engine

#endif
