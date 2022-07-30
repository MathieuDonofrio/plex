#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE_WITH_SAMPLER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE_WITH_SAMPLER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

namespace genebits::engine
{

struct VulkanImageWithSamplerCreateInfo
{
  std::shared_ptr<VulkanDevice> vulkan_device;
  BindingFrequency binding_frequency;
  VkShaderStageFlags shader_stage_flags;
  VkDeviceSize size;
  VkImageUsageFlags usage;
  VkMemoryPropertyFlags properties;
  VkFormat format;
  VkExtent2D extent;
};

/**
 * A image with sampler is the combination of a sampler and an image.
 * Sampled Image: A descriptor type that represents an image view, and supports filtered (sampled) and unfiltered
 * read-only access in a shader.
 */
class VulkanImageWithSampler : public VulkanResource
{
public:
  VulkanImageWithSampler(BindingFrequency binding_frequency,
    VkDescriptorType descriptor_type,
    VkShaderStageFlags shader_stage_flags,
    VulkanImageWithSamplerCreateInfo& image_with_sampler_create_info);

private:
  std::shared_ptr<VulkanDevice> device_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE_WITH_SAMPLER_H
