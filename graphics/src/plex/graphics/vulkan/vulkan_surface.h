#ifndef PLEX_GRAPHICS_VULKAN_SURFACE_H
#define PLEX_GRAPHICS_VULKAN_SURFACE_H

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan2/vulkan_api.h"

#include <memory>

namespace plex::graphics
{

class VulkanSurface
{
public:
  VulkanSurface(std::shared_ptr<Window> window)
  {
    if (!Initialize(dynamic_cast<VulkanCapableWindow*>(window.get())))
    {
      LOG_ERROR("Failed to initialize Vulkan surface");

      return;
    }

    LOG_INFO("Vulkan surface initialized");
  }

  VulkanSurface(const VulkanSurface&) = delete;
  VulkanSurface& operator=(const VulkanSurface&) = delete;
  VulkanSurface(VulkanSurface&&) = delete;
  VulkanSurface& operator=(VulkanSurface&&) = delete;

  ~VulkanSurface()
  {
    vkapi::vkDestroySurfaceKHR(surface_handle_, nullptr);
    surface_handle_ = VK_NULL_HANDLE;

    LOG_INFO("Vulkan surface destroyed");
  };

  [[nodiscard]] VkSurfaceKHR GetHandle() const noexcept
  {
    return surface_handle_;
  }

private:
  bool Initialize(VulkanCapableWindow* window)
  {
    surface_handle_ = window->CreateWindowSurface(vkapi::GetInstance());

    return surface_handle_ != VK_NULL_HANDLE;
  };

private:
  VkSurfaceKHR surface_handle_ { VK_NULL_HANDLE };
};

} // namespace plex::graphics
#endif
