#ifndef PLEX_GRAPHICS_FRAME_H
#define PLEX_GRAPHICS_FRAME_H

#include "plex/graphics/command_buffer.h"
#include "plex/graphics/command_pool.h"
#include "plex/graphics/fence.h"
#include "plex/graphics/image.h"
#include "plex/graphics/semaphore.h"

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

  virtual Image* GetImage() = 0;

  virtual Semaphore* GetImageAvailableSemaphore() = 0;
  virtual Semaphore* GetRenderFinishedSemaphore() = 0;

  virtual Fence* GetFence() = 0;

  virtual CommandPool* GetCommandPool() = 0;
  virtual CommandBuffer* GetMainCommandBuffer() = 0;
};
} // namespace plex::graphics
#endif // GENEBITS_FRAME_H
