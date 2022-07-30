#include "plex/graphics/renderer.h"

#include "plex/graphics/vulkan/vulkan_renderer.h"

namespace plex
{

std::shared_ptr<VulkanRenderer> CreateVulkanRenderer(
  std::shared_ptr<Window> window, const std::string& application_name, GraphicsDebugLevel debug_level)
{
  if (dynamic_cast<VulkanCapableWindow*>(window.get()) == nullptr)
  {
    LOG_ERROR("Window is not vulkan capable");

    return nullptr;
  }

  return std::make_shared<VulkanRenderer>(window, application_name, debug_level);
}

std::shared_ptr<Renderer> CreateRenderer(std::shared_ptr<Window> window,
  const std::string& application_name,
  GraphicsDebugLevel debug_level,
  RenderingBackend renderingBackend)
{
  switch (renderingBackend)
  {
  case RenderingBackend::VULKAN: return CreateVulkanRenderer(window, application_name, debug_level);
  default: return nullptr;
  }
}

} // namespace plex
