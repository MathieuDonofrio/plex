
#include "engine/graphics/window.h"

#define GLFW_INCLUDE_NONE // Removes OpenGL
#define GLFW_INCLUDE_VULKAN

#include <algorithm>
#include <iostream>

#include "GLFW/glfw3.h"

namespace genebits::engine
{

struct Window::Pimpl
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

Window::Window(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  : pimpl_(new Pimpl), Listener<Window, WindowCloseEvent>()
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

Window::~Window()
{
  glfwDestroyWindow(pimpl_->handle);

  delete pimpl_;
  pimpl_ = nullptr;
}

void Window::PollEvents()
{
  glfwPollEvents();
}

void Window::WaitEvents(double timeout)
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

const std::string& Window::GetTitle() const
{
  return pimpl_->title;
}

void Window::SetTitle(const std::string& title)
{
  pimpl_->title = title;

  glfwSetWindowTitle(pimpl_->handle, pimpl_->title.c_str());
}

uint32_t Window::GetWidth() const
{
  int width;

  glfwGetWindowSize(pimpl_->handle, &width, nullptr);

  return static_cast<uint32_t>(width);
}

uint32_t Window::GetHeight() const
{
  int height;

  glfwGetWindowSize(pimpl_->handle, nullptr, &height);

  return static_cast<uint32_t>(height);
}

void Window::Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions)
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

void Window::Focus()
{
  glfwFocusWindow(pimpl_->handle);
}

void Window::Maximize()
{
  glfwMaximizeWindow(pimpl_->handle);
}

void Window::Iconify()
{
  glfwIconifyWindow(pimpl_->handle);
}

template<>
void Window::CreateWindowSurface<VkInstance, VkSurfaceKHR>(VkInstance instance, VkSurfaceKHR* surface)
{
  glfwCreateWindowSurface(instance, pimpl_->handle, nullptr, surface);
}

void Window::Restore()
{
  glfwRestoreWindow(pimpl_->handle);
}

void Window::RequestAttention()
{
  glfwRequestWindowAttention(pimpl_->handle);
}

void Window::Close()
{
  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});
  glfwSetWindowShouldClose(pimpl_->handle, 1);
}

bool Window::IsClosing() const noexcept
{
  return glfwWindowShouldClose(pimpl_->handle) != 0;
}

void Window::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
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

uint32_t Window::GetMonitorWidth() const
{
  GLFWmonitor* monitor_ptr = glfwGetPrimaryMonitor();
  return glfwGetVideoMode(monitor_ptr)->width;
}

uint32_t Window::GetMonitorHeight() const
{
  GLFWmonitor* monitor_ptr = glfwGetPrimaryMonitor();
  return glfwGetVideoMode(monitor_ptr)->height;
}

uint32_t Window::GetMinimumWidth() const noexcept
{
  return pimpl_->size_limit.min_width;
}

uint32_t Window::GetMinimumHeight() const noexcept
{
  return pimpl_->size_limit.min_height;
}

uint32_t Window::GetMaximumWidth() const
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

uint32_t Window::GetMaximumHeight() const
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

void Window::SetMaximumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.max_width = std::max(width, pimpl_->size_limit.min_width);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMaximumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.max_height = std::max(height, pimpl_->size_limit.min_height);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMinimumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.min_width = std::min(width, pimpl_->size_limit.max_width);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMinimumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit;
  size_limit.min_height = std::min(height, pimpl_->size_limit.max_height);
  glfwSetWindowSizeLimits(pimpl_->handle, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

bool Window::IsIconified() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_ICONIFIED));
}

bool Window::IsMaximised() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_MAXIMIZED));
}

bool Window::IsFocused() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_FOCUSED));
}

bool Window::IsVisible() const
{
  return static_cast<bool>(glfwGetWindowAttrib(pimpl_->handle, GLFW_VISIBLE));
}

void Window::SetFullScreenRefreshRate(uint64_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
}

void Window::listen(const WindowCloseEvent&)
{
  std::cout << "Event closing" << std::endl;
}

void Window::Pimpl::ApplyWindowCreationHints(const WindowCreationHints& hints)
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

void Window::Pimpl::GLFWCloseEventCallback(GLFWwindow*)
{
  // TODO add a way to pass the pointer to an object that lets other system say that they object the window closing (objection counter ?)
  // That way some system can prevent closing of the program resulting in unwanted/undefined behaviour
  // 2 brain stormed ideas:
  // 1 -> Post events to see/update the objecting systems state(know if they agree to the closing of the window now)
  // 2 -> Let the system post an event when they are ready to close
  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});
}

void Window::Pimpl::GLFWMaximiseEventCallback(GLFWwindow*, int32_t current_state)
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

void Window::Pimpl::GLFWIconifyEventCallback(GLFWwindow*, int32_t current_state)
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

void Window::Pimpl::GLFWResizeEventCallback(GLFWwindow*, int32_t new_width, int32_t new_height)
{
  GetEnvironment().GetEventBus().Publish(WindowResizeEvent { static_cast<uint32_t>(new_width), static_cast<uint32_t>(new_height) });
}

void Window::Pimpl::GLFWFocusEventCallback(GLFWwindow*, int32_t current_state)
{
  GetEnvironment().GetEventBus().Publish(WindowFocusEvent { static_cast<WindowFocusEventState>(current_state) });
}

void Window::Pimpl::RegisterGLFWWindowCallbacks()
{
  glfwSetWindowCloseCallback(handle, GLFWCloseEventCallback);
  glfwSetWindowMaximizeCallback(handle, GLFWMaximiseEventCallback);
  glfwSetWindowIconifyCallback(handle, GLFWIconifyEventCallback);
  glfwSetWindowSizeCallback(handle, GLFWResizeEventCallback);
  glfwSetWindowFocusCallback(handle, GLFWFocusEventCallback);
}

} // namespace genebits::engine