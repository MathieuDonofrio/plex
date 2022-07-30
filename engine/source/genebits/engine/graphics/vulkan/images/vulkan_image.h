#ifndef GENEBITS_VULKAN_IMAGE_H
#define GENEBITS_VULKAN_IMAGE_H

#include "../vulkan_device.h"
#include "genebits/engine/debug/logging.h"
#include "vulkan_attachment.h"

#include <memory>
#include <utility>

#include "../../../../../../../../../.conan/data/vulkan-headers/1.3.216.0/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include/vulkan/vulkan_core.h"

namespace genebits::engine
{
// TODO instead of using lazy initialization, use a friend factory to call Initialize() (make_image) and make it so it's
//  impossible to create an image without the factory
class VulkanImage : public VulkanRenderingBuffer
{
public:
  // Destruction of VkImage and VkImageView are handled by the subclass vulkan_buffer_image where those handles are
  // actually owned by the image contrary to the swapchain images which are owned by the swapchain itself
  virtual ~VulkanImage() = default;

  VulkanImage(const VulkanImage&) = delete;
  VulkanImage& operator=(const VulkanImage&) = delete;

  VulkanImage& operator=(VulkanImage&& other) noexcept
  {
    device_ = std::move(other.device_);

    format_ = other.format_;

    image_handle_ = other.image_handle_;
    other.image_handle_ = VK_NULL_HANDLE;

    image_view_handle_ = other.image_view_handle_;
    other.image_view_handle_ = VK_NULL_HANDLE;

    return *this;
  };

  VulkanImage(VulkanImage&& other) noexcept : VulkanRenderingBuffer(std::move(other))
  {
    device_ = std::move(other.device_);

    format_ = other.format_;

    image_handle_ = other.image_handle_;
    other.image_handle_ = VK_NULL_HANDLE;

    image_view_handle_ = other.image_view_handle_;
    other.image_view_handle_ = VK_NULL_HANDLE;
  }

  [[nodiscard]] VkImageView GetImageViewHandle() const noexcept
  {
    return image_view_handle_;
  }

  [[nodiscard]] VkFormat GetFormat() const noexcept
  {
    return format_;
  }

protected:
  struct VulkanImageCreateInfo
  {
    std::shared_ptr<VulkanDevice> device;
    VkImage image_handle;
    const VkFormat& format;
    VkImageAspectFlags aspect_flags;
  };

  VulkanImage(
    std::shared_ptr<VulkanDevice> device, VkFormat format, const VulkanAttachmentCreateInfo& attachment_create_info)
    : device_(std::move(device)), format_(format), VulkanRenderingBuffer(attachment_create_info)
  {}

  VulkanImage(const VulkanImageCreateInfo& create_info, const VulkanAttachmentCreateInfo& attachment_create_info)
    : image_handle_(create_info.image_handle), format_(create_info.format), device_(create_info.device),
      VulkanRenderingBuffer(attachment_create_info)
  {
    if (!InitializeImageView(create_info.aspect_flags))
    {
      LOG_ERROR("Failed to initialize VulkanImageView");
      LOG_ERROR("Failed to initialize VulkanImage");
    }
    else
    {
      LOG_INFO("Vulkan Image created");
    }
  }

  [[nodiscard]] bool InitializeImageView(VkImageAspectFlags aspect_flags)
  {
    VkImageViewCreateInfo view_info {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image_handle_;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format_;
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
    LOG_TRACE("Vulkan image view created");

    return true;
  }

protected:
  std::shared_ptr<VulkanDevice> device_;
  VkImage image_handle_ { VK_NULL_HANDLE };
  VkImageView image_view_handle_ { VK_NULL_HANDLE };

  VkFormat format_ {};
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_IMAGE_H
