#include "plex/system/system.h"

namespace plex
{
[[nodiscard]] bool SystemObject::HasDependency(const SystemObject& system) const noexcept
{
  static constexpr std::string_view any {};

  for (const QueryDataAccess& data : data_access_)
  {
    if (data.thread_safe) continue; // Thread safe data cannot form a dependency

    for (const QueryDataAccess& other : system.data_access_)
    {
      if (other.thread_safe) continue; // Thread safe data cannot form a dependency
      if (other.read_only && data.read_only) continue; // Two reads cannot form a dependency
      if (other.source != data.source) continue; // Access from different data sources cannot form a dependency

      // The same section of the data source must be accessed for there to be a dependency.
      if (other.section == any || data.section == any || other.section == data.section)
      {
        return true;
      }
    }
  }

  return false;
}

} // namespace plex