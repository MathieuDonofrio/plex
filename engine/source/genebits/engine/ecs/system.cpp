#include "genebits/engine/ecs/system.h"

namespace genebits::engine
{
namespace
{
  bool HasWriteAccess(std::vector<SystemDataAccess>& access, ComponentId componentId)
  {
    for (SystemDataAccess data : access)
    {
      if (data.id == componentId) return !data.read_only;
    }

    return false;
  }

  bool HasAccess(std::vector<SystemDataAccess>& access, ComponentId componentId)
  {
    for (SystemDataAccess data : access)
    {
      if (data.id == componentId) return true;
    }

    return false;
  }

  template<typename Iterator>
  void FindDependencies(Iterator first, Iterator last, std::set<SystemBase*>& deps, SystemDataAccess data)
  {
    for (; first != last; ++first)
    {
      if (data.read_only ? HasWriteAccess(first->access, data.id) : HasAccess(first->access, data.id))
      {
        deps.insert(first->system);
        break;
      }
    }
  }

  void FindSyncDependencies(
    std::set<SystemBase*>& solved, std::set<SystemBase*>& dependencies, std::vector<SystemBase*>& sync)
  {
    for (SystemBase* dependency : dependencies)
    {
      if (!solved.contains(dependency))
      {
        sync.push_back(dependency);
        solved.insert(dependency);
      }
    }
  }
} // namespace

void SystemGroup::ComputeDependencies()
{
  for (auto it = systems_.rbegin(); it != systems_.rend(); ++it)
  {
    it->same_frame_dependencies.clear();
    it->last_frame_dependencies.clear();

    for (SystemDataAccess data : it->access)
    {
      FindDependencies(it + 1, systems_.rend(), it->same_frame_dependencies, data);
      FindDependencies(systems_.rbegin(), it, it->last_frame_dependencies, data);
    }
  }
}

void SystemGroup::ComputeSynchronizations()
{
  std::set<SystemBase*> solved;

  for (auto it = systems_.begin(); it != systems_.end(); ++it)
  {
    it->sync.clear();

    FindSyncDependencies(solved, it->same_frame_dependencies, it->sync);
  }

  for (auto it = systems_.begin(); it != systems_.end(); ++it)
  {
    FindSyncDependencies(solved, it->last_frame_dependencies, it->sync);
  }
}

} // namespace genebits::engine
