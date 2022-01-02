#ifndef GENEBITS_ENGINE_ECS_SYSTEM_H
#define GENEBITS_ENGINE_ECS_SYSTEM_H

#include "genebits/engine/debug/assertion.h"
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

class SystemBase
{
public:
  virtual void OnUpdate() = 0;

  virtual std::vector<SystemDataAccess> GetDataAccess() = 0;

  virtual JobHandle GetJobHandle() = 0;
};

class SystemGroup
{
public:
  SystemGroup() : systems_(false) {}

  void Run()
  {
    if (modified_) Compile();

    for (const SystemInfo& info : systems_)
    {
      for (SystemBase* dependency : info.sync)
      {
        JobHandle handle = dependency->GetJobHandle();

        if (handle) handle.Complete();
      }

      info.system->OnUpdate();
    }
  }

  void Compile()
  {
    UpdateDependencies();

    UpdateSync();

    modified_ = false;
  }

  void AddSystem(SystemBase* system)
  {
    SystemInfo info;

    info.system = system;
    info.access = system->GetDataAccess();

    systems_.push_back(std::move(info));

    modified_ = true;
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

    std::set<SystemBase*> dependencies;
    std::vector<SystemBase*> sync;
  };

  void UpdateSync();

  void UpdateDependencies();

private:
  std::vector<SystemInfo> systems_;

  bool modified_;
};

template<typename Entity, typename... Components>
class System : public SystemBase
{
public:
  std::vector<SystemDataAccess> GetDataAccess() final
  {
    std::vector<SystemDataAccess> access;

    (access.push_back({ GetComponentId<std::remove_cvref_t<Components>>(), std::is_const_v<Components> }), ...);

    return access;
  }

  JobHandle GetJobHandle() final
  {
    return job_handle_;
  }

  void MarkSystemJobHandle(JobHandle handle) noexcept
  {
    job_handle_ = handle;
  }

  PolyView<Entity, Components...> GetView()
  {
    return registry_->View();
  }

  JobScheduler& GetScheduler() noexcept
  {
    return *scheduler_;
  }

private:
  // TODO Initialization
  JobScheduler* scheduler_;
  Registry<Entity>* registry_;

  JobHandle job_handle_;
};

// JOBS

template<typename Update, typename Entity, typename... Components>
requires EntitiesFunction<Update, Entity, Components...>
class EntitiesJob : public JobBase
{
public:
  constexpr EntitiesJob(Update update, PolyView<Entity, Components...> view) noexcept : task_(view, std::move(update))
  {
    task_.Executor().template Bind<EntitiesJob, &EntitiesJob::UpdateAll>(this);
  }

  constexpr TaskList GetTasks() noexcept override
  {
    return { &task_, &task_ + 1 };
  }

  PolyView<Entity, Components...> GetView()
  {
    return task_.view;
  }

private:
  struct DataTask : public Task
  {
    constexpr DataTask(PolyView<Entity, Components...> other_view, Update&& other_update) noexcept
      : view(other_view), update(std::forward<Update>(other_update))
    {}

    PolyView<Entity, Components...> view;
    Update update;
  };

  void UpdateAll()
  {
    for (auto mono_view : GetView())
    {
      for (auto& data : mono_view)
      {
        EntityApply<Update, Entity, Components...>(task_.update, data);
      }
    }
  }

private:
  DataTask task_;
};

} // namespace genebits::engine

#endif
