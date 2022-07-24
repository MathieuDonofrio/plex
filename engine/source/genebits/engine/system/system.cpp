#include "genebits/engine/system/system.h"

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
  for (const QueryDataAccess& data : data_access_)
  {
    if (data.thread_safe) continue;

    if (data.read_only)
    {
      if (HasWriteDependency(system.data_access_, data)) return true;
    }
    else if (HasReadDependency(system.data_access_, data))
    {
      return true;
    }
  }

  return false;
}

} // namespace genebits::engine