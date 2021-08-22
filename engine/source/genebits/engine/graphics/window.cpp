
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

// TODO add generic to string to remove redundant code
std::string ButtonEvent::ModifierKeysToString() const
{
  const std::array<std::string, 6> key_names { "Shift", "Control", "Alt", "Super", "CapsLock", "NumLock" };
  std::string result;

  for (uint32_t i = 0; i < key_names.size(); ++i)
  {
    if (static_cast<bool>(modifiers & (1 << i)))
    {
      if (result.size() != 0) { result.append(", "); }
      result.append(key_names[i]);
    }
  }
  return result;
}

std::string ButtonEvent::ButtonActionToString() const
{
  const std::array<std::string, 3> key_names { "Released", "Pressed", "Repeated" };
  return key_names[static_cast<std::uint32_t>(action)];
}

std::string WindowMouseButtonEvent::CursorButtonToString() const
{
  const std::array<std::string, 3> key_names { "Left", "Right", "Middle" };
  return key_names[static_cast<std::uint32_t>(button)];
}

std::string WindowCursorEnterEvent::CursorHoverStateToString() const
{
  const std::array<std::string, 2> key_names { "Left", "Entered" };
  return key_names[static_cast<std::uint32_t>(cursor_hover_state)];
}

std::string WindowKeyboardEvent::KeyCodeToString() const
{
  return ::genebits::engine::KeyCodeToString(keycode);
}
} // namespace genebits::engine