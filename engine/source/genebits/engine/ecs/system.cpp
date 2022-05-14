#include "genebits/engine/ecs/system.h"

namespace genebits::engine
{
namespace
{
  bool HasWriteDependency(const Vector<QueryDataAccess>& access, std::string_view name, uint32_t category_id) noexcept
  {
    for (const QueryDataAccess& data : access)
    {
      if (data.category_id == category_id && data.name == name) return !data.read_only;
    }

    return false;
  }

  bool HasReadDependency(const Vector<QueryDataAccess>& access, std::string_view name, uint32_t category_id) noexcept
  {
    for (const QueryDataAccess& data : access)
    {
      if (data.category_id == category_id && data.name == name) return true;
    }

    return false;
  }
} // namespace

[[nodiscard]] bool SystemObject::HasDependency(const SystemObject& system) const noexcept
{
  for (const QueryDataAccess& data : info_->data_access)
  {
    if (data.thread_safe) continue;
    if (data.category_id == QueryCategory::None) continue; // Special case: Ignore dependencies

    if (data.read_only)
    {
      if (HasWriteDependency(system.info_->data_access, data.name, data.category_id)) return true;
    }
    else if (HasReadDependency(system.info_->data_access, data.name, data.category_id))
    {
      return true;
    }
  }

  return false;
}

} // namespace genebits::engine