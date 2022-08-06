#ifndef PLEX_APP_PACKAGE_H
#define PLEX_APP_PACKAGE_H

#include "plex/app/app.h"

namespace plex
{
///
/// A package is added to an application to provide it with objects and systems that it can use to build out its
/// functionality.
///
class Package
{
public:
  virtual ~Package() = default;

private:
  friend App;

  ///
  /// Adds all objects and systems to the application.
  ///
  /// @param[in] app The application to add itself to.
  ///
  virtual void DoAdd(App& app) const = 0;
};
} // namespace plex

#endif
