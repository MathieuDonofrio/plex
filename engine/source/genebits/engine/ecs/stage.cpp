#include "genebits/engine/ecs/stage.h"

namespace genebits::engine
{
[[nodiscard]] bool Stage::HasExplicitOrder(const Ref<SystemObject>& before, const Ref<SystemObject>& after) const
{
  auto before_it =
    std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == before->Handle(); });

  if (before_it == registered_systems_.end()) return false;

  auto after_it =
    std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == after->Handle(); });

  if (after_it == registered_systems_.end()) return false;

  auto& before_info = system_infos_[std::distance(registered_systems_.begin(), before_it)];

  if (std::ranges::find(before_info.run_before, after->Handle()) != before_info.run_before.end()) return true;

  auto& after_info = system_infos_[std::distance(registered_systems_.begin(), after_it)];

  if (std::ranges::find(after_info.run_after, before->Handle()) != after_info.run_after.end()) return true;

  return false;
}
Ref<SystemObject> Stage::GetSystem(SystemHandle handle) const
{
  auto it = std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == handle; });

  if (it != registered_systems_.end()) return *it;

  return nullptr;
}

[[nodiscard]] std::span<Ref<SystemObject>> Stage::GetSystems()
{
  return { registered_systems_.begin(), registered_systems_.size() };
}
} // namespace genebits::engine