#include "vulkan_resource.h"

namespace genebits::engine
{

VulkanResource::VulkanResource(
  BindingFrequency binding_frequency, VkDescriptorType descriptor_type, VkShaderStageFlags shader_stage_flags)
  : binding_frequency_(binding_frequency), descriptor_type_(descriptor_type), shader_stage_flags_(shader_stage_flags)
{}

BindingFrequency VulkanResource::GetBindingFrequency() const noexcept
{
  return binding_frequency_;
}

VkDescriptorType VulkanResource::GetDescriptorType() const noexcept
{
  return descriptor_type_;
}
VkShaderStageFlags VulkanResource::GetShaderStageFlags() const noexcept
{
  return shader_stage_flags_;
}

} // namespace genebits::engine