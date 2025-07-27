#include "plex/graphics/window.h"

#include "plex/graphics/glfw/glfw_window.h"

namespace plex
{
std::unique_ptr<Window> CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints)
{
  return std::make_unique<GLFWWindow>(title, width, height, hints);
}
} // namespace plex