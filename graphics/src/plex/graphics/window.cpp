#include "plex/graphics/window.h"

#include "plex/graphics/glfw/glfw_window.h"

namespace plex
{
std::shared_ptr<Window> CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, EventBus* bus, WindowCreationHints hints)
{
  return std::make_shared<GLFWWindow>(title, width, height, bus, hints);
}
} // namespace plex