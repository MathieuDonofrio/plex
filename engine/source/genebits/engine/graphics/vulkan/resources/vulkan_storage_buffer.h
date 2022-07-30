#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_BUFFER_H

#include "genebits/engine/graphics/vulkan/vulkan_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

/**
 * A vulkan storage buffer is a buffer is read and write.
 */
class VulkanStorageBuffer : public VulkanBuffer
{
public:
  VulkanStorageBuffer(VulkanBufferCreateInfo& create_info);

protected:
  VulkanStorageBuffer(VulkanBufferCreateInfo& buffer_create_info, VkBufferUsageFlags usage);

private:
  VulkanBufferCreateInfo& AddInfos(
    VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_BUFFER_H
