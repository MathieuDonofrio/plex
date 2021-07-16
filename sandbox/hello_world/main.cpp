
#include "engine/graphics/window_builder.h"

int main(int, char**)
{
  using namespace genebits::engine;

  auto* window_builder = new WindowBuilder("Hello World!", 800, 600);
  Window* window = window_builder->Decorated(true)->FocusOnShow(true)->Maximized(true)->Visible(true)->Build();

  window->Create();

  return 0;
}
