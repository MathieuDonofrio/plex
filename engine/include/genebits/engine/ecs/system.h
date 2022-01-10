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

template<typename... Components>
class System;

class SystemGroup;

class SystemBase
{
public:
  virtual ~SystemBase() = default;

  virtual void OnUpdate(JobHandle dependencies) = 0;

  virtual std::vector<SystemDataAccess> GetDataAccess() = 0;

  constexpr void SetJobHandle(JobHandle handle) noexcept
  {
    handle_ = handle;
  }

  [[nodiscard]] constexpr JobHandle& GetJobHandle() noexcept
  {
    return handle_;
  }

  [[nodiscard]] constexpr JobScheduler& GetScheduler() noexcept
  {
    return *scheduler_;
  }

protected:
  friend SystemGroup;

  template<typename... Components>
  friend class System;

private:
  Registry* registry_;
  JobScheduler* scheduler_;

  JobHandle handle_;
};

class SystemGroup
{
public:
  SystemGroup(JobScheduler* job_scheduler, Registry* registry) : job_scheduler_(job_scheduler), registry_(registry) {}

  void Run()
  {
    for (SystemInfo& info : systems_)
    {
      JobHandle dependencies = CombineDependencies(info.sync);

      info.system->OnUpdate(dependencies);
    }
  }

  void ForceComplete()
  {
    for (auto it = systems_.rbegin(); it != systems_.rend(); ++it)
    {

      job_scheduler_->Complete(it->system->GetJobHandle());
    }
  }

  void Compile()
  {
    ComputeDependencies();

    ComputeSynchronizations();
  }

  void AddSystem(SystemBase* system)
  {
    SystemInfo info;

    info.system = system;
    info.access = system->GetDataAccess();

    InitializeSystem(system);

    systems_.push_back(std::move(info));
  }

  [[nodiscard]] constexpr size_t Count() noexcept
  {
    return systems_.size();
  }

private:
  struct SystemInfo
  {
    SystemBase* system;

    std::vector<SystemDataAccess> access;

    std::set<SystemBase*> same_frame_dependencies;
    std::set<SystemBase*> last_frame_dependencies;

    std::vector<SystemBase*> sync;
  };

  void InitializeSystem(SystemBase* system)
  {
    system->registry_ = registry_;
    system->scheduler_ = job_scheduler_;
  }

  JobHandle CombineDependencies(const std::vector<SystemBase*>& info);

  void ComputeDependencies();

  void ComputeSynchronizations();

private:
  JobScheduler* job_scheduler_;
  Registry* registry_;

  std::vector<SystemInfo> systems_;
};

template<typename... Components>
class System : public SystemBase
{
public:
  std::vector<SystemDataAccess> GetDataAccess() final
  {
    std::vector<SystemDataAccess> access;

    (access.push_back({ GetComponentId<std::remove_cvref_t<Components>>(), std::is_const_v<Components> }), ...);

    return access;
  }

  JobHandle ScheduleDefered(JobBase* job, JobHandle dependencies, void (*deleter)(JobBase*))
  {
    JobHandle handle = GetScheduler().Schedule(job, dependencies);

    scheduled_job_.Reset(job, deleter); // Must be called here because dependencies must be finished
    SetJobHandle(handle);

    return handle;
  }

  JobHandle ScheduleDefered(JobBase* job, JobHandle dependencies)
  {
    JobHandle handle = GetScheduler().Schedule(job, dependencies);

    scheduled_job_.Reset(job); // Must be called here because dependencies must be finished
    SetJobHandle(handle);

    return handle;
  }

  JobHandle ScheduleDefered(JobBase* job, void (*deleter)(JobBase*))
  {
    scheduled_job_.Reset(job, deleter);

    JobHandle handle = GetScheduler().Schedule(job);
    SetJobHandle(handle);

    return handle;
  }

  JobHandle ScheduleDefered(JobBase* job)
  {
    scheduled_job_.Reset(job);

    JobHandle handle = GetScheduler().Schedule(job);
    SetJobHandle(handle);

    return handle;
  }

  PolyView<Components...> GetView()
  {
    return registry_->template View<Components...>();
  }

private:
  using JobPtr = ErasedPtr<JobBase>;

  JobPtr scheduled_job_;
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
