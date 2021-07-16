#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOWBUILDER_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOWBUILDER_H

#include <cstdint>
#include <string>

#include "window.h"

namespace genebits::engine
{

class WindowBuilder
{
public:
  WindowBuilder(const std::string& title, uint32_t width, uint32_t height);

  Window* Build();

  WindowBuilder* Resizable(bool resizable);

  WindowBuilder* Visible(bool visible);

  WindowBuilder* Decorated(bool decorated);

  WindowBuilder* AutoIconified(bool auto_iconified);

  WindowBuilder* FocusOnShow(bool focus_on_show);

  WindowBuilder* Floating(bool floating);

  WindowBuilder* Maximized(bool maximized);

  WindowBuilder* CursorCentered(bool cursor_centered);

  WindowBuilder* UseTransparentFrameBuffer(bool use_transparent_frame_buffer);

  WindowBuilder* ScaleToMonitor(bool scale_to_monitor);

  WindowBuilder* FullscreenRefreshRate(uint32_t fullscreen_refresh_rate);

private:
  struct Pimpl;
  Pimpl* pimpl_;
};

} // namespace genebits::engine
#endif
