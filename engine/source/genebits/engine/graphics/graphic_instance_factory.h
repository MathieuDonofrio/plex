#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_INSTANCE_FACTORY_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_INSTANCE_FACTORY_H

#include "genebits/engine/graphics/graphics_debug_level.h"
#include "genebits/engine/graphics/rendering_backend.h"
#include "graphic_instance.h"
#include <memory>

namespace genebits::engine
{

std::unique_ptr<GraphicInstance> CreateInstance(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend);

} // namespace genebits::engine

#endif
