#ifndef GENEBITS_ENGINE_ECS_SYSTEM_SCHEDULER_H
#define GENEBITS_ENGINE_ECS_SYSTEM_SCHEDULER_H

#include "genebits/engine/ecs/system_graph.h"
#include <genebits/engine/async/when_all.h>

namespace genebits::engine
{
namespace details
{
  ///
  /// Builder pattern class that facilitates the efficient creation of system graphs.
  ///
  /// Made to be reused multiple times by caching unique phases to avoid the overhead of making the graph.
  ///
  class SystemGraphCache
  {
  public:
    ///
    /// Default constructor.
    ///
    SystemGraphCache();

    ///
    /// Destructor
    ///
    ~SystemGraphCache();

    ///
    /// Returns the phase built for all builder operations since last reset.
    ///
    /// Unique phases are kept in a cache.
    ///
    /// @return The built phase.
    ///
    Ref<SystemGraph>& Build();

    ///
    /// Adds a stage to the building process of the graph.
    ///
    /// @param[in] stage Stage to add to graph.
    ///
    void Add(Ref<Stage> stage);

    ///
    /// Resets the builder to be ready for the construction of a new phase.
    ///
    void Reset() noexcept
    {
      current_ = &root_;
    }

  private:
    ///
    /// Node used to store information about a step used to build a phase.
    ///
    struct Node
    {
      Node* parent;
      Vector<Node*> children;

      Ref<SystemGraph> system_graph;
      Ref<Stage> stage;
    };

    ///
    /// Returns a newly created graph for the current node.
    ///
    /// Sets the cache of the node to the new graph.
    ///
    /// @return Graph for current node.
    ///
    Ref<SystemGraph>& BakeGraph();

    ///
    /// Destroys a node and all its children recursively.
    ///
    /// Does not destroy the node if it is root.
    ///
    /// @param[in] Node Node to destroy subtree for.
    ///
    void DestroyNode(Node* node);

    ///
    /// Returns the cached node for the step of the build process, if no node could be found, returns nullptr.
    ///
    /// Tries to obtain a node for the stage based of current build steps. If there is no node for the stage, it
    /// means that this sequence of build steps is unique.
    ///
    /// @param[in] stage Stage to get next node for.
    ///
    /// @return Node for next step or nullptr if step is unique.
    ///
    Node* TryGet(Ref<Stage>& stage);

    ///
    /// Creates an caches a new node representing the new build sequence for the stage.
    ///
    /// @param[in] stage Stage to create new path for.
    ///
    void NewPath(Ref<Stage>& stage);

  private:
    Node root_;
    Node* current_;
  };
} // namespace details

class SystemScheduler
{
public:
  ///
  /// Runs all the scheduled stages.
  ///
  /// Creates an aggregate task of update tasks for every system in the phase with proper order and parallelism
  /// potential.
  ///
  /// @return Task that runs all the system tasks in the correct order.
  ///
  Task<> RunAll(Registry& registry);

  ///
  /// Schedules the stage to be run.
  ///
  /// The stage will be run in the order that it was scheduled.
  ///
  /// A stage implicitly acts as a barrier for the system order of execution and parallelism. A system cannot be
  /// executed before a other system from a different stage that shares a common dependency (Example: Writes to the same
  /// component/resource). Systems from the same stage can be reordered freely unless order was made explicit and there
  /// is a common dependency.
  ///
  /// @param[in] stage Stage to schedule.
  ///
  void Schedule(const Ref<Stage>& stage);

private:
  ///
  /// Creates a shared task from a step. Will first wait for all its dependencies to finish, then will
  /// execute the system update.
  ///
  /// @param[in] step The step to make the task for.
  ///
  /// @return Shared task that waits for its dependencies then executes system update.
  ///
  SharedTask<> MakeSystemTask(const SystemGraph::CompactNode& step, Registry& registry);

private:
  Vector<SharedTask<>> tasks_;
  Vector<TriggerTask<void, WhenAllCounter>> triggers_;

  details::SystemGraphCache cache_;
};

} // namespace genebits::engine

#endif
