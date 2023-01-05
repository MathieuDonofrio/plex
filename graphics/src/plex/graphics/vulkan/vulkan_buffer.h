#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_BUFFER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_BUFFER_H

#include "plex/graphics/buffer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanBufferInterface : public PolymorphicBufferInterface
{
public:
  VulkanBufferInterface(VkBuffer buffer, VmaAllocation allocation, VmaAllocator allocator) noexcept
    : buffer_(buffer), allocation_(allocation), allocator_(allocator)
  {}

  ~VulkanBufferInterface() override;

  void* Map() override;
  void Unmap() override;

  [[nodiscard]] void* GetNativeHandle() const noexcept override
  {
    return buffer_;
  }

private:
  VkBuffer buffer_;
  VmaAllocation allocation_;
  VmaAllocator allocator_;
};
} // namespace plex::graphics

#endif
