#include "genebits/engine/ecs/system.h"

namespace genebits::engine
{
namespace
{
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

  bool IsDependant(const SystemDataAccessList& access1, const SystemDataAccessList& access2)
  {
    for (const SystemDataAccess& data : access1)
    {
      if (data.read_only)
      {
        if (HasWriteAccess(access2, data.id)) return true;
      }
      else if (HasAccess(access2, data.id))
      {
        return true;
      }
    }

    return false;
  }

  FastVector<bool> ComputeAdjacencyMatrix(SystemBase** systems, size_t length)
  {
    // TODO optimize

    FastVector<bool> matrix;
    matrix.Resize(length * length);

    for (size_t i = 0; i < length; i++)
    {
      const auto& access1 = systems[i]->GetDataAccess();

      for (size_t j = 0; j < length; j++)
      {
        const auto& access2 = systems[j]->GetDataAccess();

        if (IsDependant(access1, access2)) matrix[i * length + j] = true;
      }
    }

    return matrix;
  }

  void PruneAdjacencyMatrix(FastVector<bool>& matrix, size_t length)
  {
    // TODO Optimize if possible

    for (size_t i = 0; i < length; i++)
    {
      for (size_t j = 0; j < i; j++)
      {
        if (matrix[i * length + j])
        {
          for (size_t k = 0; k < length; k++)
          {
            matrix[k * length + j] = k == i;
          }
        }
      }

      for (size_t j = i + 1; j < length; j++)
      {
        if (matrix[i * length + j])
        {
          for (size_t k = i + 1; k <= j; k++)
          {
            matrix[k * length + j] = false;
          }
        }
      }
    }
  }
} // namespace

Phase Phase::Compile(SystemBase** systems, size_t dependencies, size_t total)
{
  // Compile

  auto matrix = ComputeAdjacencyMatrix(systems, total);

  PruneAdjacencyMatrix(matrix, total);

  // Bake results into phase

  const size_t phase_systems_count = total - dependencies;

  FastVector<CompiledSystem> compiled_systems;

  compiled_systems.Resize(phase_systems_count);

  for (size_t i = 0; i != phase_systems_count; i++)
  {
    CompiledSystem compiled_system;

    compiled_system.system = systems[i];

    for (size_t j = 0; j != total; j++)
    {
      if (matrix[i * total + j]) compiled_system.sync.PushBack(systems[j]);

      // TODO optimize memory (shrink to fit)
    }

    compiled_systems[i] = std::move(compiled_system);
  }

  return { std::move(compiled_systems) };

  /*
    CompiledSystem* compiled_systems = std::malloc(sizeof(Phase::CompiledSystem) * phase_systems_count);

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
    */
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

} // namespace genebits::engine
