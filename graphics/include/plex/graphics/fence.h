#ifndef PLEX_GRAPHICS_FENCE_H
#define PLEX_GRAPHICS_FENCE_H

namespace plex::graphics
{
class Fence
{
public:
  Fence() = default;
  virtual ~Fence() = default;

  Fence(const Fence&) = delete;
  Fence& operator=(const Fence&) = delete;
  Fence(Fence&&) = delete;
  Fence& operator=(Fence&&) = delete;

  virtual void Reset() = 0;

  virtual void Wait() = 0;
};
} // namespace plex::graphics

#endif
