#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_NON_MEMORY_OWNING_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_NON_MEMORY_OWNING_IMAGE_H

#include "genebits/engine/graphics/vulkan/images/images2/vulkan_image2.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include "genebits/engine/debug/logging.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanNonMemoryOwningImage : public VulkanImage2
{
public:
  VulkanNonMemoryOwningImage(std::shared_ptr<VulkanDevice> device, const VkImageViewCreateInfo& image_view_create_info)
    : VulkanImage2(std::move(device), image_view_create_info.format)
  {
    image_view_handle_ = CreateVkImageView(*device_, image_view_create_info);
  }

  VulkanNonMemoryOwningImage(const VulkanNonMemoryOwningImage&) = delete;
  VulkanNonMemoryOwningImage& operator=(const VulkanNonMemoryOwningImage&) = delete;

  VulkanNonMemoryOwningImage(VulkanNonMemoryOwningImage&&) = default;
  VulkanNonMemoryOwningImage& operator=(VulkanNonMemoryOwningImage&&) = default;

  ~VulkanNonMemoryOwningImage() override
  {
    if (image_view_handle_)
    {
      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
      image_view_handle_ = VK_NULL_HANDLE;
      LOG_TRACE("Vulkan image view destroyed");
    }
  }
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_NON_MEMORY_OWNING_IMAGE_H
