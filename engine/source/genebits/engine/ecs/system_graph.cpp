#include "genebits/engine/ecs/system_graph.h"

namespace genebits::engine
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
} // namespace genebits::engine
