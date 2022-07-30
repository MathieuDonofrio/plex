#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_STORAGE_IMAGE_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include <memory>

namespace genebits::engine
{

struct VulkanStorageImageCreateInfo
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

class VulkanStorageImage : public VulkanResource
{
public:
  VulkanStorageImage(VulkanStorageImageCreateInfo& storage_image_create_info);

private:
  std::shared_ptr<VulkanDevice> device_;
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_STORAGE_IMAGE_H
