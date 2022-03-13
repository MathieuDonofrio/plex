#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H

#include "genebits/engine/utilities/enumerator.h"

namespace genebits::engine
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

} // namespace genebits::engine

#endif // GENEBITS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H
