#ifndef GENEBITS_ENGINE_GRAPHICS_DEVICE_H
#define GENEBITS_ENGINE_GRAPHICS_DEVICE_H

namespace genebits::engine
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

} // namespace genebits::engine

#endif
