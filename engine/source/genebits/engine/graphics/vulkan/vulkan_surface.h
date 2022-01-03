#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SURFACE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SURFACE_H

#include "vulkan_instance.h"

#include <genebits/engine/debug/logging.h>
#include <memory>

namespace genebits::engine
{

class VulkanSurface
{
public:
  VulkanSurface(std::shared_ptr<VulkanInstance> instance, VulkanCapableWindow* window_handle)
  {
    if (!Initialize(std::move(instance), window_handle)) { LOG_ERROR("Failed to initialize Vulkan surface"); }
    else
    {
      LOG_INFO("Vulkan surface initialized");
    }
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
  VkSurfaceKHR surface_handle_;
  std::shared_ptr<VulkanInstance> instance_;

  bool Initialize(std::shared_ptr<VulkanInstance> instance, VulkanCapableWindow* window)
  {
    instance_ = std::move(instance);

    surface_handle_ = window->CreateWindowSurface(instance_->GetHandle());

    return surface_handle_ != VK_NULL_HANDLE;
  };
};

} // namespace genebits::engine
#endif
