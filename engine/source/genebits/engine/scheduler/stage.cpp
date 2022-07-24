#include "genebits/engine/scheduler/stage.h"

namespace genebits::engine
{
[[nodiscard]] bool Stage::HasExplicitOrder(const SystemObject& before, const SystemObject& after) const
{
  auto before_it =
    std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == before.Handle(); });

  if (before_it == registered_systems_.end()) return false;

  auto after_it =
    std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == after.Handle(); });

  if (after_it == registered_systems_.end()) return false;

  auto& before_info = system_infos_[std::distance(registered_systems_.begin(), before_it)];

  if (std::ranges::find(before_info.run_before, after.Handle()) != before_info.run_before.end()) return true;

  auto& after_info = system_infos_[std::distance(registered_systems_.begin(), after_it)];

  if (std::ranges::find(after_info.run_after, before.Handle()) != after_info.run_after.end()) return true;

  return false;
}

const SystemObject* Stage::GetSystemObject(SystemHandle handle) const
{
  for (const auto& system_object : registered_systems_)
  {
    if (system_object->Handle() == handle)
    {
      return system_object.get();
    }
  }

  return nullptr;
}
} // namespace genebits::engine