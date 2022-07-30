#include "vulkan_storage_texel_buffer.h"

namespace genebits::engine
{

VulkanStorageTexelBuffer::VulkanStorageTexelBuffer(VulkanBufferCreateInfo& create_info)
  : VulkanStorageBuffer(AddInfos(create_info))
{}

VulkanBufferCreateInfo& VulkanStorageTexelBuffer::AddInfos(
  VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type)
{
  info.usage |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
  info.descriptor_type = descriptor_type;
  return info;
}

} // namespace genebits::engine