
#include <engine/graphics/window.h>

int main(int, char**)
{
  using namespace genebits::engine;

  constexpr WindowCreationHints hints = WindowCreationHints::Maximised | WindowCreationHints::Resizable | WindowCreationHints::FocusingOnShow | WindowCreationHints::AutoIconified | WindowCreationHints::Decorated;
  auto* window = new Window("Hello world", 256, 256, hints);

  window->Create();

  window->SetWindowClosingCallback([](Window* window_ptr)
    { window_ptr->Close(); });

  unsigned long long i = 0;
  while (i < (UINT32_MAX >> 1) - 1)
  {
    i++;
  }

  return 0;
}
