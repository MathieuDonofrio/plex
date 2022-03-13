#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H

#include "genebits/engine/graphics/vulkan/vulkan_present_image.h"

#include <array>

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
    uint32_t min_image_count = 0)
    : window_(window), device_(device), surface_(surface), min_image_count_(min_image_count)
  {
    if (!Initialize())
    {
      LOG_ERROR("Failed to initialize vulkan swapchain");

      return;
    }

    LOG_INFO("Vulkan swapchain initialized");
  }

  virtual ~VulkanSwapchain()
  {
    vkDeviceWaitIdle(device_->GetHandle()); // Wait for all work to be finished before destroying

    vkDestroySwapchainKHR(device_->GetHandle(), swapchain_, nullptr);

    LOG_INFO("Vulkan swapchain destroyed");
  }

  [[nodiscard]] VkSwapchainKHR GetHandle() const noexcept
  {
    return swapchain_;
  }

  bool UpdateSwapchainSize(uint32_t, uint32_t)
  {
    return false; // TODO
  }

private:
  bool Initialize()
  {
    VulkanSwapChainSupportDetails swapchain_support =
      QuerySwapChainSupport(device_->GetPhysicalDeviceHandle(), surface_->GetHandle());

    surface_format_ = ChooseSwapSurfaceFormat(swapchain_support.formats);
    present_mode_ = ChooseSwapPresentMode(swapchain_support.present_modes);
    swapchain_extent_ = ChooseSwapExtent(swapchain_support.capabilities);

    if (min_image_count_ == 0) min_image_count_ = GetRecommendedImageCount(swapchain_support);

    VkSwapchainCreateInfoKHR swapchain_create_info {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = surface_->GetHandle();
    swapchain_create_info.minImageCount = min_image_count_;
    swapchain_create_info.imageFormat = surface_format_.format;
    swapchain_create_info.imageColorSpace = surface_format_.colorSpace;
    swapchain_create_info.imageExtent = swapchain_extent_;
    swapchain_create_info.imageArrayLayers =
      1; // Is always 1 unless using multiple image (example: stereoscopic rendering)
    swapchain_create_info.imageUsage =
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // "usage color" is for direct rendering to the images, might be
                                           // VK_IMAGE_USAGE_TRANSFER_DST_BIT in case of post-processing where the
                                           // images are not rendered to directly but are the target for transfer
                                           // operations
    swapchain_create_info.preTransform =
      swapchain_support.capabilities.currentTransform; // Transform applied to the image prior to presentation
    swapchain_create_info.compositeAlpha =
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Specifies the composition mode for the alpha
                                         // channel when compositing with other windows
    swapchain_create_info.presentMode = present_mode_;
    swapchain_create_info.clipped = VK_TRUE; // Specifies if pixels that are not visible (example: under another window)
                                             // are allowed to be discarded by the Vulkan implementation
    swapchain_create_info.oldSwapchain =
      VK_NULL_HANDLE; // When not null, may be used when recreating the swapchain when it becomes out of date (example:
                      // window resize) to reuse resources and still present image acquired from it

    VulkanQueueFamilyIndices indices = device_->GetQueueFamilyIndices();

    if (indices.graphics_family_index != indices.present_family_index)
    {
      uint32_t indices_array[] = { indices.graphics_family_index, indices.present_family_index };

      swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapchain_create_info.queueFamilyIndexCount = 2;
      swapchain_create_info.pQueueFamilyIndices = indices_array;
    }
    else
    {
      swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchain_create_info.queueFamilyIndexCount = 0; // Optional
      swapchain_create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    if (vkCreateSwapchainKHR(device_->GetHandle(), &swapchain_create_info, nullptr, &swapchain_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create vulkan swapchain");

      return false;
    }

    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_, &min_image_count_, nullptr);

    std::vector<VkImage> image_ptrs(min_image_count_);

    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_, &min_image_count_, image_ptrs.data());

    swapchain_images_.reserve(image_ptrs.size());

    for (const auto& image_ptr : image_ptrs)
    {
      swapchain_images_.emplace_back(image_ptr, surface_format_.format, VK_IMAGE_ASPECT_COLOR_BIT, device_);
    }

    return true;
  }

  // Using the minimum image count directly might result in waiting for the driver to be done with internal operations
  // to acquire new images. It is recommended to acquire at least one more
  // If the capabilities specifies 0 as the max number of images, then there is no limit
  [[nodiscard]] uint32_t GetRecommendedImageCount(const VulkanSwapChainSupportDetails& chain_support_details) const
  {
    uint32_t image_count = chain_support_details.capabilities.minImageCount + 1;
    if (chain_support_details.capabilities.maxImageCount != 0
        && image_count > chain_support_details.capabilities.maxImageCount)
    {
      image_count = chain_support_details.capabilities.maxImageCount;
    }

    return image_count;
  }

  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
  {
    for (const auto& available_format : available_formats)
    {
      if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
          && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return available_format;
      }
    }

    return available_formats[0]; // If no format matches specified format return first available
  }

  VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
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

  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

  static VkFormat FindSupportedFormat(VkPhysicalDevice adapter_handle,
    const std::vector<VkFormat>& format_candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features)
  {
    for (VkFormat format : format_candidates)
    {
      VkFormatProperties format_properties;
      vkGetPhysicalDeviceFormatProperties(adapter_handle, format, &format_properties);

      if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features
          || tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features)
      {
        return format;
      }
    }

    LOG_ERROR("Failed to find supported format");
  }

private:
  VkSwapchainKHR swapchain_;

  std::shared_ptr<Window> window_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSurface> surface_;

  VkSurfaceFormatKHR surface_format_;
  VkPresentModeKHR present_mode_;
  VkExtent2D swapchain_extent_;

  uint32_t min_image_count_;

  std::vector<VulkanPresentImage> swapchain_images_;
  std::vector<VkFramebuffer> swapchain_frame_buffers;
};
} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H