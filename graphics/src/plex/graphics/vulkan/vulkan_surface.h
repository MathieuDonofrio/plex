#ifndef PLEX_GRAPHICS_VULKAN_SURFACE_H
#define PLEX_GRAPHICS_VULKAN_SURFACE_H

#include <memory>

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_instance.h"

namespace plex
{

class VulkanSurface
{
public:
  VulkanSurface(std::shared_ptr<Window> window, std::shared_ptr<VulkanInstance> instance) : instance_(instance)
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
    vkDestroySurfaceKHR(instance_->GetHandle(), surface_handle_, nullptr);
    surface_handle_ = VK_NULL_HANDLE;

    LOG_INFO("Vulkan surface destroyed");
  };

  [[nodiscard]] const VkSurfaceKHR GetHandle() const noexcept
  {
    return surface_handle_;
  }

private:
  bool Initialize(VulkanCapableWindow* window)
  {
    surface_handle_ = window->CreateWindowSurface(instance_->GetHandle());

    return surface_handle_ != VK_NULL_HANDLE;
  };

private:
  VkSurfaceKHR surface_handle_;

  std::shared_ptr<VulkanInstance> instance_;
};

} // namespace plex
#endif
