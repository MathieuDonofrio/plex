
#include <engine/events/listener.h>
#include <engine/graphics/glfw_window.h>
#include <iostream>

namespace genebits::sandbox
{
using namespace genebits::engine;

struct TestWindowListener : public Listener<TestWindowListener,
                              WindowCloseEvent,
                              WindowFocusEvent,
                              WindowMaximiseEvent,
                              WindowIconifyEvent,
                              WindowResizeEvent,
                              WindowRestoreEvent>
{
  TestWindowListener()
    : Listener<TestWindowListener,
      WindowCloseEvent,
      WindowFocusEvent,
      WindowMaximiseEvent,
      WindowIconifyEvent,
      WindowResizeEvent,
      WindowRestoreEvent>() {};

  void listen(const WindowCloseEvent&)
  {
    std::cout << "window close event" << std::endl;
  }

  void listen(const WindowFocusEvent& event)
  {
    std::cout << "window focus event: " << ((event.state == WindowFocusEventState::Gained) ? "Gained" : "Lost")
              << std::endl;
  }

  void listen(const WindowMaximiseEvent&)
  {
    std::cout << "window maximise event" << std::endl;
  }

  void listen(const WindowIconifyEvent&)
  {
    std::cout << "window iconify event" << std::endl;
  }

  void listen(const WindowResizeEvent& event)
  {
    std::cout << "window resize event: " << event.width << ", " << event.height << std::endl;
  }

  void listen(const WindowRestoreEvent& event)
  {
    std::cout << &event; // Prevent unused parameter warning treated as an error
    std::cout << "window restore event" << std::endl;
  }
};

int run()
{
  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;
  auto* window = new GLFWWindow("Hello world", 256, 256, hints);

  TestWindowListener listener {};

  unsigned long long i = 0;
  while (i < 600 && !window->IsClosing())
  {
    window->WaitEvents(0.5);
    window->PollEvents();
    i++;
  }

  return 0;
}
} // namespace genebits::sandbox

int main(int, char**)
{
  return genebits::sandbox::run();
}