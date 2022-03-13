#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDERER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDERER_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer_group.h"
#include "genebits/engine/graphics/vulkan/vulkan_surface.h"
#include "genebits/engine/graphics/vulkan/vulkan_swapchain.h"

#include <memory>

namespace genebits::engine
{
// TODO window resize event -> resize buffers, swapchain, etc...
class VulkanRenderer : public Renderer
{
public:
  VulkanRenderer(std::shared_ptr<Window> window, const std::string& application_name, GraphicsDebugLevel debug_level)
    : window_(window)
  {
    if (dynamic_cast<VulkanCapableWindow*>(window.get()) == nullptr)
    {
      LOG_ERROR("Window is not vulkan capable");

      return;
    }

    if (!Initialize(application_name, debug_level))
    {
      LOG_ERROR("Failed to initialize vulkan renderer");

      return;
    }

    LOG_INFO("Vulkan renderer initialized");
  }

  ~VulkanRenderer()
  {
    LOG_INFO("Vulkan renderer destroyed");
  }

private:
  bool Initialize(const std::string& application_name, GraphicsDebugLevel debug_level)
  {
    instance_ = std::make_shared<VulkanInstance>(window_, application_name, debug_level);
    surface_ = std::make_shared<VulkanSurface>(window_, instance_);
    device_ = std::make_shared<VulkanDevice>(instance_, surface_);
    swapchain_ = std::make_shared<VulkanSwapchain>(window_, device_, surface_);

    return true;
  }

private:
  std::shared_ptr<Window> window_;

  std::shared_ptr<VulkanInstance> instance_;
  std::shared_ptr<VulkanSurface> surface_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSwapchain> swapchain_;
};

} // namespace genebits::engine
#endif
