#ifndef PLEX_GRAPHICS_SEMAPHORE_H
#define PLEX_GRAPHICS_SEMAPHORE_H

namespace plex::graphics
{
class Semaphore
{
public:
  Semaphore() = default;
  virtual ~Semaphore() = default;

  Semaphore(const Semaphore&) = delete;
  Semaphore& operator=(const Semaphore&) = delete;
  Semaphore(Semaphore&&) = delete;
  Semaphore& operator=(Semaphore&&) = delete;

  virtual void Wait() = 0;
};

} // namespace plex::graphics
#endif
