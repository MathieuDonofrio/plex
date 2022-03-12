#ifndef GENEBITS_ENGINE_ECS_PHASE_H
#define GENEBITS_ENGINE_ECS_PHASE_H

#include "genebits/engine/async/when_all.h"
#include "genebits/engine/ecs/system.h"
#include "genebits/engine/utilities/ref.h"

namespace genebits::engine
{
///
/// Container for compiled and optimized list of systems to be run.
///
/// Systems are executed in the compiled order, if a system has a dependency on a system that is executed before it,
/// it must be run in sequence, otherwise it can be run concurrently.
///
/// A system has a dependency to a system executed before it if either:
/// - Writes to the same component the dependency reads or writes to.
/// - Reads the same component that the dependency writes to.
///
/// Systems that do not share any components or that only read to shared components can be trivially parallelized.
///
class Phase
{
public:
  ///
  /// Creates an aggregate task of update tasks for every system in the phase, tasks are executed in order and
  /// automatically parallelized based on dependencies.
  ///
  /// @return Task that runs all the system tasks in the correct order.
  ///
  Task<> Run()
  {
    tasks_.Clear();
    triggers_.Clear();

    for (const auto& compiled : compiled_)
    {
      tasks_.PushBack(MakeSystemTask(compiled));
    }

    co_await WhenAll(tasks_);
  }

  ///
  /// Returns whether or not a system has a dependency. If a dependency was pruned away because of a redundancy this
  /// method will not return true.
  ///
  /// Method used mainly for debugging or testing purposes.
  ///
  /// @param system System to check.
  /// @param dependency Dependency to check.
  ///
  /// @return True if the system is dependant to he dependency.
  ///
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

  ///
  /// Returns the amount of compiled system in the phase.
  ///
  /// @return Amount of systems in phase.
  ///
  [[nodiscard]] size_t Count() const noexcept
  {
    return compiled_.size();
  }

public:
  ///
  /// Compiles a phase using a vector of ordered system groups. All system groups are first concatenated into a single
  /// vector of systems based on order of groups and order of systems within groups. Then, all dependencies between
  /// systems are computed and optimized.
  ///
  /// @tparam Groups System group variadic template.
  ///
  /// @param groups Ordered variadic template list of groups.
  ///
  /// @return Phase compiled with system groups.
  ///
  template<std::same_as<SystemGroup>... Groups>
  static Phase Compile(Ref<Groups>... groups)
  {
    Vector<Ref<SystemGroup>> vector;
    vector.Reserve(sizeof...(Groups));

    (vector.PushBack(std::move(groups)), ...);

    return Compile(vector);
  }

  ///
  /// Compiles a phase using a vector of ordered system groups. All system groups are first concatenated into a single
  /// vector of systems based on order of groups and order of systems within groups. Then, all dependencies between
  /// systems are computed and optimized.
  ///
  /// @param[in] groups List of ordered system groups to compile phase for.
  ///
  /// @return Phase compiled with system groups.
  ///
  static Phase Compile(const Vector<Ref<SystemGroup>>& groups);

public:
  ///
  /// Minimal compiled information about the system and its dependencies.
  ///
  struct CompiledSystem
  {
    SystemBase* system;
    Vector<size_t> dependencies; // As indexes
  };

private:
  ///
  /// Internal constructor.
  ///
  /// @param[in] compiled_systems Vector of compiled systems.
  ///
  Phase(Vector<CompiledSystem>&& compiled_systems) : compiled_(std::move(compiled_systems))
  {
    Reserve();
  }

  ///
  /// Creates a shared task from a compiled system. Will first wait for all its dependencies to finish, then will
  /// execute the system update.
  ///
  /// @param[in] compiled_system The compiled system to make the task for.
  ///
  /// @return Shared task that waits for its dependencies then executes system update.
  ///
  SharedTask<> MakeSystemTask(const CompiledSystem& compiled_system)
  {
    const size_t amount = compiled_system.dependencies.size();

    if (amount != 0)
    {
      WhenAllCounter counter(amount);

      for (const size_t dependency : compiled_system.dependencies)
      {
        triggers_.PushBack(MakeTriggerTask<WhenAllCounter>(tasks_[dependency]));
        triggers_.back().Start(counter);
      }

      co_await counter;
    }

    co_await compiled_system.system->Update();
  }

  ///
  /// Reserves the correct amount of memory for the runtime of this phase.
  ///
  void Reserve()
  {
    tasks_.Reserve(compiled_.size());

    size_t total_dependencies = 0;

    for (const auto& system : compiled_)
    {
      total_dependencies += system.dependencies.size();
    }

    triggers_.Reserve(total_dependencies);
  }

private:
  Vector<CompiledSystem> compiled_;
  Vector<SharedTask<>> tasks_;
  Vector<TriggerTask<void, WhenAllCounter>> triggers_;
};

} // namespace genebits::engine

#endif
