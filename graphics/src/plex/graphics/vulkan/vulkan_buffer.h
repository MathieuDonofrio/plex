#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_BUFFER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_BUFFER_H

#include "plex/graphics/buffer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics::pbi
{
class VulkanBufferInterface : public PolymorphicBufferInterface
{
public:
  VulkanBufferInterface(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, size_t size) noexcept
    : device_(device), buffer_(buffer), memory_(memory), size_(size)
  {}

  ~VulkanBufferInterface() override;

  void* Map() override;
  void Unmap() override;

  [[nodiscard]] void* GetNativeHandle() const noexcept override
  {
    return buffer_;
  }

private:
  VkDevice device_;
  VkBuffer buffer_;
  VkDeviceMemory memory_;
  size_t size_;
};

} // namespace plex::graphics::pbi

#endif
