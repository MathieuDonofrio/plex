#ifndef PLEX_GRAPHICS_COMMAND_BUFFER_H
#define PLEX_GRAPHICS_COMMAND_BUFFER_H

#include "plex/graphics/buffer.h"
#include "plex/graphics/material.h"
#include "plex/graphics/vertex.h"

namespace plex::graphics
{
class CommandBuffer
{
public:
  CommandBuffer() = default;
  virtual ~CommandBuffer() = default;

  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&&) = delete;
  CommandBuffer& operator=(CommandBuffer&&) = delete;

  virtual void Reset() = 0;

  virtual void Begin() = 0;
  virtual void End() = 0;

  virtual void BeginRenderPass() = 0;
  virtual void EndRenderPass() = 0;

  virtual void SetViewport(float x, float y, float width, float height, float min_depth, float max_depth) = 0;
  virtual void SetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;

  virtual void BindVertexBuffer(const Buffer<Vertex>& buffer) = 0;

  virtual void BindIndexBuffer(const Buffer<uint32_t>& buffer) = 0;
  virtual void BindIndexBuffer(const Buffer<uint16_t>& buffer) = 0;

  virtual void BindMaterial(Material* material) = 0;

  virtual void Draw(uint32_t vertex_count) = 0;
  virtual void DrawIndexed(uint32_t index_count) = 0;
};
} // namespace plex::graphics

#endif
