#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE2_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE2_H

#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanImage2
{
public:
  VulkanImage2() = delete;

  VulkanImage2(const VulkanImage2&) = delete;
  VulkanImage2& operator=(const VulkanImage2&) = delete;

  VulkanImage2(VulkanImage2&&) = default;
  VulkanImage2& operator=(VulkanImage2&&) = default;

  virtual ~VulkanImage2() = default;

  [[nodiscard]] VkImage GetHandle() const noexcept
  {
    return image_handle_;
  };

  [[nodiscard]] VkImageView GetImageViewHandle() const noexcept
  {
    return image_view_handle_;
  };

  [[nodiscard]] VkFormat GetFormat() const noexcept
  {
    return format_;
  }

protected:
  VulkanImage2(std::shared_ptr<VulkanDevice> device, VkFormat format) : device_(std::move(device)), format_(format) {}

  /**
   * Create a vulkan image view on a device with the given parameters.
   * @param device The device to create the image on.
   * @param image_view_create_info The image view create info.
   * @return The image view handle.
   */
  static VkImageView CreateVkImageView(const VulkanDevice& device, const VkImageViewCreateInfo& image_view_create_info)
  {
    VkImageView image_view;
    if (vkCreateImageView(device.GetHandle(), &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create Vulkan image view");
      return {};
    }
    LOG_TRACE("Vulkan image view created");
    return image_view;
  }

  /**
   * Create a vulkan image on a device with the given parameters.
   * @param device The device to create the image on.
   * @param image_create_info The image create info.
   * @param allocation The allocation to create the image with.
   * @return The image handle.
   */
  static VkImage CreateVkImage(
    const VulkanDevice& device, const VkImageCreateInfo& image_create_info, VmaAllocation& allocation)
  {
    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    alloc_info.priority = 1.0f;

    VkImage image;
    const auto allocator = device.GetAllocator();
    if (vmaCreateImage(allocator, &image_create_info, &alloc_info, &image, &allocation, nullptr) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create image");
      return {};
    }

    LOG_TRACE("Image created");
    return image;
  }

protected:
  std::shared_ptr<VulkanDevice> device_;
  VkImage image_handle_ { VK_NULL_HANDLE };
  VkImageView image_view_handle_ { VK_NULL_HANDLE };
  VkFormat format_ { VK_FORMAT_UNDEFINED };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE2_H
