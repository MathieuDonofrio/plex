#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SWAPCHAIN_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SWAPCHAIN_H

#include "plex/graphics/renderer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_frame.h"
#include "plex/graphics/vulkan/vulkan_image.h"

namespace plex::graphics
{
class VulkanSwapchain
{
public:
  VulkanSwapchain(VulkanDevice* device,
    VulkanSurface* surface,
    PresentMode present_mode,
    uint32_t width,
    uint32_t height,
    uint32_t image_count);

  ~VulkanSwapchain();

  VulkanSwapchain(const VulkanSwapchain&) = delete;
  VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
  VulkanSwapchain(VulkanSwapchain&&) = delete;
  VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

  [[nodiscard]] uint32_t AquireNextImage(VkSemaphore semaphore, VkFence fence);

  void Present(uint32_t image_index, VkQueue queue, VkSemaphore semaphore);

  [[nodiscard]] VkImage GetImage(uint32_t index) const noexcept
  {
    return images_[index];
  }

  [[nodiscard]] VkImageView GetImageView(uint32_t index) const noexcept
  {
    return image_views_[index];
  }

  [[nodiscard]] uint32_t GetImageCount() const noexcept
  {
    return image_count_;
  }

  [[nodiscard]] VkSwapchainKHR GetHandle() const noexcept
  {
    return swapchain_;
  }

private:
  VkSwapchainKHR swapchain_;
  VkDevice device_;

  VkSurfaceFormatKHR surface_format_;
  PresentMode present_mode_;
  VkExtent2D extent_;
  uint32_t image_count_;

  std::vector<VkImage> images_;
  std::vector<VkImageView> image_views_;
};
} // namespace plex::graphics

#endif