#ifndef PLEX_ECS_ECS_PACKAGE_H
#define PLEX_ECS_ECS_PACKAGE_H

#include "plex/app/app.h"
#include "plex/app/package.h"

namespace plex
{
///
/// Package for entity component system.
///
/// Adds an entity registry to the global context.
///
class ECSPackage : public Package
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
