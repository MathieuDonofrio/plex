#include <iostream>

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/events/listener.h"
#include "genebits/engine/graphics/window.h"

using namespace genebits::engine;

struct TestWindowListener : public Listener<TestWindowListener,
                              WindowCloseEvent,
                              WindowFocusEvent,
                              WindowMaximizeEvent,
                              WindowIconifyEvent,
                              WindowResizeEvent,
                              WindowKeyboardEvent,
                              WindowCursorEnterEvent,
                              WindowCursorMoveEvent,
                              WindowMouseButtonEvent,
                              WindowMouseScrollEvent>
{
  void Listen(const WindowCloseEvent&)
  {
    std::cout << "window close event" << std::endl;
  }

  void Listen(const WindowFocusEvent& event)
  {
    std::cout << "window focus event: " << ((event.state == WindowFocusEvent::FocusState::Gained) ? "Gained" : "Lost")
              << std::endl;
  }

  void Listen(const WindowMaximizeEvent& event)
  {
    std::cout << "window maximise event: " << event.maximized << std::endl;
  }

  void Listen(const WindowIconifyEvent& event)
  {
    std::cout << "window iconify event: " << event.iconified << std::endl;
  }

  void Listen(const WindowResizeEvent& event)
  {
    std::cout << "window resize event: " << event.width << ", " << event.height << std::endl;
  }

  void Listen(const WindowKeyboardEvent& event)
  {
    std::cout << "keyboard event: " << KeyCodeToString(event.keycode) << ", " << static_cast<uint32_t>(event.modifiers)
              << ", " << static_cast<uint32_t>(event.action) << std::endl;

    if (event.keycode == genebits::engine::W && event.action == genebits::engine::ButtonEvent::Action::Pressed)
    {
      std::cout << "Window monitor query: " << event.window->GetMonitorWidth() << "x"
                << event.window->GetMonitorHeight() << std::endl;
    }
  }

  void Listen(const WindowCursorMoveEvent& event)
  {
    std::cout << "Cursor move event: x=" << event.pos_x << " y=" << event.pos_y << std::endl;
  }

  void Listen(const WindowCursorEnterEvent& event)
  {
    std::cout << "Cursor enter/leave event: " << static_cast<uint32_t>(event.cursor_hover_state) << std::endl;
  }

  void Listen(const WindowMouseButtonEvent& event)
  {
    std::cout << "Mouse button event: " << static_cast<uint32_t>(event.action) << ", "
              << static_cast<uint32_t>(event.button) << ", Mods: " << static_cast<uint32_t>(event.modifiers)
              << std::endl;
  }

  void Listen(const WindowMouseScrollEvent& event)
  {
    std::cout << "Mouse scroll event: offset: " << event.vertical_offset << std::endl;
  }
};

int main(int, char**)
{
  // Setup listener

  TestWindowListener listener {};

  // Create Window

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  Window* window = CreateWindow("Hello world", 256, 256, hints);

  // Window loop

  while (!window->IsClosing())
  {
    window->WaitEvents(0.5);
    window->PollEvents();
  }

  // Cleanup

  delete window;

  return 0;
}
