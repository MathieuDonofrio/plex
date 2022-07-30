#include "vulkan_uniform_texel_buffer.h"

namespace genebits::engine
{

VulkanUniformTexelBuffer::VulkanUniformTexelBuffer(VulkanBufferCreateInfo& create_info, VkFormat format)
  : VulkanUniformBuffer(AddInfos(create_info))
{
  buffer_view_ = { create_info.device, format, GetBuffer() };
}

VulkanBufferCreateInfo& VulkanUniformTexelBuffer::AddInfos(
  VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type)
{
  info.usage |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
  info.descriptor_type = descriptor_type;
  return info;
}

} // namespace genebits::engine