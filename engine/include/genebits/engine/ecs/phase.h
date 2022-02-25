#ifndef GENEBITS_ENGINE_ECS_PHASE_H
#define GENEBITS_ENGINE_ECS_PHASE_H

#include <numeric>

#include "genebits/engine/ecs/system.h"
#include "genebits/engine/parallel/when_all.h"
#include "genebits/engine/util/ref.h"

namespace genebits::engine
{
class Phase
{
public:
  Task<> Run()
  {
    Prepare();

    co_await WhenAll(tasks_);
  }

  // Used for debugging
  bool CheckDependency(SystemBase* system, SystemBase* dependency)
  {
    for (auto system_it = compiled_.begin(); system_it != compiled_.end(); ++system_it)
    {
      if (system_it->system == system)
      {
        for (auto it = system_it->dependencies.begin(); it != system_it->dependencies.end(); ++it)
        {
          if (compiled_[*it].system == dependency) return true;
        }

        break;
      }
    }

    return false;
  }

  [[nodiscard]] size_t Count() const noexcept
  {
    return compiled_.Size();
  }

  template<std::same_as<SystemGroup>... Group>
  static Phase Compile(Ref<Group>... groups)
  {
    FastVector<Ref<SystemGroup>> vector;
    vector.Reserve(sizeof...(Group));

    (vector.PushBack(std::move(groups)), ...);

    return Compile(vector);
  }

  static Phase Compile(const FastVector<Ref<SystemGroup>>& groups);

public:
  struct CompiledSystem
  {
    SystemBase* system;
    FastVector<size_t> dependencies;
  };

private:
  Phase(FastVector<CompiledSystem>&& compiled_systems) : compiled_(std::move(compiled_systems))
  {
    Reserve();
  }

  SharedTask<> MakeSystemTask(const CompiledSystem& compiled_system)
  {
    const size_t amount = compiled_system.dependencies.size();

    WhenAllCounter counter(amount);

    for (const size_t dependency : compiled_system.dependencies)
    {
      triggers_.PushBack(MakeTriggerTask<WhenAllCounter>(tasks_[dependency]));
      triggers_.back().Start(counter);
    }

    co_await counter;

    co_await compiled_system.system->Update();
  }

  void Prepare()
  {
    tasks_.Clear();
    triggers_.Clear();

    for (const auto& compiled : compiled_)
    {
      tasks_.PushBack(MakeSystemTask(compiled));
    }
  }

  void Reserve()
  {
    tasks_.Reserve(compiled_.Size());

    size_t total_dependencies = 0;

    for (const auto& system : compiled_)
    {
      total_dependencies += system.dependencies.size();
    }

    triggers_.Reserve(total_dependencies);
  }

private:
  FastVector<CompiledSystem> compiled_;
  FastVector<SharedTask<>> tasks_;
  FastVector<TriggerTask<void, WhenAllCounter>> triggers_;
};

} // namespace genebits::engine

#endif
