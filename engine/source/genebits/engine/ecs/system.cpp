#include "genebits/engine/ecs/system.h"

namespace genebits::engine
{
namespace
{
  struct CompileSystemInfo
  {
    SystemBase* system;

    SystemDataAccessList access;

    std::set<SystemBase*> pre_deps;
    std::set<SystemBase*> post_deps;

    FastVector<SystemBase*> pre_sync;
    FastVector<SystemBase*> post_sync;
  };

  struct CompileInfo
  {
    FastVector<CompileSystemInfo> systems;

    size_t total_syncs;
  };

  CompileInfo CreateCompileInfo(SystemBase** systems, size_t count)
  {
    CompileInfo info;

    info.systems.Reserve(count);

    for (size_t i = 0; i != count; i++)
    {
      SystemBase* system = systems[i];

      CompileSystemInfo system_info;

      system_info.system = system;
      system_info.access = system->GetDataAccess();

      info.systems.PushBack(std::move(system_info));
    }

    info.total_syncs = 0;

    return info;
  }

  bool HasWriteAccess(const SystemDataAccessList& access, ComponentId componentId)
  {
    for (SystemDataAccess data : access)
    {
      if (data.id == componentId) return !data.read_only;
    }

    return false;
  }

  bool HasAccess(const SystemDataAccessList& access, ComponentId componentId)
  {
    for (SystemDataAccess data : access)
    {
      if (data.id == componentId) return true;
    }

    return false;
  }

  template<typename Iterator>
  void AddDependencies(Iterator first, Iterator last, std::set<SystemBase*>& deps, SystemDataAccess data)
  {
    for (; first != last; ++first)
    {
      const auto& access = first->access;

      const bool dependant = data.read_only ? HasWriteAccess(access, data.id) : HasAccess(access, data.id);

      if (dependant) // Add to list of dependencies
      {
        deps.insert(first->system);
        break;
      }
    }
  }

  void AddSynchronizations(
    std::set<SystemBase*>& solved, std::set<SystemBase*>& dependencies, FastVector<SystemBase*>& sync)
  {
    for (SystemBase* dependency : dependencies)
    {
      if (!solved.contains(dependency)) // If not already added
      {
        sync.PushBack(dependency);

        solved.insert(dependency);
      }
    }
  }

  void ComputeDependencies(CompileInfo& info)
  {
    for (auto it = info.systems.rbegin(); it != info.systems.rend(); ++it)
    {
      for (const SystemDataAccess& data : it->access)
      {
        // Pre dependencies: All possible dependencies from systems that run before in same run
        AddDependencies(it + 1, info.systems.rend(), it->pre_deps, data);

        // Post dependencies: All possible dependencies from systems that run after from last run
        AddDependencies(info.systems.rbegin(), it + 1, it->post_deps, data);
      }
    }
  }

  void PruneDependencies(CompileInfo& info)
  {
    std::set<SystemBase*> solved;

    for (CompileSystemInfo& system_info : info.systems)
    {
      AddSynchronizations(solved, system_info.pre_deps, system_info.pre_sync);

      info.total_syncs += system_info.pre_sync.Size();
    }

    for (CompileSystemInfo& system_info : info.systems)
    {
      AddSynchronizations(solved, system_info.post_deps, system_info.post_sync);

      info.total_syncs += system_info.post_sync.Size();
    }
  }
} // namespace

Phase Phase::Compile(SystemBase** systems, size_t dependencies, size_t total)
{
  // Compile

  CompileInfo info = CreateCompileInfo(systems, total);

  ComputeDependencies(info);

  PruneDependencies(info);

  // Bake results into phase

  const size_t phase_systems_count = total - dependencies;

  void* memory = std::malloc(sizeof(Phase::CompiledSystem) * phase_systems_count + sizeof(void*) * info.total_syncs);

  auto compiled_system_ptr = static_cast<Phase::CompiledSystem*>(memory);
  auto sync_ptr = reinterpret_cast<SystemBase**>(compiled_system_ptr + phase_systems_count);

  size_t current_sync = 0;

  for (size_t i = 0; i != phase_systems_count; i++)
  {
    const CompileSystemInfo& system_info = info.systems[i + dependencies];
    Phase::CompiledSystem* compiled_system = compiled_system_ptr + i;

    compiled_system->system = system_info.system;

    auto& pre_sync = system_info.pre_sync;
    auto& post_sync = system_info.post_sync;

    const size_t sync_count = pre_sync.Size() + post_sync.Size();

    if (sync_count)
    {
      compiled_system->sync.begin = sync_ptr + current_sync;
      compiled_system->sync.end = compiled_system->sync.begin + sync_count;

      std::uninitialized_copy(pre_sync.begin(), pre_sync.end(), compiled_system->sync.begin);
      std::uninitialized_copy(post_sync.begin(), post_sync.end(), compiled_system->sync.begin + pre_sync.Size());
    }
    else
    {
      compiled_system->sync.begin = nullptr;
      compiled_system->sync.end = nullptr;
    }
  }

  return { compiled_system_ptr, compiled_system_ptr + phase_systems_count };
}

Phase Phase::Compile(SystemGroup& group, std::initializer_list<SystemGroup*> dependencies)
{
  size_t dependencies_count = 0;

  for (SystemGroup* dependency : dependencies)
  {
    dependencies_count += dependency->Count();
  }

  const size_t total_count = dependencies_count + group.Count();

  auto systems = static_cast<SystemBase**>(std::malloc(sizeof(void*) * total_count));

  Phase phase = Compile(systems, dependencies_count, total_count);

  std::free(systems);

  return std::move(phase);
}

Phase Phase::Compile(SystemGroup& group)
{
  return Compile(group.RawSystems(), 0, group.Count());
}

void Phase::Destroy()
{
  if (begin_) std::free(begin_);

  begin_ = nullptr;
  end_ = nullptr;
}

} // namespace genebits::engine
