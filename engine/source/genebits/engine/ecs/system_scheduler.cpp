#include "genebits/engine/ecs/system_scheduler.h"

namespace genebits::engine
{
namespace details
{
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

Task<> SystemScheduler::RunAll(Context& context)
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

void SystemScheduler::Schedule(const Ref<Stage>& stage)
{
  cache_.Add(stage);
}

SharedTask<> SystemScheduler::MakeSystemTask(const SystemGraph::CompactNode& step, Context& context)
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
