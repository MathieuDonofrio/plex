#ifndef PLEX_GRAPHICS_FRAME_H
#define PLEX_GRAPHICS_FRAME_H

#include "plex/graphics/command_buffer.h"
#include "plex/graphics/command_pool.h"

namespace plex::graphics
{
class Frame
{
public:
  Frame() = default;
  virtual ~Frame() = default;

  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;
  Frame(Frame&&) = delete;
  Frame& operator=(Frame&&) = delete;

  virtual CommandPool* GetCommandPool() = 0;
};
} // namespace plex::graphics
#endif // GENEBITS_FRAME_H
