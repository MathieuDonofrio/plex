#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_IMAGE_H

#include "vulkan_image.h"

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanBufferAttachment : public VulkanImage
{
public:
  VulkanBufferAttachment(const VulkanBufferAttachment&) = delete;
  VulkanBufferAttachment& operator=(const VulkanBufferAttachment&) = delete;
  VulkanBufferAttachment(VulkanBufferAttachment&&) = default;
  VulkanBufferAttachment& operator=(VulkanBufferAttachment&&) = default;

  struct VulkanBufferAttachmentCreateInfo
  {
    std::shared_ptr<VulkanDevice> device;
    VkExtent2D extent;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags properties;
    VkImageAspectFlags aspect_flags;
  };

  ~VulkanBufferAttachment() override
  {
    if (image_view_handle_)
    {
      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
      image_view_handle_ = VK_NULL_HANDLE;
    }

    if (allocation_handle_)
    {
      vmaDestroyImage(device_->GetAllocator(), image_handle_, allocation_handle_);
      image_handle_ = VK_NULL_HANDLE;
      allocation_handle_ = VK_NULL_HANDLE;
    }
  }

protected:
  explicit VulkanBufferAttachment(const VulkanBufferAttachmentCreateInfo& buffer_image_create_info,
    const VulkanAttachmentCreateInfo& attachment_create_info)
    : VulkanImage(buffer_image_create_info.device, buffer_image_create_info.format, attachment_create_info)
  {
    if (!InitializeImage(buffer_image_create_info))
    {
      LOG_ERROR("Failed to initialize VulkanBufferImage");
      return;
    }
    else
    {
      LOG_INFO("Vulkan Buffer Image created");
    }

    if (!InitializeImageView(buffer_image_create_info.aspect_flags))
    {
      LOG_ERROR("Failed to initialize VulkanBufferImageView");
    }
    else
    {
      LOG_INFO("Vulkan buffer image view created");
    }
  }

  static VkFormat GetFormat(const std::shared_ptr<VulkanDevice>& device,
    const std::vector<VkFormat>& format_candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features)
  {
    return device->FindSupportedFormat(format_candidates, tiling, features);
  }

private:
  bool InitializeImage(const VulkanBufferAttachmentCreateInfo& create_info)
  {
    VkImageCreateInfo image_info {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1; // Might be good to support that in the future
    image_info.arrayLayers = 1;
    image_info.extent.width = create_info.extent.width;
    image_info.extent.height = create_info.extent.height;
    image_info.format = create_info.format;
    image_info.tiling = create_info.tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = create_info.usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.queueFamilyIndexCount = 0;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    alloc_info.priority = 1.0f;

    const auto allocator = device_->GetAllocator();
    if (vmaCreateImage(allocator, &image_info, &alloc_info, &image_handle_, &allocation_handle_, nullptr) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create image");
      return false;
    }
    else
    {
      LOG_TRACE("Image created");
    }

    return true;
  }

protected:
  VmaAllocation allocation_handle_;
  VkDeviceMemory image_memory_handle_ { VK_NULL_HANDLE };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_IMAGE_H
