
#include "engine/graphics/window_builder.h"

#include <GLFW/glfw3.h>

#include "window_creation_hint.h"

namespace genebits::engine
{

struct WindowBuilder::Pimpl
{
  WindowCreationHints window_creation_hint_;
  uint32_t width_;
  uint32_t height_;
  std::string title_;
};

WindowBuilder::WindowBuilder(const std::string& title, uint32_t width, uint32_t height)
  : pimpl_(new Pimpl)
{
  pimpl_->width_ = width;
  pimpl_->height_ = height;
  pimpl_->title_ = title;
}

Window* WindowBuilder::Build()
{
  //TODO Review: Should I just implement methods in window to set the window hints instead?
  auto* window_instance = new Window(pimpl_->title_, pimpl_->width_, pimpl_->height_);
  window_instance->SetWindowCreationHints<WindowCreationHints>(pimpl_->window_creation_hint_);
  return window_instance;
}

WindowBuilder* WindowBuilder::AutoIconified(bool auto_iconified)
{
  pimpl_->window_creation_hint_.is_auto_iconified = auto_iconified;
  return this;
}

WindowBuilder* WindowBuilder::Resizable(bool resizable)
{
  pimpl_->window_creation_hint_.is_resizable_ = resizable;
  return this;
}

WindowBuilder* WindowBuilder::Visible(bool visible)
{
  pimpl_->window_creation_hint_.is_visible_ = visible;
  return this;
}

WindowBuilder* WindowBuilder::Decorated(bool decorated)
{
  pimpl_->window_creation_hint_.is_decorated_ = decorated;
  return this;
}

WindowBuilder* WindowBuilder::FocusOnShow(bool focus_on_show)
{
  pimpl_->window_creation_hint_.is_focusing_on_show = focus_on_show;
  return this;
}

WindowBuilder* WindowBuilder::Floating(bool floating)
{
  pimpl_->window_creation_hint_.is_floating = floating;
  return this;
}

WindowBuilder* WindowBuilder::Maximized(bool maximized)
{
  pimpl_->window_creation_hint_.is_maximized = maximized;
  return this;
}

WindowBuilder* WindowBuilder::CursorCentered(bool cursor_centered)
{
  pimpl_->window_creation_hint_.is_cursor_centered = cursor_centered;
  return this;
}

WindowBuilder* WindowBuilder::UseTransparentFrameBuffer(bool use_transparent_frame_buffer)
{
  pimpl_->window_creation_hint_.is_using_transparent_framebuffer = use_transparent_frame_buffer;
  return this;
}

WindowBuilder* WindowBuilder::ScaleToMonitor(bool scale_to_monitor)
{
  pimpl_->window_creation_hint_.is_scaling_to_monitor = scale_to_monitor;
  return this;
}

WindowBuilder* WindowBuilder::FullscreenRefreshRate(uint32_t fullscreen_refresh_rate)
{
  pimpl_->window_creation_hint_.fullscreen_refresh_rate = fullscreen_refresh_rate;
  return this;
}

} // namespace genebits::engine
