#ifndef PLEX_GRAPHICS_GRAPHIC_RENDERER_H
#define PLEX_GRAPHICS_GRAPHIC_RENDERER_H

#include <filesystem>
#include <memory>
#include <optional>

#include "plex/graphics/buffer.h"
#include "plex/graphics/command_buffer.h"
#include "plex/graphics/material.h"
#include "plex/graphics/shader.h"
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

  virtual CommandBuffer* AcquireNextFrame() = 0;
  virtual void Render() = 0;
  virtual void Present() = 0;
  virtual void WaitIdle() = 0;

  [[nodiscard]] virtual std::unique_ptr<Material> CreateMaterial(const MaterialCreateInfo& create_info) = 0;
  [[nodiscard]] virtual std::unique_ptr<Shader> CreateShader(const std::filesystem::path& source, ShaderType type) = 0;

  template<typename Type>
  [[nodiscard]] Buffer<Type> CreateBuffer(size_t size, BufferUsageFlags usage, MemoryPropertyFlags properties)
  {
    return { CreateBuffer(size * sizeof(Type), usage, properties), static_cast<uint32_t>(size), usage, properties };
  }

private:
  virtual std::unique_ptr<pbi::PolymorphicBufferInterface> CreateBuffer(
    size_t size, BufferUsageFlags usage, MemoryPropertyFlags properties) = 0;
};

struct RendererCreateInfo
{
  std::string application_name;
  Window* window;
  DebugLevel debug_level;
  PresentMode present_mode;
  BufferingMode buffering_mode;
};

std::unique_ptr<Renderer> CreateRenderer(const RendererCreateInfo& create_info, BackendType backend_type);

} // namespace plex::graphics
#endif
