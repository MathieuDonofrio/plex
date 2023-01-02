#include "plex/graphics/renderer.h"

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_renderer.h"

namespace plex::graphics
{
std::unique_ptr<Renderer> CreateRenderer(
  [[maybe_unused]] const RendererCreateInfo& create_info, BackendType backend_type)
{
  switch (backend_type)
  {
  case BackendType::Vulkan: return std::make_unique<VulkanRenderer>(create_info);
  default: return nullptr;
  }
}

} // namespace plex::graphics
