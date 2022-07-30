#ifndef PLEX_GRAPHICS_DEVICE_H
#define PLEX_GRAPHICS_DEVICE_H

namespace plex
{

class Device
{
public:
  Device() = default;

  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;
  Device(Device&&) = delete;
  Device& operator=(Device&&) = delete;

  virtual ~Device() = default;

protected:
private:
};

} // namespace plex

#endif
