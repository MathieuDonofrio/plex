#include "genebits/engine/ecs/phase.h"

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

  FastVector<bool> ComputeAdjacencyMatrix(const FastVector<SystemBase*>& systems)
  {
    size_t count = systems.size();

    FastVector<bool> matrix;
    matrix.Resize(count * count);

    for (size_t i = 0; i < count; i++)
    {
      const auto& access1 = systems[i]->GetDataAccess();

      for (size_t j = 0; j < count; j++)
      {
        const auto& access2 = systems[j]->GetDataAccess();

        if (IsDependant(access1, access2))
        {
          //
          matrix[i * count + j] = true;
        }
      }
    }

    return matrix;
  }

  void PruneRedundant(FastVector<bool>& matrix, size_t length)
  {
    // For each system
    for (size_t i = 0; i < length; i++) // i = current system (us)
    {
      // For each system that starts execution before us.
      for (size_t j = 0; j < i; j++) // j = current dependency
      {
        // Are we dependent to that system
        if (matrix[i * length + j])
        {
          // If a system is both dependant to us and the current dependency, then the dependency is redundant since we
          // would have already waited for it.

          // For each system that starts execution after us
          for (size_t k = i + 1; k < length; k++) // k = other system
          {
            // Is the other system dependant to us
            if (matrix[k * length + i])
            {
              matrix[k * length + j] = false; // Remove redundant dependency if any (Might already be false)
            }
          }
        }
      }
    }
  }

  FastVector<SystemBase*> CombineGroups(const FastVector<Ref<SystemGroup>>& groups)
  {
    FastVector<SystemBase*> systems;

    for (const Ref<SystemGroup>& group : groups)
    {
      for (size_t i = 0; i < group->Count(); i++)
      {
        systems.PushBack(group->RawSystems()[i].Get());
      }
    }

    return systems;
  }

  FastVector<Phase::CompiledSystem> MakeCompiledSystems(
    const FastVector<SystemBase*>& systems, FastVector<bool>& matrix)
  {
    size_t count = systems.Size();

    FastVector<Phase::CompiledSystem> compiled_systems;
    compiled_systems.Resize(count);

    for (size_t i = 0; i != count; i++)
    {
      Phase::CompiledSystem compiled_system;

      compiled_system.system = systems[i];

      for (size_t j = 0; j != i; j++)
      {
        if (matrix[i * count + j]) compiled_system.dependencies.PushBack(j);
      }

      compiled_systems[i] = std::move(compiled_system);
    }

    return compiled_systems;
  }

} // namespace

Phase Phase::Compile(const FastVector<Ref<SystemGroup>>& groups)
{
  FastVector<SystemBase*> systems = CombineGroups(groups);

  FastVector<bool> matrix = ComputeAdjacencyMatrix(systems);

  PruneRedundant(matrix, systems.size());

  return { MakeCompiledSystems(systems, matrix) };
}

} // namespace genebits::engine
