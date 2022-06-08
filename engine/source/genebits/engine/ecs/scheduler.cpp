#include "genebits/engine/ecs/scheduler.h"

namespace genebits::engine
{
namespace details
{
  SystemGraph::SystemGraph(const Vector<Ref<Stage>>& stages)
  {
    for (const auto& stage : stages)
    {
      for (const auto& system : stage->GetSystems())
      {
        nodes_.PushBack(Node { system, stage });
      }
    }

    ComputeDependencies();
    PruneDependencies();
    Compact();
  }

  void SystemGraph::ComputeDependencies()
  {
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it)
    {
      for (auto other_it = nodes_.begin(); other_it != it; ++other_it)
      {
        if (it->stage == other_it->stage && !it->stage->HasExplicitOrder(other_it->system, it->system))
        {
          // If we are from the same stage and there was no explicit order between the two systems,
          // we are free to reorder these systems however we want, therefore there is no dependency.
          continue;
        }

        if (it->system->HasDependency(*other_it->system)) it->dependencies.PushBack(&*other_it);
      }
    }
  }

  void SystemGraph::PruneDependencies()
  {
    struct Utils
    {
      static Vector<Node*> SecondaryDependencies(Node* node)
      {
        Vector<Node*> secondary_dependencies;

        for (auto dependency : node->dependencies)
        {
          ConcatDependencies(dependency, secondary_dependencies);
        }

        return secondary_dependencies;
      }

      static void ConcatDependencies(Node* node, Vector<Node*>& dependencies)
      {
        for (auto dependency : node->dependencies)
        {
          dependencies.PushBack(dependency);

          ConcatDependencies(node, dependencies);
        }
      }
    };

    for (auto& node : nodes_)
    {
      Vector<Node*> secondary_dependencies = Utils::SecondaryDependencies(&node);

      for (auto it = node.dependencies.rbegin(); it != node.dependencies.rend();)
      {
        const bool found = std::ranges::find(secondary_dependencies, *it) != secondary_dependencies.end();

        if (found) node.dependencies.UnorderedErase(it.base());
      }
    }
  }

  void SystemGraph::Compact()
  {
    for (auto& node : nodes_)
    {
      Vector<size_t> dependency_indexes;
      dependency_indexes.Reserve(node.dependencies.size());

      for (auto dependency : node.dependencies)
      {
        dependency_indexes.PushBack(dependency - nodes_.begin());
      }

      compact_nodes_.PushBack(CompactNode { node.system->GetExecutor(), dependency_indexes });
    }
  }

  SystemGraphCache::SystemGraphCache()
  {
    root_.parent = nullptr;

    root_.system_graph = MakeRef<SystemGraph>();

    Reset();
  }

  SystemGraphCache::~SystemGraphCache()
  {
    DestroyNode(&root_);
  }

  Ref<SystemGraph>& SystemGraphCache::Build()
  {
    ASSERT(current_ != nullptr, "Builder not prepared");

    auto& graph = current_->system_graph;

    if (graph) [[likely]] { return graph; }
    else // SlowPath
    {
      return BakeGraph(); // Cache the phase
    };
  }

  void SystemGraphCache::Add(Ref<Stage> stage)
  {
    Node* child = TryGet(stage);

    if (child != nullptr) [[likely]] { current_ = child; }
    else // SlowPath
    {
      NewPath(stage);
    }
  }

  Ref<SystemGraph>& SystemGraphCache::BakeGraph()
  {
    Vector<Ref<Stage>> stages; // Use circular buffer or container with O(1) insert

    Node* current = current_;

    while (current->parent != nullptr)
    {
      stages.PushBack(current->stage);

      current = current_->parent;
    }

    std::reverse(stages.begin(), stages.end());

    current_->system_graph = MakeRef<SystemGraph>(stages);

    return current_->system_graph;
  }

  void SystemGraphCache::DestroyNode(Node* node)
  {
    if (node == nullptr) return;

    for (Node* child : current_->children)
    {
      DestroyNode(child);
    }

    if (node->parent != nullptr) // Not root
    {
      delete node;
    }
  }

  SystemGraphCache::Node* SystemGraphCache::TryGet(Ref<Stage>& stage)
  {
    for (Node* child : current_->children)
    {
      if (child->stage == stage) return child;
    }

    return nullptr;
  }

  void SystemGraphCache::NewPath(Ref<Stage>& stage)
  {
    Node* node = new Node;
    node->parent = current_;
    node->stage = stage;

    current_->children.PushBack(node);

    current_ = node;
  }
} // namespace details

Task<> Scheduler::RunAll(Context& context)
{
  tasks_.Clear();
  triggers_.Clear();

  const auto& steps = cache_.Build()->GetCompactNodes();

  for (const auto& step : steps)
  {
    tasks_.PushBack(MakeSystemTask(step, context));
  }

  co_await WhenAll(tasks_);
}

void Scheduler::Schedule(const Ref<Stage>& stage)
{
  cache_.Add(stage);
}

SharedTask<> Scheduler::MakeSystemTask(const details::SystemGraph::CompactNode& step, Context& context)
{
  const size_t amount = step.dependencies.size();

  if (amount != 0)
  {
    WhenAllCounter counter(amount);

    for (const size_t dependency : step.dependencies)
    {
      triggers_.PushBack(MakeTriggerTask<WhenAllCounter>(tasks_[dependency]));
      triggers_.back().Start(counter);
    }

    co_await counter;
  }

  co_await step.executor(context);
}

} // namespace genebits::engine
