#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SWAPCHAIN_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SWAPCHAIN_H

#include "plex/graphics/renderer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"
#include "plex/graphics/vulkan/vulkan_surface.h"

namespace plex::graphics
{
class VulkanSwapchain
{
public:
  VulkanSwapchain(
    Window* window, VulkanDevice* device, VulkanSurface* surface, PresentMode present_mode, uint32_t image_count);

  ~VulkanSwapchain();

  VulkanSwapchain(const VulkanSwapchain&) = delete;
  VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
  VulkanSwapchain(VulkanSwapchain&&) = delete;
  VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

  [[nodiscard]] uint32_t AquireNextImage(VkSemaphore semaphore);

  void Present(uint32_t image_index, VkQueue queue, VkSemaphore semaphore);

  void Recreate();

  void InitFramebuffers(VkRenderPass render_pass)
  {
    render_pass_ = render_pass;
    CreateFramebuffers();
  }

  [[nodiscard]] VkImage GetImage(uint32_t index) const noexcept
  {
    return images_[index];
  }

  [[nodiscard]] VkImageView GetImageView(uint32_t index) const noexcept
  {
    return image_views_[index];
  }

  [[nodiscard]] VkFramebuffer GetFramebuffer(uint32_t index) const noexcept
  {
    return framebuffers_[index];
  }

  void FlagFramebufferResized()
  {
    framebuffer_resized_ = true;
  }

  [[nodiscard]] VkSurfaceFormatKHR GetSurfaceFormat() const noexcept
  {
    return surface_format_;
  }

  [[nodiscard]] VkExtent2D GetExtent() const noexcept
  {
    return extent_;
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
  void CreateSwapchain();
  void CreateImageViews();
  void CreateFramebuffers();

  void CleanupSwapchain();

private:
  VkSwapchainKHR swapchain_;

  Window* window_;
  VulkanDevice* device_;
  VulkanSurface* surface_;

  VkSurfaceFormatKHR surface_format_;
  VkPresentModeKHR desired_present_mode_;
  VkPresentModeKHR present_mode_;
  VkSurfaceCapabilitiesKHR surface_capabilities_;
  VkExtent2D extent_;
  VkRenderPass render_pass_;
  uint32_t queue_family_indices_[2];
  uint32_t image_count_;
  bool framebuffer_resized_;

  std::vector<VkImage> images_;
  std::vector<VkImageView> image_views_;
  std::vector<VkFramebuffer> framebuffers_;
};
} // namespace plex::graphics

#endif