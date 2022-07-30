#include "vulkan_storage_image.h"

namespace genebits::engine
{

VulkanStorageImage::VulkanStorageImage(VulkanStorageImageCreateInfo& storage_image_create_info)
  : device_(std::move(storage_image_create_info.vulkan_device)),
    VulkanResource(storage_image_create_info.binding_frequency,
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
      storage_image_create_info.shader_stage_flags)
{}

} // namespace genebits::engine