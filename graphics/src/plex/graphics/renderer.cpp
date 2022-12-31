#include "plex/graphics/renderer.h"

namespace plex::graphics
{

std::unique_ptr<Renderer> CreateRenderer(
  [[maybe_unused]] const RendererCreateInfo& create_info, BackendType backend_type)
{
  switch (backend_type)
  {
  case BackendType::Vulkan: return nullptr; // TODO
  default: return nullptr;
  }
}

} // namespace plex::graphics
