#ifndef PLEX_GRAPHICS_COMMAND_POOL_H
#define PLEX_GRAPHICS_COMMAND_POOL_H

#include "plex/graphics/command_buffer.h"

namespace plex::graphics
{
class CommandPool
{
public:
  CommandPool() = default;
  virtual ~CommandPool() = default;

  CommandPool(const CommandPool&) = delete;
  CommandPool& operator=(const CommandPool&) = delete;
  CommandPool(CommandPool&&) = delete;
  CommandPool& operator=(CommandPool&&) = delete;

  virtual CommandBuffer* Allocate() = 0;
  virtual void Free(CommandBuffer* command_buffer) = 0;

  virtual void Reset() = 0;
};
} // namespace plex::graphics

#endif
