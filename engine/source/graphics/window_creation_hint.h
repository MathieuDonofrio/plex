#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_CREATION_HINT_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_CREATION_HINT_H

namespace genebits::engine
{

struct WindowCreationHints
{
  bool is_resizable_ = true;
  bool is_visible_ = true;
  bool is_decorated_ = true;
  bool is_focused_ = true;
  bool is_auto_iconified = true;
  bool is_focusing_on_show = true;
  bool is_floating = false;
  bool is_maximized = false;
  bool is_cursor_centered = false;
  bool is_using_transparent_framebuffer = false;
  bool is_scaling_to_monitor = false;

  uint32_t fullscreen_refresh_rate = -1; // Max available refresh rate
};

} // namespace genebits::engine
#endif
