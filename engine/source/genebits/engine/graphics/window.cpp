
#include "genebits/engine/graphics/window.h"
#include "genebits/engine/graphics/glfw_window.h"

#include <array>

namespace genebits::engine
{
Window* CreateWindow(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints)
{
  return static_cast<Window*>(new GLFWWindow(title, width, height, hints));
}

std::string ModifierKeysToString(const ModifierKeys modifier_key)
{
  const std::array<std::string, 6> key_names { "Shift", "Control", "Alt", "Super", "CapsLock", "NumLock" };
  std::string result;

  for (uint32_t i = 0; i < key_names.size(); ++i)
  {
    if (static_cast<bool>(modifier_key & (1 << i)))
    {
      if (result.size() != 0) { result.append(", "); }
      result.append(key_names[i]);
    }
  }
  return result;
}
} // namespace genebits::engine