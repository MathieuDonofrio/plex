#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_TEXEL_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_TEXEL_BUFFER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_buffer_view.h"
#include "genebits/engine/graphics/vulkan/resources/vulkan_uniform_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

/**
 * A vulkan uniform texel buffer is a buffer that is read only and can be sampled.
 */
class VulkanUniformTexelBuffer : public VulkanUniformBuffer
{
public:
  VulkanUniformTexelBuffer(VulkanBufferCreateInfo& create_info, VkFormat format);

private:
  VulkanBufferCreateInfo& AddInfos(
    VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER);

private:
  VulkanBufferView buffer_view_{};
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_TEXEL_BUFFER_H
