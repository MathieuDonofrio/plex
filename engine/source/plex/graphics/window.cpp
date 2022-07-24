#include "genebits/engine/graphics/window.h"

#include "genebits/engine/graphics/glfw_window.h"

namespace plex
{
Window* CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, EventBus* bus, WindowCreationHints hints)
{
  return static_cast<Window*>(new GLFWWindow(title, width, height, bus, hints));
}
} // namespace plex