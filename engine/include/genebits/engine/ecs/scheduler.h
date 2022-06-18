#ifndef GENEBITS_ENGINE_ECS_SCHEDULER_H
#define GENEBITS_ENGINE_ECS_SCHEDULER_H

#include "genebits/engine/async/shared_task.h"
#include "genebits/engine/async/when_all.h"
#include "genebits/engine/ecs/stage.h"
#include "genebits/engine/ecs/system.h"
#include "genebits/engine/utilities/ref.h"

namespace genebits::engine
{
///
/// Scheduler class responsible for running systems.
///
/// Will attempt to parallelize the execution of systems as much as possible.
///
class Scheduler
{
public:
  ///
  /// Runs all the scheduled stages.
  ///
  /// Creates an aggregate task of update tasks for every system in the phase with proper order and parallelism
  /// potential.
  ///
  /// @param[in] context The context to run systems with.
  ///
  /// @return Task that runs all the system tasks in the correct order.
  ///
  Task<> RunAll(Context& context);

  ///
  /// Schedules the stage to be run.
  ///
  /// The stage will be run in the order that it was scheduled.
  ///
  /// A stage implicitly acts as a barrier for the system order of execution and parallelism. A system cannot be
  /// executed before a other system from a different stage that shares a common dependency (Example: Writes to the same
  /// component). Systems from the same stage can be reordered freely unless order was made explicit and there
  /// is a common dependency.
  ///
  /// @param[in] stage Stage to schedule.
  ///
  template<typename StageType>
  void Schedule()
  {
    cache_.Add(&stages_.Assure<StageType>());
  }

  ///
  /// Adds a system to the scheduler for the given stage.
  ///
  /// @tparam StageType The stage to add system to.
  /// @tparam SystemType Type of the system to add.
  ///
  /// @param[in] system The system to add.
  ///
  /// @return Builder-pattern style interface for ordering the added system.
  ///
  template<typename StageType, typename SystemType>
  Stage::SystemOrder AddSystem(SystemType system)
  {
    return stages_.Assure<StageType>().AddSystem(system);
  }

public:
  ///
  /// A scheduler step.
  ///
  struct Step
  {
    SystemExecutor executor;
    Vector<size_t> dependencies; // As indexes
  };

private:
  ///
  /// Creates a shared task from a step. Will first wait for all its dependencies to finish, then will
  /// execute the system update.
  ///
  /// @param[in] step Information about the system to execute.
  /// @param[in] context The context to run systems with.
  ///
  /// @return Shared task that waits for its dependencies then executes system update.
  ///
  SharedTask<> MakeSystemTask(const Step& step, Context& context);

private:
  ///
  /// Builder pattern class that facilitates the efficient creation of scheduler data.
  ///
  /// Records every unique sequence of stages and builds the scheduler steps for each sequence only once by caching the
  /// results.
  ///
  class Cache
  {
  public:
    Cache();

    ~Cache();

    ///
    /// Returns the scheduler steps for the current sequence of added stages.
    ///
    /// Will first attempt to retrieve the cached steps, if none, the steps will be built.
    ///
    /// @return Scheduler steps for the current sequence of added stages.
    ///
    const Vector<Scheduler::Step>& Build()
    {
      ASSERT(current_ != nullptr, "Builder not prepared");

      if (current_->baked) [[likely]]
      {
        return current_->steps;
      }
      else // SlowPath
      {
        return Bake(); // Cache the phase
      };
    }

    ///
    /// Add a stage to the sequence of stages.
    ///
    /// @param[in] stage The stage to add.
    ///
    void Add(Stage* stage)
    {
      Node* child = TryGet(stage);

      if (child != nullptr) [[likely]]
      {
        current_ = child;
      }
      else // SlowPath
      {
        NewPath(stage);
      }
    }

  private:
    struct Node
    {
      using IsTriviallyRelocatable = std::true_type;

      Node* parent;
      Vector<Node*> children;

      Stage* stage;

      bool baked;

      Vector<Scheduler::Step> steps;
    };

    const Vector<Scheduler::Step>& Bake();

    void DestroyNode(Node* node);

    void NewPath(Stage* stage);

    Node* TryGet(Stage* stage)
    {
      for (Node* child : current_->children)
      {
        if (child->stage == stage) return child;
      }

      return nullptr;
    }

  private:
    Node root_;
    Node* current_;
  };

  Vector<SharedTask<>> tasks_;
  Vector<TriggerTask<void, WhenAllCounter>> triggers_;

  TypeMap<Stage> stages_;

  Cache cache_;
};

} // namespace genebits::engine

#endif
