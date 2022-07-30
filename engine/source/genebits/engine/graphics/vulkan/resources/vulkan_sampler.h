#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SAMPLER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SAMPLER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

namespace genebits::engine
{

struct VulkanSamplerCreateInfo
{
  std::shared_ptr<VulkanDevice> vulkan_device;
  BindingFrequency binding_frequency;
  VkShaderStageFlags shader_stage_flags;
};

/**
 * A vulkan sampler let's the shader know how to sample an image.
 *  An object that contains state that controls how sampled image data is sampled (or filtered) when accessed in a
 * shader. Also a descriptor type describing the object. Represented by a VkSampler object.
 */
class VulkanSampler : public VulkanResource
{
public:
  VulkanSampler(VulkanSamplerCreateInfo& sampler_create_info);

private:
  std::shared_ptr<VulkanDevice> device_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SAMPLER_H
