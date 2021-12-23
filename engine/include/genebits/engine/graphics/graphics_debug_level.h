#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H

#include "genebits/engine/util/enumerator.h"

namespace genebits::engine
{
///
/// Enum flag used to indicate at which level of severity should the debug messages be printed
///
enum class GraphicsDebugLevel : uint32_t
{
  Trace = BitFlag(0),
  Info = BitFlag(1),
  Warn = BitFlag(2),
  Error = BitFlag(3)
};

} // namespace genebits::engine

#endif // GENEBITS_GRAPHIC_DEBUG_MESSAGE_SEVERITY_THRESHOLD_H
