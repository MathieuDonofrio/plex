#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_RENDERER_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_RENDERER_H

#include "genebits/engine/graphics/graphics_debug_level.h"
#include "genebits/engine/graphics/rendering_backend.h"
#include <genebits/engine/graphics/window.h>

namespace genebits::engine
{

class Renderer
{
public:
  Renderer(Window* window_handle,
    const char* application_name,
    bool is_debug,
    GraphicsDebugLevel debug_message_severity_threshold,
    RenderingBackend renderingBackend);

  virtual ~Renderer();

private:
  struct Pimpl;

  Pimpl* pimpl_;

  Window* window_handle_;
};

} // namespace genebits::engine
#endif
