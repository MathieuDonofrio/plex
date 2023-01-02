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

  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
  {
    for (const auto& available_format : available_formats)
    {
      if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM // SRGB difference ?
          && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return available_format;
      }
    }

    return available_formats[0];
  }

  VkPresentModeKHR ChoosePresentMode(
    const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR desired_present_mode)
  {
    if (std::find(available_present_modes.begin(), available_present_modes.end(), desired_present_mode)
        != available_present_modes.end())
    {
      return desired_present_mode;
    }

    return VK_PRESENT_MODE_FIFO_KHR; // Guaranteed to be available
  }

  VkExtent2D ChooseSwapExtent(Window* window, const VkSurfaceCapabilitiesKHR& capabilities)
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
      return capabilities.currentExtent;
    }
    else
    {
      auto [width, height] = window->GetFrameBufferSize();

      VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

      actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }

  void WaitUntilNotMinimized(Window* window)
  {
    while (true)
    {
      auto [width, height] = window->GetFrameBufferSize();

      if (width > 0 && height > 0)
      {
        break;
      }

      window->WaitEvents();
    }
  }
} // namespace

VulkanSwapchain::VulkanSwapchain(
  Window* window, VulkanDevice* device, VulkanSurface* surface, PresentMode present_mode, uint32_t image_count)
  : window_(window), swapchain_(nullptr), device_(device), surface_(surface), image_count_(image_count),
    desired_present_mode_(FromPresentMode(present_mode))
{
  CreateSwapchain();
  CreateImageViews();
}

VulkanSwapchain::~VulkanSwapchain()
{
  CleanupSwapchain();
}

void VulkanSwapchain::CreateSwapchain()
{
  std::vector<VkSurfaceFormatKHR> surface_formats = surface_->GetSurfaceFormats(device_->GetPhysicalHandle());

  surface_format_ = ChooseSwapSurfaceFormat(surface_formats);

  std::vector<VkPresentModeKHR> present_modes = surface_->GetPresentModes(device_->GetPhysicalHandle());

  present_mode_ = ChoosePresentMode(present_modes, desired_present_mode_);

  surface_capabilities_ = surface_->GetSurfaceCapabilities(device_->GetPhysicalHandle());

  queue_family_indices_[0] = device_->GetGraphicsQueueFamilyIndex();
  queue_family_indices_[1] = device_->GetPresentQueueFamilyIndex();

  extent_ = ChooseSwapExtent(window_, surface_capabilities_);

  LOG_INFO("Swapchain extent: {}x{}", extent_.width, extent_.height);

  if (surface_capabilities_.maxImageCount > 0 && image_count_ > surface_capabilities_.maxImageCount)
  {
    image_count_ = surface_capabilities_.maxImageCount;

    LOG_WARN("Image count exceeds maximum, clamping to {}", image_count_);
  }

  VkSwapchainCreateInfoKHR swapchain_create_info = {};
  swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_create_info.surface = surface_->GetHandle();
  swapchain_create_info.minImageCount = image_count_;
  swapchain_create_info.imageFormat = surface_format_.format;
  swapchain_create_info.imageColorSpace = surface_format_.colorSpace;
  swapchain_create_info.imageExtent = extent_;
  swapchain_create_info.imageArrayLayers = 1;
  swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if (queue_family_indices_[0] != queue_family_indices_[1])
  {
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.pQueueFamilyIndices = queue_family_indices_;
  }
  else
  {
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0; // Optional
    swapchain_create_info.pQueueFamilyIndices = nullptr; // Optional
  }

  swapchain_create_info.preTransform = surface_capabilities_.currentTransform;
  swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchain_create_info.presentMode = present_mode_;
  swapchain_create_info.clipped = VK_TRUE;
  swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device_->GetHandle(), &swapchain_create_info, nullptr, &swapchain_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create swapchain");
  }
}

void VulkanSwapchain::CreateImageViews()
{
  vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_, &image_count_, nullptr);
  images_.resize(image_count_);
  vkGetSwapchainImagesKHR(device_->GetHandle(), swapchain_, &image_count_, images_.data());

  image_views_.resize(image_count_);

  for (size_t i = 0; i < image_count_; i++)
  {
    VkImageViewCreateInfo image_view_create_info = {};
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.image = images_[i];
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.format = surface_format_.format;
    image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = 1;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device_->GetHandle(), &image_view_create_info, nullptr, &image_views_[i]) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create image view");
    }
  }
}

void VulkanSwapchain::CreateFramebuffers()
{
  framebuffers_.resize(image_views_.size());

  for (size_t i = 0; i < image_views_.size(); i++)
  {
    VkImageView attachments[] = { image_views_[i] };

    VkFramebufferCreateInfo framebuffer_create_info = {};
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.renderPass = render_pass_;
    framebuffer_create_info.attachmentCount = 1;
    framebuffer_create_info.pAttachments = attachments;
    framebuffer_create_info.width = extent_.width;
    framebuffer_create_info.height = extent_.height;
    framebuffer_create_info.layers = 1;

    vkCreateFramebuffer(device_->GetHandle(), &framebuffer_create_info, nullptr, &framebuffers_[i]);
  }
}

void VulkanSwapchain::CleanupSwapchain()
{
  vkDestroySwapchainKHR(device_->GetHandle(), swapchain_, nullptr);

  images_.clear();

  for (auto image_view : image_views_)
  {
    vkDestroyImageView(device_->GetHandle(), image_view, nullptr);
  }

  image_views_.clear();

  for (auto frame_buffer : framebuffers_)
  {
    vkDestroyFramebuffer(device_->GetHandle(), frame_buffer, nullptr);
  }

  framebuffers_.clear();
}

uint32_t VulkanSwapchain::AquireNextImage(VkSemaphore semaphore)
{
  uint32_t image_index = 0;
  VkResult result =
    vkAcquireNextImageKHR(device_->GetHandle(), swapchain_, UINT64_MAX, semaphore, VK_NULL_HANDLE, &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    Recreate();
    return UINT32_MAX;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    LOG_ERROR("Failed to acquire swapchain image");
  }

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

  VkResult result = vkQueuePresentKHR(queue, &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized_)
  {
    framebuffer_resized_ = false;
    Recreate();
  }
  else if (result != VK_SUCCESS)
  {
    LOG_ERROR("Failed to present swapchain image");
  }
}

void VulkanSwapchain::Recreate()
{
  WaitUntilNotMinimized(window_);

  vkDeviceWaitIdle(device_->GetHandle());

  CleanupSwapchain();

  CreateSwapchain();
  CreateImageViews();
  CreateFramebuffers();
}

} // namespace plex::graphics