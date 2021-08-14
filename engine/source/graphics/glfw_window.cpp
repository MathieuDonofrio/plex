
#include "engine/graphics/glfw_window.h"

#define GLFW_INCLUDE_NONE // Removes OpenGL
#define GLFW_INCLUDE_VULKAN

#include <algorithm>
#include <iostream>

#include "GLFW/glfw3.h"

namespace genebits::engine
{

struct GLFWWindow::Pimpl
{
  GLFWwindow* handle {};

  std::string title;
  uint32_t width;
  uint32_t height;

  struct
  {
    uint32_t max_width = GLFW_DONT_CARE;
    uint32_t max_height = GLFW_DONT_CARE;
    uint32_t min_width = 720;
    uint32_t min_height = 480;
  } size_limit;

  void ApplyWindowCreationHints(const WindowCreationHints& hints);
  void RegisterGLFWWindowCallbacks();

  static void GLFWCloseEventCallback(GLFWwindow*);
  static void GLFWMaximiseEventCallback(GLFWwindow*, int32_t current_state);
  static void GLFWIconifyEventCallback(GLFWwindow*, int32_t current_state);
  static void GLFWResizeEventCallback(GLFWwindow*, int32_t new_width, int32_t new_height);
  static void GLFWFocusEventCallback(GLFWwindow*, int32_t current_state);
};

GLFWWindow::GLFWWindow(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  : pimpl_(new Pimpl), Listener<GLFWWindow, WindowCloseEvent>()
{
  pimpl_->title = title;
  pimpl_->width = width;
  pimpl_->height = height;

  glfwInit(); // Assert this

  if (window_creation_hints == WindowCreationHints::Defaults)
  {
    glfwDefaultWindowHints();
  }
  else
  {
    pimpl_->ApplyWindowCreationHints(window_creation_hints);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  pimpl_->handle = glfwCreateWindow(
    static_cast<int>(pimpl_->width), static_cast<int>(pimpl_->height), pimpl_->title.c_str(), nullptr, nullptr);

  glfwSetWindowUserPointer(pimpl_->handle, this);

  pimpl_->RegisterGLFWWindowCallbacks();
}

GLFWWindow::~GLFWWindow()
{
  glfwDestroyWindow(pimpl_->handle);

  delete pimpl_;
  pimpl_ = nullptr;
}

void GLFWWindow::PollEvents()
{
  glfwPollEvents();
}

void GLFWWindow::WaitEvents(double timeout)
{
  if (timeout > 0.0)
  {
    glfwWaitEvents();
  }
  else
  {
    glfwWaitEventsTimeout(timeout);
  }
}

const std::string& GLFWWindow::GetTitle() const
{
  return pimpl_->title;
}

void GLFWWindow::SetTitle(const std::string& title)
{
  pimpl_->title = title;

  glfwSetWindowTitle(pimpl_->handle, pimpl_->title.c_str());
}

uint32_t GLFWWindow::GetWidth() const
{
  int width;

  glfwGetWindowSize(pimpl_->handle, &width, nullptr);

  return static_cast<uint32_t>(width);
}

uint32_t GLFWWindow::GetHeight() const
{
  int height;

  glfwGetWindowSize(pimpl_->handle, nullptr, &height);

  return static_cast<uint32_t>(height);
}

void GLFWWindow::Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions)
{
  if (!glfwGetWindowAttrib(pimpl_->handle, GLFW_MAXIMIZED))
  {
    if (overwrite_max_dimensions)
    {
      SetMaximumWidth(width);
      SetMaximumHeight(height);
    }
    else
    {
      uint32_t max_width = (pimpl_->size_limit.max_width == -1) ? GetMaximumWidth() : pimpl_->size_limit.max_width;
      uint32_t max_height = (pimpl_->size_limit.max_height == -1) ? GetMaximumHeight() : pimpl_->size_limit.max_height;

      uint32_t new_width = std::max(pimpl_->size_limit.min_width, std::min(width, max_width));
      uint32_t new_height = std::max(pimpl_->size_limit.min_height, std::min(height, max_height));

      glfwSetWindowSize(pimpl_->handle, static_cast<int>(new_width), static_cast<int>(new_height));
    }
  }
}

void GLFWWindow::Focus()
{
  glfwFocusWindow(pimpl_->handle);
}

void GLFWWindow::Maximize()
{
  glfwMaximizeWindow(pimpl_->handle);
}

void GLFWWindow::Iconify()
{
  glfwIconifyWindow(pimpl_->handle);
}

template<>
void GLFWWindow::CreateWindowSurface<VkInstance, VkSurfaceKHR>(VkInstance instance, VkSurfaceKHR* surface)
{
  glfwCreateWindowSurface(instance, pimpl_->handle, nullptr, surface);
}

void GLFWWindow::Restore()
{
  glfwRestoreWindow(pimpl_->handle);
}

void GLFWWindow::RequestAttention()
{
  glfwRequestWindowAttention(pimpl_->handle);
}

void GLFWWindow::Close()
{
  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});
  glfwSetWindowShouldClose(pimpl_->handle, 1);
}

