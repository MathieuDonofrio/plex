#include "genebits/engine/ecs/stage.h"

namespace genebits::engine
{
[[nodiscard]] bool Stage::IsAfter(const SystemObject& before, const SystemObject& after) const
{
  auto system_it =
    std::ranges::find_if(registered_systems_, [&](auto& system) { return system->Handle() == before.Handle(); });

  if (system_it != registered_systems_.end())
  {
    auto& info = system_infos_[system_it - registered_systems_.begin()];

    return std::ranges::find_if(info.run_after, [&](auto& system) { return system == after.Handle(); })
           != info.run_after.end();
  }

  return false;
}
} // namespace genebits::engine