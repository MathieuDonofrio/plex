#include <iostream>

#include "plex/debug/logging.h"
#include "plex/events/listener.h"
#include "plex/graphics/window.h"

using namespace plex;

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
                              WindowMouseScrollEvent,
                              WindowFramebufferResizeEvent>
{
  TestWindowListener(EventBus& bus) : Listener(bus) {}

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

    if (event.keycode == plex::W && event.action == plex::ButtonEvent::Action::Pressed)
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

  void Listen(const WindowFramebufferResizeEvent& event)
  {
    std::cout << "Window framebuffer resize event: " << event.width << "x" << event.height << std::endl;
  }
};

int main(int, char**)
{
  // Setup event bus

  EventBus bus;

  // Setup listener

  TestWindowListener listener { bus };

  // Create Window

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  Window* window = CreateWindow("Hello world", 256, 256, &bus, hints);

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