bool GLFWWindow::IsClosing() const noexcept
{
  return glfwWindowShouldClose(pimpl_->handle) != 0;
}

void GLFWWindow::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
{
  // TODO add LOD icons (small, med, big)
  if (pixels != nullptr)
  {
    GLFWimage icon { static_cast<int>(width), static_cast<int>(height), pixels };
    glfwSetWindowIcon(pimpl_->handle, 1, &icon);
  }
  else
  {
    glfwSetWindowIcon(pimpl_->handle, 0, nullptr);
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

uint32_t GLFWWindow::GetMinimumWidth() const noexcept
{
  return pimpl_->size_limit.min_width;
}

uint32_t GLFWWindow::GetMinimumHeight() const noexcept
{
  return pimpl_->size_limit.min_height;
}

uint32_t GLFWWindow::GetMaximumWidth() const
{
  if (pimpl_->size_limit.max_width == GLFW_DONT_CARE)
  {
    return GetMonitorWidth();
  }
  else
  {
    return pimpl_->size_limit.max_width;
  }
}

uint32_t GLFWWindow::GetMaximumHeight() const
{
  if (pimpl_->size_limit.max_height == GLFW_DONT_CARE)
  {
    return GetMonitorHeight();
  }
  else
  {
    return pimpl_->size_limit.max_height;
  }
}

void GLFWWindow::SetMaximumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.max_width = std::max(width, pimpl_->size_limit.min_width);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void GLFWWindow::SetMaximumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.max_height = std::max(height, pimpl_->size_limit.min_height);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void GLFWWindow::SetMinimumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.min_width = std::min(width, pimpl_->size_limit.max_width);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void GLFWWindow::SetMinimumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.min_height = std::min(height, pimpl_->size_limit.max_height);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

bool GLFWWindow::IsIconified() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_ICONIFIED));
}

bool GLFWWindow::IsMaximised() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_MAXIMIZED));
}

bool GLFWWindow::IsFocused() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_FOCUSED));
}

bool GLFWWindow::IsVisible() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_VISIBLE));
}

void GLFWWindow::SetFullScreenRefreshRate(uint64_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
}

void GLFWWindow::listen(const WindowCloseEvent&)
{
  std::cout << "Event closing" << std::endl;
}

void GLFWWindow::Pimpl::ApplyWindowCreationHints(const WindowCreationHints& hints)
{
  // TODO might be good to keep track of some of those states
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

void GLFWWindow::Pimpl::GLFWCloseEventCallback(GLFWwindow*)
{
  // TODO add a way to pass the pointer to an object that lets other system say that they object the window closing (objection counter ?)
  // That way some system can prevent closing of the program resulting in unwanted/undefined behaviour
  // 2 brain stormed ideas:
  // 1 -> Post events to see/update the objecting systems state(know if they agree to the closing of the window now)
  // 2 -> Let the system post an event when they are ready to close
  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});
}

void GLFWWindow::Pimpl::GLFWMaximiseEventCallback(GLFWwindow*, int32_t current_state)
{
  if (current_state)
  {
    GetEnvironment().GetEventBus().Publish(WindowMaximiseEvent {});
  }
  else
  {
    GetEnvironment().GetEventBus().Publish(WindowRestoreEvent {});
  }
}

void GLFWWindow::Pimpl::GLFWIconifyEventCallback(GLFWwindow*, int32_t current_state)
{
  if (current_state)
  {
    GetEnvironment().GetEventBus().Publish(WindowIconifyEvent {});
  }
  else
  {
    GetEnvironment().GetEventBus().Publish(WindowRestoreEvent {});
  }
}

void GLFWWindow::Pimpl::GLFWResizeEventCallback(GLFWwindow*, int32_t new_width, int32_t new_height)
{
  GetEnvironment().GetEventBus().Publish(WindowResizeEvent { static_cast<uint32_t>(new_width), static_cast<uint32_t>(new_height) });
}

void GLFWWindow::Pimpl::GLFWFocusEventCallback(GLFWwindow*, int32_t current_state)
{
  GetEnvironment().GetEventBus().Publish(WindowFocusEvent { static_cast<WindowFocusEventState>(current_state) });
}

void GLFWWindow::Pimpl::RegisterGLFWWindowCallbacks()
{
  glfwSetWindowCloseCallback(handle, GLFWCloseEventCallback);
  glfwSetWindowMaximizeCallback(handle, GLFWMaximiseEventCallback);
  glfwSetWindowIconifyCallback(handle, GLFWIconifyEventCallback);
  glfwSetWindowSizeCallback(handle, GLFWResizeEventCallback);
  glfwSetWindowFocusCallback(handle, GLFWFocusEventCallback);
}

} // namespace genebits::engine