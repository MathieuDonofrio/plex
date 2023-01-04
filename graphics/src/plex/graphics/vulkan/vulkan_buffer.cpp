#include "plex/graphics/vulkan/vulkan_buffer.h"

namespace plex::graphics
{
VulkanBufferInterface::~VulkanBufferInterface()
{
  vkDestroyBuffer(device_, buffer_, nullptr);
  vkFreeMemory(device_, memory_, nullptr);
}

void* VulkanBufferInterface::Map()
{
  void* data;
  vkMapMemory(device_, memory_, 0, size_, 0, &data);
  return data;
}

void VulkanBufferInterface::Unmap()
{
  vkUnmapMemory(device_, memory_);
}
} // namespace plex::graphics
