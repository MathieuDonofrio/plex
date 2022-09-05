#include "plex/ecs/entity_registry.h"

namespace plex
{
EntityRegistry::EntityRegistry()
{
  storages_.resize(MaxArchetypes);
}

EntityRegistry::~EntityRegistry()
{
  for (auto storage : storages_)
  {
    if (storage) delete storage;
  }
}
} // namespace plex