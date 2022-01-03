#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDERER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDERER_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_adapter_queries.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/vulkan_surface.h"
#include "genebits/engine/graphics/vulkan/vulkan_swapchain.h"

#include <memory>

namespace genebits::engine
{

class VulkanRenderer : public Renderer
{
public:
  VulkanRenderer(VulkanCapableWindow* window_handle,
    const char* application_name,
    bool use_debug_messenger,
    GraphicsDebugLevel debug_level)
  {
    if (!Initialize(window_handle, application_name, use_debug_messenger, debug_level))
    {
      LOG_ERROR("Failed to initialize vulkan renderer");
    }
    else
    {
      LOG_INFO("Vulkan renderer initialized");
    }
  }

  ~VulkanRenderer()
  {
    LOG_INFO("Vulkan renderer destroyed");
  }

private:
  std::shared_ptr<VkPhysicalDevice_T> adapter_; // Using shared_ptr for consistency TODO Mathieu's opinion
  std::shared_ptr<VulkanInstance> instance_;
  std::shared_ptr<VulkanSurface> surface_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSwapchain> swapchain_;

  const std::vector<std::string> required_extensions_ = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  bool Initialize(VulkanCapableWindow* window_handle,
    const char* application_name,
    bool use_debug_messenger,
    GraphicsDebugLevel debug_level)
  {
    instance_ = std::make_shared<VulkanInstance>(window_handle, application_name, use_debug_messenger, debug_level);
    if (!instance_.get()) { return false; }

    surface_ = std::make_shared<VulkanSurface>(instance_, window_handle);
    if (!surface_.get()) { return false; }

    adapter_ = std::shared_ptr<VkPhysicalDevice_T>(
      VulkanAdapterQueries::FindSuitableAdapter(instance_->GetHandle(), surface_->GetHandle(), required_extensions_),
      [](VkPhysicalDevice_T*) {});
    if (!adapter_.get()) { return false; }

    device_ = std::make_shared<VulkanDevice>(adapter_, surface_->GetHandle(), required_extensions_);
    if (!device_.get()) { return false; }

    auto [width, height] = window_handle->GetFrameBufferSize();
    swapchain_ = std::make_shared<VulkanSwapchain>(device_, surface_, adapter_.get(), width, height);
    if (!swapchain_.get()) { return false; }

    return true;
  }
};

} // namespace genebits::engine
#endif
