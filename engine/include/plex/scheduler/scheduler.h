#ifndef PLEX_SCHEDULER_SCHEDULER_H
#define PLEX_SCHEDULER_SCHEDULER_H

#include "plex/async/shared_task.h"
#include "plex/async/when_all.h"
#include "plex/scheduler/stage.h"
#include "plex/system/system.h"
#include "plex/utilities/ref.h"

namespace plex
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
  template<typename StageType, System SystemType>
  Stage::SystemOrder AddSystem(SystemType* system)
  {
    return stages_.Assure<StageType>().AddSystem(system);
  }

public:
  ///
  /// A scheduler step.
  ///
  struct Step
  {
    SystemObject* system;
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

      const auto& steps = current_->baked ? current_->steps : Bake();

      current_ = &root_;

      return steps;
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
      else
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

    ///
    /// Creates the scheduler steps and caches them.
    ///
    /// @return The scheduler steps.
    ///
    const Vector<Scheduler::Step>& Bake();

    ///
    /// Recursively destroys the node and its children.
    ///
    /// @param[in] node Node to destroy.
    ///
    void DestroyNode(Node* node);

    ///
    /// Forks the sequence of steps and creates a new path with the given stage.
    ///
    /// @param[in] stage Stage to create new path with.
    ///
    void NewPath(Stage* stage);

    ///
    /// Attempts to find an existing path for the stage, returns nullptr if no path was found.
    ///
    /// @param[in] stage Stage to attempt to find path for.
    ///
    /// @return The node for the path, or nullptr if no path was found.
    ///
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

///
/// Using the stages in the order provided, computes the scheduler step data.
///
/// Steps are ordered optimally for parallelism.
///
/// @warning This is a very expensive computation and results should be cached.
///
/// @param[in] stages
///
/// @return Scheduler steps.
///
NO_INLINE Vector<Scheduler::Step> ComputeSchedulerData(const Vector<Stage*>& stages);

} // namespace plex

#endif
