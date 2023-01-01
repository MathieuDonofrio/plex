#include "plex/graphics/vulkan/vulkan_swapchain.h"

#include <algorithm>

#include "plex/debug/logging.h"

namespace plex::graphics
{
namespace
{
  constexpr VkPresentModeKHR FromPresentMode(PresentMode mode)
  {
    switch (mode)
    {
    case PresentMode::Immediate: return VK_PRESENT_MODE_IMMEDIATE_KHR;
    case PresentMode::FIFO: return VK_PRESENT_MODE_FIFO_KHR;
    case PresentMode::FIFO_Relaxed: return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    case PresentMode::Mailbox: return VK_PRESENT_MODE_MAILBOX_KHR;
    default: return VK_PRESENT_MODE_FIFO_KHR; // Is guaranteed to be available
    }
  }

  constexpr ImageFormat ToImageFormat(VkFormat format)
  {
    switch (format)
    {
    case VK_FORMAT_R8G8B8_UNORM: return ImageFormat::RGB;
    case VK_FORMAT_R8G8B8A8_UNORM: return ImageFormat::RGBA;
    case VK_FORMAT_B8G8R8_UNORM: return ImageFormat::BGR;
    case VK_FORMAT_B8G8R8A8_UNORM: return ImageFormat::BGRA;
    case VK_FORMAT_R8_UNORM: return ImageFormat::LUMINANCE;
    case VK_FORMAT_R8G8_UNORM: return ImageFormat::LUMINANCE_ALPHA;
    default: return ImageFormat::UNDEFINED;
    }
  }
} // namespace

VulkanSwapchain::VulkanSwapchain(VulkanDevice* device,
  VulkanSurface* surface,
  PresentMode present_mode,
  uint32_t width,
  uint32_t height,
  uint32_t image_count)
  : device_(device->GetHandle()), present_mode_(present_mode), extent_ { width, height }, image_count_(image_count)
{
  std::vector<VkSurfaceFormatKHR> surface_formats = surface->GetSurfaceFormats(device->GetPhysicalHandle());

  VkSurfaceFormatKHR surface_format;
  if (surface_formats.size() == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED)
  {
    surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
    surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  }
  else
  {
    surface_format = surface_formats[0];
  }

  std::vector<VkPresentModeKHR> present_modes = surface->GetPresentModes(device->GetPhysicalHandle());

  VkPresentModeKHR surface_present_mode = FromPresentMode(present_mode);

  if (std::find(present_modes.begin(), present_modes.end(), surface_present_mode) == present_modes.end())
  {
    LOG_WARN("Present mode not supported by surface, falling back to FIFO");

    surface_present_mode = VK_PRESENT_MODE_FIFO_KHR;
  }

  VkSurfaceCapabilitiesKHR surface_capabilities = surface->GetSurfaceCapabilities(device->GetPhysicalHandle());

  if (surface_capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
  {
    extent_.width =
      std::clamp(extent_.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
    extent_.height = std::clamp(
      extent_.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

    LOG_WARN("Surface extent is undefined, using window size: {}x{}", extent_.width, extent_.height);
  }
  else
  {
    extent_ = surface_capabilities.currentExtent;
  }

  if (surface_capabilities.maxImageCount > 0 && image_count_ > surface_capabilities.maxImageCount)
  {
    image_count_ = surface_capabilities.maxImageCount;
    LOG_WARN("Image count exceeds maximum, clamping to {}", image_count_);
  }

  VkSwapchainCreateInfoKHR swapchain_create_info = {};
  swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_create_info.surface = surface->GetHandle();
  swapchain_create_info.minImageCount = image_count_;
  swapchain_create_info.imageFormat = surface_format.format;
  swapchain_create_info.imageColorSpace = surface_format.colorSpace;
  swapchain_create_info.imageExtent = extent_;
  swapchain_create_info.imageArrayLayers = 1;
  swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchain_create_info.preTransform = surface_capabilities.currentTransform;
  swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchain_create_info.presentMode = surface_present_mode;
  swapchain_create_info.clipped = VK_TRUE;
  swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device_, &swapchain_create_info, nullptr, &swapchain_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create swapchain");
  }

  uint32_t swapchain_image_count = 0;
  vkGetSwapchainImagesKHR(device_, swapchain_, &swapchain_image_count, nullptr);
  images_.resize(swapchain_image_count);
  vkGetSwapchainImagesKHR(device_, swapchain_, &swapchain_image_count, images_.data());

  image_count_ = swapchain_image_count;

  for (size_t i = 0; i < swapchain_image_count; i++)
  {
    VkImageViewCreateInfo image_view_create_info = {};
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.image = images_[i];
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.format = surface_format.format;
    image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = 1;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device_, &image_view_create_info, nullptr, &image_views_[i]) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create image view");
    }
  }
}

VulkanSwapchain::~VulkanSwapchain()
{
  vkDestroySwapchainKHR(device_, swapchain_, nullptr);
}

uint32_t VulkanSwapchain::AquireNextImage(VkSemaphore semaphore, VkFence fence)
{
  uint32_t image_index = 0;
  vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, semaphore, fence, &image_index);

  return image_index;
}

void VulkanSwapchain::Present(uint32_t image_index, VkQueue queue, VkSemaphore semaphore)
{
  VkPresentInfoKHR present_info = {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapchain_;
  present_info.pImageIndices = &image_index;

  vkQueuePresentKHR(queue, &present_info);
}

} // namespace plex::graphics