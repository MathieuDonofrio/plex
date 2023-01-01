#include "plex/graphics/renderer.h"

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_renderer.h"

namespace plex::graphics
{
std::unique_ptr<Renderer> CreateVulkanRenderer(const RendererCreateInfo& create_info)
{
  auto* vulkan_capable_window = dynamic_cast<VulkanCapableWindow*>(create_info.window_handle);

  if (vulkan_capable_window == nullptr)
  {
    LOG_ERROR("Window is not Vulkan capable");
    return nullptr;
  }

  uint32_t width = create_info.window_handle->GetWidth();
  uint32_t height = create_info.window_handle->GetHeight();

  uint32_t image_count = create_info.buffering_mode == BufferingMode::Double ? 2 : 3;

  return std::make_unique<VulkanRenderer>(vulkan_capable_window,
    create_info.application_name,
    create_info.debug_level,
    create_info.present_mode,
    width,
    height,
    image_count);
}

std::unique_ptr<Renderer> CreateRenderer(
  [[maybe_unused]] const RendererCreateInfo& create_info, BackendType backend_type)
{
  switch (backend_type)
  {
  case BackendType::Vulkan: return CreateVulkanRenderer(create_info);

  default: return nullptr;
  }
}

} // namespace plex::graphics
