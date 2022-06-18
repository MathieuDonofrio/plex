#include "genebits/engine/ecs/scheduler.h"
#include "genebits/engine/ecs/scheduler/algorithm.h"

namespace genebits::engine
{
Task<> Scheduler::RunAll(Context& context)
{
  tasks_.Clear();
  triggers_.Clear();

  const auto& steps = cache_.Build();

  for (const auto& step : steps)
  {
    tasks_.PushBack(MakeSystemTask(step, context));
  }

  co_await WhenAll(tasks_);
}

SharedTask<> Scheduler::MakeSystemTask(const Step& step, Context& context)
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

Scheduler::Cache::Cache()
{
  root_.parent = nullptr;
  current_ = &root_;
}

Scheduler::Cache::~Cache()
{
  DestroyNode(&root_);
}

const Vector<Scheduler::Step>& Scheduler::Cache::Bake()
{
  Vector<Stage*> stages;

  Node* current = current_;

  while (current->parent != nullptr)
  {
    stages.PushBack(current->stage);

    current = current->parent;
  }

  std::reverse(stages.begin(), stages.end()); // TODO Optimization: Try to avoid this

  current_->steps = ComputeSchedulerData(stages);
  current_->baked = true;

  return current_->steps;
}

void Scheduler::Cache::DestroyNode(Node* node)
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

void Scheduler::Cache::NewPath(Stage* stage)
{
  Node* node = new Node;
  node->parent = current_;
  node->stage = stage;
  node->baked = false;

  current_->children.PushBack(node);

  current_ = node;
}
} // namespace genebits::engine
