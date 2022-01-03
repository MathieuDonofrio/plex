#include "genebits/engine/graphics/renderer.h"

#include "genebits/engine/graphics/vulkan/vulkan_renderer.h"

namespace genebits::engine
{

VulkanRenderer* CreateVulkanRenderer(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold)
{
  auto* vulkan_capable_window_handle = dynamic_cast<VulkanCapableWindow*>(window_handle);
  if (!vulkan_capable_window_handle) { LOG_ERROR("Failed to cast window handle to vulkan capable window handle"); }

  return new VulkanRenderer(vulkan_capable_window_handle, application_name, is_debug, debug_message_severity_threshold);
}

Renderer* CreateRenderer(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend)
{
  if (renderingBackend == RenderingBackend::VULKAN)
  {
    return CreateVulkanRenderer(window_handle, application_name, is_debug, debug_message_severity_threshold);
  }

  return nullptr; // oops
}

} // namespace genebits::engine
