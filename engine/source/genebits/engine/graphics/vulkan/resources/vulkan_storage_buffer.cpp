#include "vulkan_storage_buffer.h"

namespace genebits::engine
{

VulkanStorageBuffer::VulkanStorageBuffer(VulkanBufferCreateInfo& create_info) : VulkanBuffer(AddInfos(create_info)) {}

VulkanStorageBuffer::VulkanStorageBuffer(VulkanBufferCreateInfo& buffer_create_info, VkBufferUsageFlags usage)
  : VulkanBuffer(AddInfos(buffer_create_info, buffer_create_info.descriptor_type))
{}

VulkanBufferCreateInfo& VulkanStorageBuffer::AddInfos(VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type)
{
  info.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  info.descriptor_type = descriptor_type;
  return info;
}

} // namespace genebits::engine