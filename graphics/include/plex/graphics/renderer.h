#ifndef PLEX_GRAPHICS_GRAPHIC_RENDERER_H
#define PLEX_GRAPHICS_GRAPHIC_RENDERER_H

#include "plex/graphics/frame.h"
#include "plex/graphics/window.h"

namespace plex::graphics
{
enum class BackendType
{
  Vulkan,
  // NOT SUPPORTED (YET or EVER)
  // OpenGL,
  // DirectX12,
  // Metal,
};

enum class DebugLevel
{
  Trace,
  Info,
  Warn,
  Error,
};

enum class PresentMode
{
  Immediate,
  Mailbox, // Recommended triple buffering
  FIFO, // Recommended double buffering
  FIFO_Relaxed
};

enum class BufferingMode
{
  Double,
  Triple
};

class Renderer
{
public:
  Renderer() = default;
  virtual ~Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  virtual Frame* AquireNextFrame() = 0;

  virtual void Render(Frame* frame, CommandBuffer* command_buffer) = 0;

  virtual void Present(Frame* frame) = 0;
};

struct RendererCreateInfo
{
  Window* window_handle;
  std::string application_name;
  DebugLevel debug_level;
  PresentMode present_mode;
  BufferingMode buffering_mode;
};

std::unique_ptr<Renderer> CreateRenderer(const RendererCreateInfo& create_info, BackendType backend_type);

} // namespace plex::graphics
#endif
