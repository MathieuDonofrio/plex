#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_TEXEL_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_TEXEL_BUFFER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_storage_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

/**
 * A vulkan storage texel buffer is a buffer is read and write and can be sampled.
 */
class VulkanStorageTexelBuffer : public VulkanStorageBuffer
{
public:
  VulkanStorageTexelBuffer(VulkanBufferCreateInfo& create_info);

private:
  VulkanBufferCreateInfo& AddInfos(
    VulkanBufferCreateInfo& info, VkDescriptorType descriptor_type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER);

private:
  VkBufferView buffer_view_handle_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_TEXEL_BUFFER_H
