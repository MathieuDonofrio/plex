#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_RESOURCE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_RESOURCE_H

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

enum class BindingFrequency
{
  Static,
  PerFrame,
  PerDrawCall,
  Auto
};

struct VulkanResourceCreateInfo
{
  BindingFrequency binding_frequency;
  VkDescriptorType descriptor_type;
  VkShaderStageFlags shader_stage_flags;
};

/**
 * A vulkan resource is a resource that can be used by a shader. It can be a buffer, image, sampler, etc.
 * Resources can be sampled contrary to rendering buffers.
 */
class VulkanResource
{
public:
  VulkanResource(
    BindingFrequency binding_frequency, VkDescriptorType descriptor_type, VkShaderStageFlags shader_stage_flags);

  [[nodiscard]] VkDescriptorType GetDescriptorType() const noexcept;

  [[nodiscard]] VkShaderStageFlags GetShaderStageFlags() const noexcept;

  [[nodiscard]] BindingFrequency GetBindingFrequency() const noexcept;

private:
  BindingFrequency binding_frequency_;
  VkDescriptorType descriptor_type_;
  VkShaderStageFlags shader_stage_flags_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_RESOURCE_H
