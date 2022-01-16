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

  JobHandle ScheduleDefered(std::shared_ptr<JobBase> job, JobHandle dependencies, void (*deleter)(JobBase*))
  {
    ASSERT(Initialized(), "System not initialized");

    JobHandle handle = scheduler_->Schedule(job, dependencies);

    handle_ = handle;

    return handle;
  }

  JobHandle ScheduleDefered(std::shared_ptr<JobBase> job, JobHandle dependencies)
  {
    ASSERT(Initialized(), "System not initialized");

    JobHandle handle = scheduler_->Schedule(job, dependencies);

    handle_ = handle;

    return handle;
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

template<typename Update, typename... Components>
requires EntityFunctor<Update, Components...>
class EntitiesJob : public JobBase
{
public:
  constexpr EntitiesJob(Update update, PolyView<Components...> view) noexcept : task_(view, std::move(update))
  {
    task_.Executor().template Bind<EntitiesJob, &EntitiesJob::UpdateAll>(this);
  }

  void Wait() noexcept final
  {
    task_.Wait();
  }

  constexpr TaskList GetTasks() noexcept final
  {
    return { &task_, &task_ + 1 };
  }

  PolyView<Components...> GetView()
  {
    return task_.view;
  }

private:
  struct DataTask : public Task
  {
    constexpr DataTask(PolyView<Components...> other_view, Update&& other_update) noexcept
      : view(other_view), update(std::forward<Update>(other_update))
    {}

    PolyView<Components...> view;
    Update update;
  };

  void UpdateAll()
  {
    for (auto mono_view : GetView())
    {
      for (auto& data : mono_view)
      {
        EntityApply<Update, Components...>(task_.update, data);
      }
    }
  }

private:
  DataTask task_;
};

} // namespace genebits::engine

#endif
