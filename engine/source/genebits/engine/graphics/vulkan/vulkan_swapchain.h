#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment.h"
#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_group.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_image.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_image_factory.h"

#include <array>
#include <iostream>
#include <unordered_map>
#include <utility>

namespace genebits::engine
{
struct VulkanSwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;

  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;
};

VulkanSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

class VulkanSwapchain
{
public:
  VulkanSwapchain(std::shared_ptr<Window> window,
    std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanSurface> surface,
    SwapchainImageUsage swapchain_image_usage = SwapchainImageUsage::ColorAttachment,
    uint32_t min_image_count = 0)
    : window_(std::move(window)), device_(std::move(device)), surface_(std::move(surface)),
      image_count_(min_image_count)
  {
    if (!Initialize(swapchain_image_usage))
    {
      LOG_ERROR("Failed to initialize vulkan swapchain");

      return;
    }

    LOG_INFO("Vulkan swapchain initialized");
  }

  virtual ~VulkanSwapchain()
  {
    vkDeviceWaitIdle(device_->GetHandle()); // Wait for all work to be finished before destroying

    vkDestroySwapchainKHR(device_->GetHandle(), swapchain_handle_, nullptr);

    LOG_INFO("Vulkan swapchain destroyed");
  }

  [[nodiscard]] VkSwapchainKHR GetHandle() const noexcept
  {
    return swapchain_handle_;
  }

  [[nodiscard]] const VkExtent2D& GetExtent() const
  {
    return swapchain_extent_;
  }

  [[nodiscard]] uint32_t GetImageCount() const
  {
    return image_count_;
  }

  [[nodiscard]] const VkImage* GetImages() const noexcept
  {
    return swapchain_images_handles_.data();
  }

  [[nodiscard]] const VulkanAttachmentGroup& GetAttachments(uint32_t attachment_index)
  {
    auto attachment_create_info = GetAttachmentCreateInfo(attachment_index);

    if (swapchain_images_.IsEmpty())
    {
      std::vector<std::shared_ptr<VulkanAttachment>> attachments;
      attachments.reserve(swapchain_images_handles_.size());
      for (const auto& image_handle : swapchain_images_handles_)
      {
        const auto image_view_create_info = VulkanImageFactory::CreateImageViewCreateInfo(
          surface_format_.format, VK_IMAGE_ASPECT_COLOR_BIT, image_handle);
        const auto image = VulkanImageFactory::CreateNonMemoryOwningImage(device_, image_view_create_info);
        attachments.emplace_back(std::make_shared<VulkanAttachment>(image, attachment_create_info));
      }
      swapchain_images_ = std::move(attachments);
    }

    return swapchain_images_;
  }

