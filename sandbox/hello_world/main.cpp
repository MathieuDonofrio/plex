
#include <engine/graphics/window.h>
#include <iostream>

int main(int, char**)
{
  using namespace genebits::engine;

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;
  auto* window = new Window("Hello world", 256, 256, hints);

  window->Create();

  window->SetWindowClosingCallback([](Window* window_ptr)
    {
      window_ptr->Close();
      std::cout << "closing" << std::endl;
    });

  unsigned long long i = 0;
  while (i < 300)
  {
    window->WaitEvents(0.5);
    i++;
  }

  window->Close();

  return 0;
}
