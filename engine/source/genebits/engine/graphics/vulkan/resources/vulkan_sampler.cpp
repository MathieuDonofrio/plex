#include "vulkan_sampler.h"

#include <utility>

namespace genebits::engine
{

VulkanSampler::VulkanSampler(VulkanSamplerCreateInfo& sampler_create_info)
  : device_(std::move(sampler_create_info.vulkan_device)),
    VulkanResource(
      sampler_create_info.binding_frequency, VK_DESCRIPTOR_TYPE_SAMPLER, sampler_create_info.shader_stage_flags)
{}

} // namespace genebits::engine