  [[maybe_unused]] bool UpdateSwapchainSize(uint32_t, uint32_t)
  {
    return false; // TODO
  }

private:
  bool Initialize(SwapchainImageUsage usage)
  {
    VulkanSwapChainSupportDetails swapchain_support =
      QuerySwapChainSupport(device_->GetPhysicalDeviceHandle(), surface_->GetHandle());

    surface_format_ = ChooseSwapSurfaceFormat(swapchain_support.formats);
    present_mode_ = ChooseSwapPresentMode(swapchain_support.present_modes);
    swapchain_extent_ = ChooseSwapExtent(swapchain_support.capabilities);

    if (image_count_ == 0) image_count_ = GetRecommendedImageCount(swapchain_support);

    VkSwapchainCreateInfoKHR swapchain_create_info {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = surface_->GetHandle();
    swapchain_create_info.minImageCount = image_count_;
    swapchain_create_info.imageFormat = surface_format_.format;
    swapchain_create_info.imageColorSpace = surface_format_.colorSpace;
    swapchain_create_info.imageExtent = swapchain_extent_;

    // Is always 1 unless using multiple image (example: stereoscopic rendering)
    swapchain_create_info.imageArrayLayers = 1;

    // "usage color" is for direct rendering to the images, might be
    // VK_IMAGE_USAGE_TRANSFER_DST_BIT in case of post-processing where the
    // images are not rendered to directly but are the target for transfer
    // operations or VK_IMAGE_USAGE_STORAGE_BIT for access from a compute shader
    swapchain_create_info.imageUsage = ConvertSwapchainUsage(usage);

    // Transform applied to the image prior to presentation
    swapchain_create_info.preTransform = swapchain_support.capabilities.currentTransform;

    // Specifies the composition mode for the alpha
    // channel when compositing with other windows
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    swapchain_create_info.presentMode = present_mode_;

    // Specifies if pixels that are not visible (example: under another window)
    // are allowed to be discarded by the Vulkan implementation
    swapchain_create_info.clipped = VK_TRUE;

    // When not null, may be used when recreating the swapchain when it becomes out of date (example:
    // window resize) to reuse resources and still present image acquired from it
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    VulkanQueueFamilyIndices indices = device_->GetQueueFamilyIndices();

    if (!indices.AreAllFamilySame())
    {
      uint32_t indices_array[] = {
        indices.graphics_family_index, indices.present_family_index, indices.compute_family_index
      };

      swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapchain_create_info.queueFamilyIndexCount = 3;
      swapchain_create_info.pQueueFamilyIndices = indices_array;
    }
    else
    {
      swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchain_create_info.queueFamilyIndexCount = 0; // Optional
      swapchain_create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    if (vkCreateSwapchainKHR(device_->GetHandle(), &swapchain_create_info, nullptr, &swapchain_handle_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create vulkan swapchain");

      return false;
    }

    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_handle_, &image_count_, nullptr);

    swapchain_images_handles_ = std::vector<VkImage>(image_count_);

    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_handle_, &image_count_, swapchain_images_handles_.data());

    return true;
  }

  [[nodiscard]] static VkImageUsageFlags ConvertSwapchainUsage(SwapchainImageUsage usage) noexcept
  {
    switch (usage)
    {
    case SwapchainImageUsage::StorageImage: return VK_IMAGE_USAGE_STORAGE_BIT;
    case SwapchainImageUsage::ColorAttachment: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    case SwapchainImageUsage::TransferDestination: return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    default: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
  }

  // Using the minimum image count directly might result in waiting for the driver to be done with internal operations
  // to acquire new images. It is recommended to acquire at least one more.
  // If the capabilities specifies 0 as the max number of images, then there is no limit
  [[nodiscard]] static uint32_t GetRecommendedImageCount(
    const VulkanSwapChainSupportDetails& chain_support_details) noexcept
  {
    const VkSurfaceCapabilitiesKHR& capabilities = chain_support_details.capabilities;

    uint32_t image_count = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount != 0 && image_count > capabilities.maxImageCount)
    {
      image_count = capabilities.maxImageCount;
    }

    return image_count;
  }

  static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) noexcept
  {
    for (const auto& available_format : available_formats)
    {
      // VK_FORMAT_B8G8R8A8_SRGB is not supported for storage images
      // so VK_FORMAT_B8G8R8A8_UNORM is used instead
      //See Table 67 in the 1.3.211.0 vulkan spec for more info
      if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM
          && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return available_format;
      }
    }

    return available_formats[0]; // If no format matches specified format return first available
  }

  static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) noexcept
  {
    for (const auto& available_present_mode : available_present_modes)
    {
      // MAILBOX is like FIFO but instead of blocking when the queue is full, the old images are replaced with the newer
      // ones. Allows framerate to be as fast as possible while still avoiding tearing.
      if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) return available_present_mode;
    }

    // FIFO is a queue that dequeues an image when the display is refreshed. Blocks if the queue is full.
    // Similar to "vertical sync".
    return VK_PRESENT_MODE_FIFO_KHR; // FIFO is guaranteed to be available
  }

  [[nodiscard]] VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) return capabilities.currentExtent;
    else
    {
      auto [width, height] = window_->GetFrameBufferSize();

      VkExtent2D extent { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

      extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.minImageExtent.width);
      extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.minImageExtent.height);

      return extent;
    }
  }

  [[nodiscard]] VulkanAttachmentCreateInfo GetAttachmentCreateInfo(uint32_t attachment_index) const noexcept
  {
    AttachmentOp attachment_op = {
      VK_ATTACHMENT_LOAD_OP_CLEAR, // Clear buffer when starting render pass // TODO LOAD_OP_CLEAR requires a clear
                                   // value see: https://vulkan-tutorial.com/Depth_buffering#page_Framebuffer
      VK_ATTACHMENT_STORE_OP_STORE // Keep results after render pass
    };

    LayoutTransitionSequence layout_transition_sequence = {
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VulkanAttachmentCreateInfo attachment_create_info = {
      attachment_index, layout_transition_sequence, attachment_op, AttachmentType::Present
    };

    return attachment_create_info;
  }

private:
  VkSwapchainKHR swapchain_handle_ {};

  std::shared_ptr<Window> window_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSurface> surface_;

  VkSurfaceFormatKHR surface_format_ {};
  VkPresentModeKHR present_mode_;
  VkExtent2D swapchain_extent_ {};

  uint32_t image_count_;

  std::vector<VkImage> swapchain_images_handles_;
  VulkanAttachmentGroup swapchain_images_;
};
} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H