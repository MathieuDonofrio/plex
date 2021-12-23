#include "graphic_instance_factory.h"
#include "genebits/engine/graphics/vulkan/vulkan_instance.h"

namespace genebits::engine
{

std::unique_ptr<GraphicInstance> CreateInstance(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend)
{
  std::unique_ptr<GraphicInstance> instance;

  switch (renderingBackend)
  {
  case RenderingBackend::VULKAN:
    instance =
      std::make_unique<VulkanInstance>(window_handle, application_name, is_debug, debug_message_severity_threshold);
    break;
  }

  return instance;
}
} // namespace genebits::engine
