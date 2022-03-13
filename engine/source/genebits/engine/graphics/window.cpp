#include "genebits/engine/graphics/window.h"

#include "genebits/engine/graphics/glfw/glfw_window.h"

namespace genebits::engine
{
std::shared_ptr<Window> CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, EventBus* bus, WindowCreationHints hints)
{
  return std::make_shared<GLFWWindow>(title, width, height, bus, hints);
}
} // namespace genebits::engine