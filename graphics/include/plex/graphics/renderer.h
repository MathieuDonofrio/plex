#ifndef PLEX_GRAPHICS_GRAPHIC_RENDERER_H
#define PLEX_GRAPHICS_GRAPHIC_RENDERER_H

#include "graphics_debug_level.h"
#include "rendering_backend.h"
#include "window.h"
namespace plex
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

  virtual void Draw(uint32_t frame_index) = 0;
};

std::shared_ptr<Renderer> CreateRenderer(std::shared_ptr<Window> window_handle,
  const std::string& application_name,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend);

} // namespace plex
#endif
