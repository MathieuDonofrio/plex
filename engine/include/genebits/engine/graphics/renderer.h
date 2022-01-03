#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_RENDERER_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_RENDERER_H

#include "graphics_debug_level.h"
#include "rendering_backend.h"
#include "window.h"
namespace genebits::engine
{

class Renderer
{
public:
  Renderer() = default;
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual ~Renderer() = default;
};

Renderer* CreateRenderer(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend);

} // namespace genebits::engine
#endif
