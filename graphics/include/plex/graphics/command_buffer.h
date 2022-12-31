#ifndef PLEX_GRAPHICS_COMMAND_BUFFER_H
#define PLEX_GRAPHICS_COMMAND_BUFFER_H

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

  // TODO: add more commands
};
} // namespace plex::graphics

#endif
