#include <iostream>

#include "genebits/engine/events/listener.h"
#include "genebits/engine/graphics/window.h"

using namespace genebits::engine;

struct TestWindowListener : public Listener<TestWindowListener,
                              WindowCloseEvent,
                              WindowFocusEvent,
                              WindowMaximizeEvent,
                              WindowIconifyEvent,
                              WindowResizeEvent>
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
