#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H

#include "genebits/engine/graphics/vulkan/vulkan_adapter_queries.h"
#include "genebits/engine/graphics/vulkan/vulkan_present_image.h"
#include "genebits/engine/graphics/vulkan/vulkan_swap_chain_support_details.h"

namespace genebits::engine
{

class VulkanSwapchain
{
public:
  ///
  ///
  /// @param min_image_count Number of buffers desired for presentation, 0 (auto) means the minimum supported count of
  /// buffers + 1
  ///
  VulkanSwapchain(std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanSurface> surface,
    VkPhysicalDevice adapter_handle,
    uint32_t image_width,
    uint32_t image_height,
    uint32_t min_image_count = 0)
    : device_(std::move(device)), surface_(std::move(surface)), swapchain_extent_({ image_width, image_height }),
      min_image_count_(min_image_count)
  {
    if (!Initialize(adapter_handle)) { LOG_ERROR("Failed to initialize vulkan swapchain"); }
    else
    {
      LOG_INFO("Vulkan swapchain initialized");
    }
  }

  virtual ~VulkanSwapchain()
  {
    // Wait for all work to be finished before the clean-up
    vkDeviceWaitIdle(device_->GetHandle());

    vkDestroySwapchainKHR(device_->GetHandle(), swapchain_handle_, nullptr);

    LOG_INFO("Vulkan swapchain destroyed");
  }

  [[nodiscard]] const VkSwapchainKHR GetSwapchainHandle() const noexcept
  {
    return swapchain_handle_;
  }

  bool UpdateSwapchainImageSize(uint32_t, uint32_t)
  {
    return false;
  }

private:
  VkSwapchainKHR swapchain_handle_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSurface> surface_;

  VkSwapchainCreateInfoKHR swapchain_create_info_;
  VkSurfaceFormatKHR surface_format_;
  VkPresentModeKHR present_mode_;
  VkExtent2D swapchain_extent_;

  uint32_t min_image_count_;

  std::vector<VulkanPresentImage> swapchain_images_;
  std::vector<VkFramebuffer> swapchain_frame_buffers;

  bool Initialize(VkPhysicalDevice adapter_handle)
  {
    VulkanQueueFamilyIndices queue_family_indices = device_->GetQueueFamilyIndices();

    VulkanSwapChainSupportDetails chain_support_details =
      VulkanAdapterQueries::GetAdapterSwapChainSupportDetails(adapter_handle, surface_->GetHandle());

    surface_format_ = ChooseSwapSurfaceFormat(chain_support_details.formats);
    present_mode_ = ChooseSwapPresentMode(chain_support_details.present_modes);
    swapchain_extent_ = ClampExtentToCapabilities(chain_support_details.capabilities, swapchain_extent_);

    if (min_image_count_ == 0) { min_image_count_ = GetRecommendedImageCount(chain_support_details); }

    // TODO use members instead of stay pure function
    swapchain_create_info_ =
      PopulateCreateInfo(surface_->GetHandle(), min_image_count_, surface_format_, swapchain_extent_);
    SetQueueFamilyIndicesInfo(swapchain_create_info_, queue_family_indices);
    SetMiscInfo(swapchain_create_info_, chain_support_details.capabilities, present_mode_);

    if (vkCreateSwapchainKHR(device_->GetHandle(), &swapchain_create_info_, nullptr, &swapchain_handle_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create vulkan swapchain");
      return false;
    }

    GetSwapchainImages();

    return true;
  }

  void GetSwapchainImages()
  {
    // Get the number of allocated images by the vulkan implementation by using nullptr
    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_handle_, &min_image_count_, nullptr);

    // Get the pointer to the images
    std::vector<VkImage> image_ptrs(min_image_count_);
    vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_handle_, &min_image_count_, image_ptrs.data());

    swapchain_images_.reserve(image_ptrs.size());
    for (const auto image_ptr : image_ptrs)
    {
      swapchain_images_.emplace_back(image_ptr, surface_format_.format, VK_IMAGE_ASPECT_COLOR_BIT, device_);
    }
  }

  VkSwapchainCreateInfoKHR PopulateCreateInfo(
    VkSurfaceKHR surface_handle, size_t image_count, VkSurfaceFormatKHR surface_format, VkExtent2D swapchain_extent)
  {
    VkSwapchainCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface_handle;

    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = swapchain_extent;
    create_info.imageArrayLayers = 1; // Is always 1 unless using multiple image (example: stereoscopic rendering)
    create_info.imageUsage =
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // "usage color" is for direct rendering to the images, might be
                                           // VK_IMAGE_USAGE_TRANSFER_DST_BIT in case of post-processing where the
                                           // images are not rendered to directly but are the target for transfer
                                           // operations
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Specifies the composition mode for the alpha
                                                                    // channel when compositing with other windows
    create_info.clipped = VK_TRUE; // Specifies if pixels that are not visible (example: under another window) are
                                   // allowed to be discarded by the Vulkan implementation
    create_info.oldSwapchain =
      VK_NULL_HANDLE; // When not null, may be used when recreating the swapchain when it becomes out of date (example:
                      // window resize) to reuse resources and still present image acquired from it

    return create_info;
  }

  void SetQueueFamilyIndicesInfo(VkSwapchainCreateInfoKHR& create_info, VulkanQueueFamilyIndices& queue_family_indices)
  {
    if (!queue_family_indices.SameFamilies())
    {
      auto queue_family_indices_array = queue_family_indices.GetFamilyIndicesArray();
      create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      create_info.queueFamilyIndexCount = queue_family_indices_array.size();
      create_info.pQueueFamilyIndices = queue_family_indices_array.data();
    }
    else
    {
      create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      create_info.queueFamilyIndexCount = 0; // Optional
      create_info.pQueueFamilyIndices = nullptr; // Optional
    }
  }

  void SetMiscInfo(
    VkSwapchainCreateInfoKHR& create_info, const VkSurfaceCapabilitiesKHR& capabilities, VkPresentModeKHR present_mode)
  {
    create_info.preTransform = capabilities.currentTransform; // Transform applied to the image prior to presentation
    create_info.presentMode = present_mode;
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
      if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB // TODO Check if alpha is needed
          && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return available_format;
      }
    }

    // If no format matches specified format return first available
    return available_formats[0];
  }

  VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
  {
    for (const auto& available_present_mode : available_present_modes)
    {
      if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) { return available_present_mode; }
    }

    return VK_PRESENT_MODE_FIFO_KHR; // Use FIFO if specified present mode is not available
  }

  VkExtent2D ClampExtentToCapabilities(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D swapchain_extent)
  {
    if (capabilities.currentExtent.width != UINT32_MAX) { return capabilities.currentExtent; }
    else
    {
      swapchain_extent.width =
        std::clamp(swapchain_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      swapchain_extent.height =
        std::clamp(swapchain_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return swapchain_extent;
    }
  }
};
} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAPCHAIN_H