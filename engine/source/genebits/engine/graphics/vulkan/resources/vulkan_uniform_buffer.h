#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_BUFFER_H

#include "genebits/engine/graphics/vulkan/vulkan_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

/**
 * A vulkan uniform buffer is a buffer that is read only.
 */
class VulkanUniformBuffer : public VulkanBuffer
{
public:
  VulkanUniformBuffer(VulkanBufferCreateInfo& create_info);

protected:
  VulkanUniformBuffer(VulkanBufferCreateInfo& buffer_create_info, VkBufferUsageFlags usage);

private:
  VulkanBufferCreateInfo& AddInfos(
    VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_UNIFORM_BUFFER_H
