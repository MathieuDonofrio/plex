#ifndef PLEX_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H
#define PLEX_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H

#include "plex/utilities/enumerator.h"

namespace plex
{
///
/// Enum used to indicate the level of severity of logged messages involving the underlying graphics API.
///
enum class GraphicsDebugLevel : uint32_t
{
  Trace,
  Info,
  Warn,
  Error,
};

} // namespace plex

#endif
