#ifndef PLEX_GRAPHICS_COMMAND_BUFFER_H
#define PLEX_GRAPHICS_COMMAND_BUFFER_H

#include <vector>

#include "plex/graphics/shader.h"

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

  virtual void FirstTriangleTest(const std::vector<char>& vert, const std::vector<char>& frag) = 0; // TODO remove

  // TODO: add more commands
};
} // namespace plex::graphics

#endif
