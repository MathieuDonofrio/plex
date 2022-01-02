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
} // namespace

void SystemGroup::UpdateSync()
{
  std::set<SystemBase*> solved;

  for (auto it = systems_.begin(); it != systems_.end(); ++it)
  {
    it->sync.clear();

    for (SystemBase* dependency : it->dependencies)
    {
      if (!solved.contains(dependency))
      {
        it->sync.push_back(dependency);
        solved.insert(dependency);
      }
    }
  }
}

void SystemGroup::UpdateDependencies()
{
  for (auto it = systems_.rbegin(); it != systems_.rend(); ++it)
  {
    it->dependencies.clear();

    for (SystemDataAccess data : it->access)
    {
      FindDependencies(it + 1, systems_.rend(), it->dependencies, data);
      FindDependencies(systems_.rbegin(), it, it->dependencies, data);
    }
  }
}

} // namespace genebits::engine
