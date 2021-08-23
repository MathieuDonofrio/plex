
#include "genebits/engine/graphics/window.h"
#include "genebits/engine/graphics/glfw_window.h"
#include "genebits/engine/graphics/key_codes.h"

#include <array>

namespace genebits::engine
{

Window* CreateWindow(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints)
{
  return static_cast<Window*>(new GLFWWindow(title, width, height, hints));
}

std::string WindowKeyboardEvent::KeyCodeToString() const
{
  return ::genebits::engine::KeyCodeToString(keycode);
}
} // namespace genebits::engine