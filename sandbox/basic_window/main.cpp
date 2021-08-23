#include <iostream>

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
  void listen(const WindowCloseEvent&)
  {
    std::cout << "window close event" << std::endl;
  }

  void listen(const WindowFocusEvent& event)
  {
    std::cout << "window focus event: " << ((event.state == WindowFocusEvent::FocusState::Gained) ? "Gained" : "Lost")
              << std::endl;
  }

  void listen(const WindowMaximizeEvent& event)
  {
    std::cout << "window maximise event: " << event.maximized << std::endl;
  }

  void listen(const WindowIconifyEvent& event)
  {
    std::cout << "window iconify event: " << event.iconified << std::endl;
  }

  void listen(const WindowResizeEvent& event)
  {
    std::cout << "window resize event: " << event.width << ", " << event.height << std::endl;
  }

  void listen(const WindowKeyboardEvent& event)
  {
    std::cout << "keyboard event: " << event.KeyCodeToString() << ", " << event.ModifierKeysToString() << ", "
              << event.ButtonActionToString() << std::endl;
  }

  void listen(const WindowCursorMoveEvent& event)
  {
    std::cout << "Cursor move event: x=" << event.x_pos << " y=" << event.y_pos << std::endl;
  }

  void listen(const WindowCursorEnterEvent& event)
  {
    std::cout << "Cursor enter/leave event: " << event.CursorHoverStateToString() << std::endl;
  }

  void listen(const WindowMouseButtonEvent& event)
  {
    std::cout << "Mouse button event: " << event.ButtonActionToString() << ", " << event.CursorButtonToString()
              << ", Mods: " << event.ModifierKeysToString() << std::endl;
  }

  void listen(const WindowMouseScrollEvent& event)
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
