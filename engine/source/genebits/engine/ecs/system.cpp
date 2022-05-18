#include "genebits/engine/ecs/system.h"

namespace genebits::engine
{
namespace
{
  bool HasWriteDependency(const Vector<QueryDataAccess>& access, const QueryDataAccess& data) noexcept
  {
    for (const QueryDataAccess& other : access)
    {
      if (other.category == data.category && other.name == data.name) return !other.read_only;
    }

    return false;
  }

  bool HasReadDependency(const Vector<QueryDataAccess>& access, const QueryDataAccess& data) noexcept
  {
    for (const QueryDataAccess& other : access)
    {
      if (other.category == data.category && other.name == data.name) return true;
    }

    return false;
  }
} // namespace

[[nodiscard]] bool SystemObject::HasDependency(const SystemObject& system) const noexcept
{
  for (const QueryDataAccess& data : info_->data_access)
  {
    if (data.thread_safe) continue;

    if (data.read_only)
    {
      if (HasWriteDependency(system.info_->data_access, data)) return true;
    }
    else if (HasReadDependency(system.info_->data_access, data))
    {
      return true;
    }
  }

  return false;
}

} // namespace genebits::engine