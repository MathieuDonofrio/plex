#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

namespace genebits::engine
{

struct VulkanBufferCreateInfo
{
  std::shared_ptr<VulkanDevice> device;
  VkDeviceSize size;
  VkBufferUsageFlags usage;
  VkMemoryPropertyFlags memory_properties;
  BindingFrequency binding_frequency;
  VkDescriptorType descriptor_type;
  VkShaderStageFlags shader_stage_flags;
};

class VulkanBuffer : public VulkanResource
{
public:
  ~VulkanBuffer();

  VulkanBuffer(const VulkanBuffer&) = delete;
  VulkanBuffer& operator=(const VulkanBuffer&) = delete;
  VulkanBuffer(VulkanBuffer&&) = default;
  VulkanBuffer& operator=(VulkanBuffer&&) = default;

protected:
  VulkanBuffer(VulkanBufferCreateInfo& buffer_create_info);

  VkBuffer GetBuffer() const noexcept;
private:
  std::shared_ptr<VulkanDevice> device_;
  VkBuffer buffer_handle_ { VK_NULL_HANDLE };
  VmaAllocation allocation_ { VK_NULL_HANDLE };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_H
