#ifndef PLEX_EVENTS_EVENTS_PACKAGE_H
#define PLEX_EVENTS_EVENTS_PACKAGE_H

#include "plex/app/app.h"
#include "plex/app/package.h"

namespace plex
{
///
/// Package for event system.
///
class EventsPackage : public Package
{
private:
  ///
  /// Adds the package to the application
  ///
  /// @param[in] app Application to add package to.
  ///
  void DoAdd(App& app) const override;
};
} // namespace plex

#endif
