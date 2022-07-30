#include "vulkan_uniform_buffer.h"

namespace genebits::engine
{

VulkanUniformBuffer::VulkanUniformBuffer(VulkanBufferCreateInfo& create_info) : VulkanBuffer(AddInfos(create_info)) {}

VulkanUniformBuffer::VulkanUniformBuffer(VulkanBufferCreateInfo& buffer_create_info, VkBufferUsageFlags usage)
  : VulkanBuffer(AddInfos(buffer_create_info, buffer_create_info.descriptor_type))
{}

VulkanBufferCreateInfo& VulkanUniformBuffer::AddInfos(VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type)
{
  info.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  info.descriptor_type = descriptor_type;
  return info;
}

} // namespace genebits::engine