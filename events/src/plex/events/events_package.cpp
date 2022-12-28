#include "plex/events/events_package.h"

#include "plex/events/event_registry.h"

namespace plex
{

void EventsPackage::DoAdd(plex::App& app) const
{
  app.EmplaceGlobal<EventRegistry>();
}

} // namespace plex
