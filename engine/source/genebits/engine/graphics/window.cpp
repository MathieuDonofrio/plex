#include "genebits/engine/graphics/window.h"

#include "genebits/engine/graphics/glfw/glfw_window.h"

namespace genebits::engine
{
Window* CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, EventBus* bus, WindowCreationHints hints)
{
  return static_cast<Window*>(new GLFWWindow(title, width, height, bus, hints));
}
} // namespace genebits::engine