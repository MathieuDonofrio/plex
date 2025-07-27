#include "plex/graphics/vulkan/vulkan_buffer.h"

namespace plex::graphics
{
VulkanBufferInterface::~VulkanBufferInterface()
{
  vmaDestroyBuffer(allocator_, buffer_, allocation_);
}

void* VulkanBufferInterface::Map()
{
  void* data;
  vmaMapMemory(allocator_, allocation_, &data);
  return data;
}

void VulkanBufferInterface::Unmap()
{
  vmaUnmapMemory(allocator_, allocation_);
}
} // namespace plex::graphics
