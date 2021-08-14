
#include "glfw_window.h"

#include <algorithm>
#include <iostream>

namespace genebits::engine
{
GLFWWindow::GLFWWindow(
  const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  : title_(title)
{
  glfwInit(); // Assert this

  if (window_creation_hints == WindowCreationHints::Defaults) { glfwDefaultWindowHints(); }
  else
  {
    ApplyWindowCreationHints(window_creation_hints);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  handle_ = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);

  glfwSetWindowSizeLimits(handle_, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);

  glfwSetWindowUserPointer(handle_, this);

  RegisterGLFWWindowCallbacks();
}

GLFWWindow::~GLFWWindow()
{
  glfwDestroyWindow(handle_);
}

void GLFWWindow::PollEvents()
{
  glfwPollEvents();
}

void GLFWWindow::WaitEvents()
{
  glfwWaitEvents();
}

void GLFWWindow::WaitEvents(double timeout)
{
  glfwWaitEventsTimeout(timeout);
}

const std::string& GLFWWindow::GetTitle() const
{
  return title_;
}

void GLFWWindow::SetTitle(const std::string& title)
{
  title_ = title;

  glfwSetWindowTitle(handle_, title.c_str());
}

uint32_t GLFWWindow::GetWidth() const
{
  int width;

  glfwGetWindowSize(handle_, &width, nullptr);

  return static_cast<uint32_t>(width);
}

uint32_t GLFWWindow::GetHeight() const
{
  int height;

  glfwGetWindowSize(handle_, nullptr, &height);

  return static_cast<uint32_t>(height);
}

void GLFWWindow::Resize(uint32_t width, uint32_t height)
{
  glfwSetWindowSize(handle_, static_cast<int>(width), static_cast<int>(height));
}

void GLFWWindow::Focus()
{
  glfwFocusWindow(handle_);
}

void GLFWWindow::Maximize()
{
  glfwMaximizeWindow(handle_);
}

void GLFWWindow::Iconify()
{
  glfwIconifyWindow(handle_);
}

void GLFWWindow::Restore()
{
  glfwRestoreWindow(handle_);
}

void GLFWWindow::RequestAttention()
{
  glfwRequestWindowAttention(handle_);
}

void GLFWWindow::Close()
{
  // TODO Matt: Brainstorm ideas for closing protocol

  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});
  glfwSetWindowShouldClose(handle_, 1);
}

bool GLFWWindow::IsClosing() const
{
  return glfwWindowShouldClose(handle_) != 0;
}

void GLFWWindow::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
{
  // TODO Matt: Simplify

  // TODO add LOD icons (small, med, big)
  if (pixels != nullptr)
  {
    GLFWimage icon { static_cast<int>(width), static_cast<int>(height), pixels };
    glfwSetWindowIcon(handle_, 1, &icon);
  }
  else
  {
    glfwSetWindowIcon(handle_, 0, nullptr);
  }
}

uint32_t GLFWWindow::GetMonitorWidth() const
{
  GLFWmonitor* monitor_ptr = glfwGetPrimaryMonitor();
  return glfwGetVideoMode(monitor_ptr)->width;
}

uint32_t GLFWWindow::GetMonitorHeight() const
{
  GLFWmonitor* monitor_ptr = glfwGetPrimaryMonitor();
  return glfwGetVideoMode(monitor_ptr)->height;
}

bool GLFWWindow::IsIconified() const
{
  return static_cast<bool>(glfwGetWindowAttrib(handle_, GLFW_ICONIFIED));
}

bool GLFWWindow::IsMaximised() const
{
  return static_cast<bool>(glfwGetWindowAttrib(handle_, GLFW_MAXIMIZED));
}

bool GLFWWindow::IsFocused() const
{
  return static_cast<bool>(glfwGetWindowAttrib(handle_, GLFW_FOCUSED));
}

bool GLFWWindow::IsVisible() const
{
  return static_cast<bool>(glfwGetWindowAttrib(handle_, GLFW_VISIBLE));
}

void GLFWWindow::SetFullScreenRefreshRate(uint64_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
}

void GLFWWindow::ApplyWindowCreationHints(const WindowCreationHints& hints)
{
  glfwWindowHint(GLFW_RESIZABLE, (hints & WindowCreationHints::Resizable) != 0);
  glfwWindowHint(GLFW_VISIBLE, (hints & WindowCreationHints::Visible) != 0);
  glfwWindowHint(GLFW_DECORATED, (hints & WindowCreationHints::Decorated) != 0);
  glfwWindowHint(GLFW_FOCUSED, (hints & WindowCreationHints::Focused) != 0);
  glfwWindowHint(GLFW_AUTO_ICONIFY, (hints & WindowCreationHints::AutoIconified) != 0);
  glfwWindowHint(GLFW_FLOATING, (hints & WindowCreationHints::Floating) != 0);
  glfwWindowHint(GLFW_MAXIMIZED, (hints & WindowCreationHints::Maximised) != 0);
  glfwWindowHint(GLFW_CENTER_CURSOR, (hints & WindowCreationHints::CursorCentered) != 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (hints & WindowCreationHints::TransparentFramebuffer) != 0);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, (hints & WindowCreationHints::FocusingOnShow) != 0);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, (hints & WindowCreationHints::ScalingToMonitor) != 0);
}

void GLFWWindow::GLFWCloseEventCallback(GLFWWindowHandle handle)
{
  WindowCloseEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWMaximiseEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowMaximiseEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  event.maximized = current_state == GLFW_TRUE ? true : false;

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWIconifyEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowIconifyEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  event.iconified = current_state == GLFW_TRUE ? true : false;

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWResizeEventCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height)
{
  WindowResizeEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  event.width = static_cast<uint32_t>(new_width);
  event.height = static_cast<uint32_t>(new_height);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWFocusEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowFocusEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  event.state = static_cast<WindowFocusEvent::FocusState>(current_state);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::RegisterGLFWWindowCallbacks()
{
  glfwSetWindowCloseCallback(handle_, GLFWCloseEventCallback);
  glfwSetWindowMaximizeCallback(handle_, GLFWMaximiseEventCallback);
  glfwSetWindowIconifyCallback(handle_, GLFWIconifyEventCallback);
  glfwSetWindowSizeCallback(handle_, GLFWResizeEventCallback);
  glfwSetWindowFocusCallback(handle_, GLFWFocusEventCallback);
}

} // namespace genebits::engine