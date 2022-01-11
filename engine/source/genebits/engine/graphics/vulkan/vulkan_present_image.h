#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_IMAGE_H

#include "genebits/engine/graphics/vulkan/vulkan_image.h"

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

// TODO find a hierarchy with VulkanImage

class VulkanPresentImage
{
public:

  VulkanPresentImage() = default;

  VulkanPresentImage(VulkanPresentImage&& other) = default;
  VulkanPresentImage& operator=(VulkanPresentImage&&) = default;
  VulkanPresentImage(const VulkanPresentImage&) = delete;
  VulkanPresentImage& operator=(const VulkanPresentImage&) = delete;


  VulkanPresentImage(
    VkImage image_handle, VkFormat format, VkImageAspectFlags aspect_flags, std::shared_ptr<VulkanDevice> device)
    : image_handle_(image_handle), image_view_handle_(VK_NULL_HANDLE), device_(std::move(device))
  {
    if (!Initialize(format, aspect_flags)) { LOG_ERROR("Failed to create vulkan image"); }
    else
    {
      LOG_TRACE("Vulkan image view created");
    }
  };

  ~VulkanPresentImage()
  {
    if(device_)
    {
      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
      image_view_handle_ = VK_NULL_HANDLE;
      LOG_TRACE("Vulkan image view destroyed");
    }
  }

protected:
  VkImage image_handle_;
  VkImageView image_view_handle_;
  std::shared_ptr<VulkanDevice> device_;

  bool Initialize(VkFormat format, VkImageAspectFlags aspect_flags)
  {
    VkImageViewCreateInfo view_info {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image_handle_;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = aspect_flags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device_->GetHandle(), &view_info, nullptr, &image_view_handle_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create Vulkan image view");
      return false;
    }

    return true;
  }
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_PRESENT_IMAGE_H
