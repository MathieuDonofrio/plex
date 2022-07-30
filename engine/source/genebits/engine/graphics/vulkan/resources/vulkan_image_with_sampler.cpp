#include "vulkan_image_with_sampler.h"

namespace genebits::engine
{

VulkanImageWithSampler::VulkanImageWithSampler(BindingFrequency binding_frequency,
  VkDescriptorType descriptor_type,
  VkShaderStageFlags shader_stage_flags,
  VulkanImageWithSamplerCreateInfo& image_with_sampler_create_info)
  : device_(std::move(image_with_sampler_create_info.vulkan_device)),
    VulkanResource(image_with_sampler_create_info.binding_frequency,
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      image_with_sampler_create_info.shader_stage_flags)
{

}

} // namespace genebits::engine
