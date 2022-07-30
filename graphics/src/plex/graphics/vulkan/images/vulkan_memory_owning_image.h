#ifndef PLEX_GRAPHICS_VULKAN_MEMORY_OWNING_IMAGE_H
#define PLEX_GRAPHICS_VULKAN_MEMORY_OWNING_IMAGE_H

#include "plex/graphics/vulkan/images/vulkan_image.h"
#include "plex/graphics/vulkan/vulkan_device.h"

#include "plex/debug/logging.h"

#include <memory>
#include <utility>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace plex
{

class VulkanMemoryOwningImage : public VulkanImage
{
public:
  VulkanMemoryOwningImage(std::shared_ptr<VulkanDevice> device,
    const VkImageCreateInfo& image_create_info,
    const VkImageViewCreateInfo& image_view_create_info)
    : VulkanImage(std::move(device), image_view_create_info.format)
  {
    image_handle_ = CreateVkImage(*device_, image_create_info, allocation_handle_);

    auto image_view_create_info_copy = image_view_create_info;
    image_view_create_info_copy.image = image_handle_;

    image_view_handle_ = CreateVkImageView(*device_, image_view_create_info);
  }

  VulkanMemoryOwningImage(const VulkanMemoryOwningImage&) = delete;
  VulkanMemoryOwningImage& operator=(const VulkanMemoryOwningImage&) = delete;

  VulkanMemoryOwningImage(VulkanMemoryOwningImage&&) = default;
  VulkanMemoryOwningImage& operator=(VulkanMemoryOwningImage&&) = default;

  ~VulkanMemoryOwningImage() override
  {
    if (image_view_handle_)
    {
      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
      image_view_handle_ = VK_NULL_HANDLE;
      LOG_TRACE("Vulkan image view destroyed");
    }

    if (allocation_handle_)
    {
      vmaDestroyImage(device_->GetAllocator(), image_handle_, allocation_handle_);
      image_handle_ = VK_NULL_HANDLE;
      allocation_handle_ = VK_NULL_HANDLE;
      LOG_TRACE("Vulkan image memory destroyed");
    }
  }

private:
  VmaAllocation allocation_handle_ { VK_NULL_HANDLE };
};

} // namespace plex

#endif
