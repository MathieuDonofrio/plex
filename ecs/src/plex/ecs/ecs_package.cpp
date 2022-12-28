#include "plex/ecs/ecs_package.h"

#include "plex/ecs/entity_registry.h"

namespace plex
{

void ECSPackage::DoAdd(plex::App& app) const
{
  app.EmplaceGlobal<EntityRegistry>();
}

} // namespace plex
