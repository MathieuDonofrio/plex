#include <iostream>

#include "plex/debug/logging.h"
#include "plex/graphics/window.h"

using namespace plex;

void SetupCallbacks(Window* window)
{
  WindowEventCallback<WindowResizeEvent> resize_event_callback;
  resize_event_callback.Bind([](const WindowResizeEvent& event)
    { std::cout << "Window resize callback: " << event.width << "x" << event.height << std::endl; });
  window->AddWindowResizeEventCallback(resize_event_callback);

  WindowEventCallback<WindowCloseEvent> close_event_callback;
  close_event_callback.Bind([](const WindowCloseEvent&) { std::cout << "Window close callback" << std::endl; });
  window->AddWindowCloseEventCallback(close_event_callback);

  WindowEventCallback<WindowFocusEvent> focus_event_callback;
  focus_event_callback.Bind(
    [](const WindowFocusEvent& event)
    {
      std::cout << "Window focus callback: "
                << ((event.state == WindowFocusEvent::FocusState::Gained) ? "Gained" : "Lost") << std::endl;
    });
  window->AddWindowFocusEventCallback(focus_event_callback);

  WindowEventCallback<WindowMaximizeEvent> maximize_event_callback;
  maximize_event_callback.Bind([](const WindowMaximizeEvent& event)
    { std::cout << "Window maximise callback: " << event.maximized << std::endl; });
  window->AddWindowMaximizeEventCallback(maximize_event_callback);

  WindowEventCallback<WindowIconifyEvent> iconify_event_callback;
  iconify_event_callback.Bind(
    [](const WindowIconifyEvent& event) { std::cout << "Window iconify callback: " << event.iconified << std::endl; });
  window->AddWindowIconifyEventCallback(iconify_event_callback);

  WindowEventCallback<WindowKeyboardEvent> keyboard_event_callback;
  keyboard_event_callback.Bind(
    [](const WindowKeyboardEvent& event)
    {
      std::cout << "Keyboard callback: " << KeyCodeToString(event.keycode) << ", "
                << static_cast<uint32_t>(event.modifiers) << ", " << static_cast<uint32_t>(event.action) << std::endl;
    });
  window->AddWindowKeyboardEventCallback(keyboard_event_callback);

  WindowEventCallback<WindowCursorMoveEvent> cursor_move_event_callback;
  cursor_move_event_callback.Bind([](const WindowCursorMoveEvent& event)
    { std::cout << "Cursor move callback: x=" << event.pos_x << " y=" << event.pos_y << std::endl; });
  window->AddWindowCursorMoveEventCallback(cursor_move_event_callback);

  WindowEventCallback<WindowCursorEnterEvent> cursor_enter_event_callback;
  cursor_enter_event_callback.Bind([](const WindowCursorEnterEvent& event)
    { std::cout << "Cursor enter/leave callback: " << static_cast<uint32_t>(event.cursor_hover_state) << std::endl; });
  window->AddWindowCursorEnterEventCallback(cursor_enter_event_callback);

  WindowEventCallback<WindowMouseButtonEvent> mouse_button_event_callback;
  mouse_button_event_callback.Bind(
    [](const WindowMouseButtonEvent& event)
    {
      std::cout << "Mouse button callback: " << static_cast<uint32_t>(event.action) << ", "
                << static_cast<uint32_t>(event.button) << ", Mods: " << static_cast<uint32_t>(event.modifiers)
                << std::endl;
    });
  window->AddWindowMouseButtonEventCallback(mouse_button_event_callback);

  WindowEventCallback<WindowMouseScrollEvent> mouse_scroll_event_callback;
  mouse_scroll_event_callback.Bind([](const WindowMouseScrollEvent& event)
    { std::cout << "Mouse scroll callback: offset: " << event.vertical_offset << std::endl; });
  window->AddWindowMouseScrollEventCallback(mouse_scroll_event_callback);

  WindowEventCallback<WindowFramebufferResizeEvent> framebuffer_resize_event_callback;
  framebuffer_resize_event_callback.Bind([](const WindowFramebufferResizeEvent& event)
    { std::cout << "Window framebuffer resize callback: " << event.width << "x" << event.height << std::endl; });
  window->AddWindowFramebufferResizeEventCallback(framebuffer_resize_event_callback);
}

int main(int, char**)
{
  // Create Window

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  std::unique_ptr<Window> window = CreateWindow("Hello world", 512, 512, hints);

  SetupCallbacks(window.get());

  // Loop

  while (!window->IsClosing())
  {
    window->WaitEvents(0.5);
    window->PollEvents();
  }

  return 0;
}