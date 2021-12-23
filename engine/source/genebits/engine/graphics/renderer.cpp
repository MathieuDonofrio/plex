#include "genebits/engine/graphics/renderer.h"

#include "genebits/engine/graphics/graphic_instance_factory.h"
#include "genebits/engine/graphics/vulkan/vulkan_instance.h"

#include <memory>

namespace genebits::engine
{

struct Renderer::Pimpl
{
  std::shared_ptr<GraphicInstance> graphic_instance_;
};

// TODO free function
Renderer::Renderer(Window* window_handle,
  const char* application_name,
  bool is_debug,
  GraphicsDebugLevel debug_message_severity_threshold,
  RenderingBackend renderingBackend)
  : window_handle_(window_handle)
{
  pimpl_ = new Pimpl {};

  pimpl_->graphic_instance_ =
    CreateInstance(window_handle, application_name, is_debug, debug_message_severity_threshold, renderingBackend);
}

Renderer::~Renderer()
{
  delete pimpl_;
}

} // namespace genebits::engine
