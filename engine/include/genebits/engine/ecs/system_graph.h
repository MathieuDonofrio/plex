#ifndef GENEBITS_ENGINE_ECS_SYSTEM_GRAPH_H
#define GENEBITS_ENGINE_ECS_SYSTEM_GRAPH_H

#include "genebits/engine/async/when_all.h"
#include "genebits/engine/containers/vector.h"
#include "genebits/engine/ecs/stage.h"
#include "genebits/engine/utilities/delegate.h"

namespace genebits::engine
{
class SystemGraph
{
public:
  struct CompactNode
  {
    SystemExecutor executor;
    Vector<size_t> dependencies; // As indexes
  };

  struct Node
  {
    Ref<SystemObject> system;
    Ref<Stage> stage;

    Vector<Node*> dependencies;
  };

public:
  template<std::same_as<Ref<Stage>>... Stages>
  SystemGraph(Stages&&... stages) : SystemGraph(Vector<Ref<Stage>> { std::forward<Stage>(stages)... })
  {}

  SystemGraph(const Vector<Ref<Stage>>& stages);

  // TODO incremental runtime optimization steps

  const Vector<CompactNode>& GetCompactNodes()
  {
    return compact_nodes_;
  }

private:
  void ComputeDependencies();

  void PruneDependencies();

  void Compact();

private:
  Vector<CompactNode> compact_nodes_;
  Vector<Node> nodes_;
};
} // namespace genebits::engine

#endif
