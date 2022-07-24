#include "genebits/engine/scheduler/scheduler.h"

#include "genebits/engine/containers/deque.h"

namespace genebits::engine
{
Task<> Scheduler::RunAll(Context& context)
{
  tasks_.clear();
  triggers_.clear();

  const auto& steps = cache_.Build();

  for (const auto& step : steps)
  {
    tasks_.push_back(MakeSystemTask(step, context));
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
      triggers_.push_back(MakeTriggerTask<WhenAllCounter>(tasks_[dependency]));
      triggers_.back().Start(counter);
    }

    co_await counter;
  }

  co_await step.system->operator()(context);
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
    stages.push_back(current->stage);

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

  for (Node* child : node->children)
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

  current_->children.push_back(node);

  current_ = node;
}

//
// Scheduler Algorithms
//

struct IntermediateStep
{
  SystemObject* system;
  Vector<size_t> dependants;
};

Vector<IntermediateStep> ComputeDependencyGraph(const Vector<Stage*>& stages)
{
  Vector<IntermediateStep> steps;

  size_t step_index = 0;

  for (auto stage_it = stages.begin(); stage_it != stages.end(); ++stage_it)
  {
    const auto& stage = **stage_it;

    const auto& systems = stage.GetSystemObjects();

    for (auto system_it = systems.begin(); system_it != systems.end(); ++system_it)
    {
      const auto& system = **system_it;

      size_t other_step_index = 0;

      // Find all dependencies in earlier stages

      for (auto other_stage_it = stages.begin(); other_stage_it != stage_it; ++other_stage_it)
      {
        const auto& other_systems = (*other_stage_it)->GetSystemObjects();

        for (auto other_system_it = other_systems.begin(); other_system_it != other_systems.end(); ++other_system_it)
        {
          const auto& other_system = **other_system_it;

          if (system.HasDependency(other_system))
          {
            steps[other_step_index].dependants.push_back(step_index);
          }

          other_step_index++;
        }
      }

      // Find all explicit dependencies in same stage

      for (auto other_system_it = systems.begin(); other_system_it != systems.end(); ++other_system_it)
      {
        const auto& other_system = **other_system_it;

        if (stage.HasExplicitOrder(other_system, system) && system.HasDependency(other_system))
        {
          steps[other_step_index].dependants.push_back(step_index);
        }

        other_step_index++;
      }

      steps.push_back({ system_it->get() });

      step_index++;
    }
  }

  return steps;
}

Vector<size_t> TopologicalSort(const Vector<IntermediateStep>& steps)
{
  // Kahn's algorithm

  Vector<size_t> in_degree;
  in_degree.resize(steps.size());

  for (const auto& step : steps)
  {
    for (auto dependant : step.dependants)
    {
      in_degree[dependant]++;
    }
  }

  Deque<size_t> queue;
  queue.reserve(steps.size());

  for (size_t i = 0; i < steps.size(); i++)
  {
    if (in_degree[i] == 0)
    {
      queue.push_back(i);
    }
  }

  [[maybe_unused]] size_t visited_counter = 0;

  Vector<size_t> order;
  order.reserve(steps.size());

  while (!queue.empty())
  {
    size_t index = queue.front();
    queue.pop_front();
    order.push_back(index);

    for (auto dependant : steps[index].dependants)
    {
      if (--in_degree[dependant] == 0)
      {
        queue.push_back(dependant);
      }
    }

    visited_counter++;
  }

  ASSERT(visited_counter == steps.size(), "Cycle detected");

  return order;
}

Vector<Scheduler::Step> ComputeExecutionGraph(
  const Vector<IntermediateStep>& intermediate_steps, const Vector<size_t>& order)
{
  Vector<Scheduler::Step> steps;
  steps.reserve(intermediate_steps.size());

  // Naive transitive reduction.
  // This allows us to have as little dependencies as possible, giving a little less work to the scheduler every run.
  // Probably a lot of potential to optimize if ever this becomes a problem. It may even be possible to remove this
  // if it doesn't provide any significant performance improvements.
  struct TransitiveReduction
  {
    static bool IsRedundant(
      const Vector<Scheduler::Step>& steps, const Vector<size_t>& dependencies, size_t new_dependency)
    {
      for (auto dependency : dependencies)
      {
        if (dependency == new_dependency) return true;
        else if (IsRedundant(steps, steps[dependency].dependencies, new_dependency))
          return true;
      }
      return false;
    }
  };

  for (size_t i = 0; i < order.size(); i++)
  {
    const auto& intermediate_step = intermediate_steps[order[i]];

    Vector<size_t> dependencies;

    for (size_t j = 0; j != i; j++)
    {
      const auto& other_intermediate_step = intermediate_steps[order[j]];

      if (intermediate_step.system->HasDependency(*other_intermediate_step.system))
      {
        if (!TransitiveReduction::IsRedundant(steps, dependencies, j))
        {
          dependencies.push_back(j);
        }
      }
    }

    steps.push_back({ intermediate_step.system, dependencies });
  }

  return steps;
}

Vector<Scheduler::Step> ComputeSchedulerData(const Vector<Stage*>& stages)
{
  auto intermediate_steps = ComputeDependencyGraph(stages);
  auto order = TopologicalSort(intermediate_steps);
  auto steps = ComputeExecutionGraph(intermediate_steps, order);

  return steps;
}
} // namespace genebits::engine
