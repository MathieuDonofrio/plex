
#include "engine/graphics/window.h"

#define GLFW_INCLUDE_NONE // Removes OpenGL
#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"

namespace genebits::engine
{

struct Window::Pimpl
{
  GLFWwindow* handle_ {};

  std::string title_;
  uint32_t width_;
  uint32_t height_;

  struct SizeLimit
  {
    uint32_t max_width = GLFW_DONT_CARE;
    uint32_t max_height = GLFW_DONT_CARE;
    uint32_t min_width = 720;
    uint32_t min_height = 480;
  } size_limit_;

  void ApplyWindowCreationHints(const WindowCreationHints& hints);

  // This is very heavyweight and will be replaced by events in the future, this only serves for PoC needs
  std::function<void(Window*)> window_closing_user_callback_;
  std::function<void(GLFWwindow*)> window_closing_internal_callback_;
};

Window::Window(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  : pimpl_(new Pimpl)
{
  pimpl_->title_ = title;
  pimpl_->width_ = width;
  pimpl_->height_ = height;

  //TODO Get the defaults but only modify glfw hints if creation hints are different
  if (window_creation_hints == WindowCreationHints::None)
  {
    glfwDefaultWindowHints();
  }
  else
  {
    pimpl_->ApplyWindowCreationHints(window_creation_hints);
  }

  glfwInit(); // Assert this

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void Window::Create()
{
  pimpl_->handle_ = glfwCreateWindow(
    static_cast<int>(pimpl_->width_), static_cast<int>(pimpl_->height_), pimpl_->title_.c_str(), nullptr, nullptr);

  glfwSetWindowUserPointer(pimpl_->handle_, this);
}

Window::~Window()
{
  glfwDestroyWindow(pimpl_->handle_);

  delete pimpl_;
  pimpl_ = nullptr;
}

const std::string& Window::GetTitle() const
{
  return pimpl_->title_;
}

void Window::SetTitle(const std::string& title)
{
  pimpl_->title_ = title;

  glfwSetWindowTitle(pimpl_->handle_, pimpl_->title_.c_str());
}

uint32_t Window::GetWidth() const
{
  int width;

  glfwGetWindowSize(pimpl_->handle_, &width, nullptr);

  return static_cast<uint32_t>(width);
}

uint32_t Window::GetHeight() const
{
  int height;

  glfwGetWindowSize(pimpl_->handle_, nullptr, &height);

  return static_cast<uint32_t>(height);
}

void Window::Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions)
{
  if (overwrite_max_dimensions)
  {
    SetMaximumWidth(std::max(pimpl_->size_limit_.max_width, width));
    SetMaximumHeight(std::max(pimpl_->size_limit_.max_height, height));
  }
  else
  {
    glfwSetWindowSize(pimpl_->handle_, width, height);
  }
}

void Window::Focus()
{
  glfwFocusWindow(pimpl_->handle_);
}

void Window::Maximize()
{
  glfwMaximizeWindow(pimpl_->handle_);
}

void Window::Minimize()
{
  glfwIconifyWindow(pimpl_->handle_);
}

template<>
void Window::CreateWindowSurface<VkInstance, VkSurfaceKHR>(VkInstance instance, VkSurfaceKHR* surface)
{
  glfwCreateWindowSurface(instance, pimpl_->handle_, nullptr, surface);
}

void Window::Restore()
{
  glfwRestoreWindow(pimpl_->handle_);
}

void Window::Close()
{
  glfwSetWindowShouldClose(pimpl_->handle_, 1);
}

bool Window::IsClosed() const noexcept
{
  return glfwWindowShouldClose(pimpl_->handle_) != 0;
}

//To revert to the default window icon, pass in a nullptr for the pixel array
void Window::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
{
  //TODO add LOD icons (small, med, big)
  if (pixels != nullptr)
  {
    GLFWimage icon[1] { { static_cast<int>(width), static_cast<int>(height), pixels } };
    glfwSetWindowIcon(pimpl_->handle_, 1, icon);
  }
  else
  {
    glfwSetWindowIcon(pimpl_->handle_, 0, nullptr);
  }
}

uint32_t Window::GetMonitorWidth() const
{
  GLFWmonitor* monitor_ptr = glfwGetWindowMonitor(pimpl_->handle_);
  return glfwGetVideoMode(monitor_ptr)->width;
}

uint32_t Window::GetMonitorHeight() const
{
  GLFWmonitor* monitor_ptr = glfwGetWindowMonitor(pimpl_->handle_);
  return glfwGetVideoMode(monitor_ptr)->height;
}

uint32_t Window::GetMinimumWidth() const noexcept
{
  return pimpl_->size_limit_.min_width;
}

uint32_t Window::GetMinimumHeight() const noexcept
{
  return pimpl_->size_limit_.min_height;
}

uint32_t Window::GetMaximumWidth() const
{
  if (pimpl_->size_limit_.max_width == GLFW_DONT_CARE)
  {
    return GetMonitorWidth();
  }
  else
  {
    return pimpl_->size_limit_.max_width;
  }
}

uint32_t Window::GetMaximumHeight() const
{
  if (pimpl_->size_limit_.max_height == GLFW_DONT_CARE)
  {
    return GetMonitorHeight();
  }
  else
  {
    return pimpl_->size_limit_.max_height;
  }
}

void Window::SetMaximumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit_;
  size_limit.max_width = width;
  glfwSetWindowSizeLimits(pimpl_->handle_, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMaximumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit_;
  size_limit.max_height = height;
  glfwSetWindowSizeLimits(pimpl_->handle_, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMinimumWidth(uint32_t width)
{
  auto& size_limit = pimpl_->size_limit_;
  size_limit.min_width = width;
  glfwSetWindowSizeLimits(pimpl_->handle_, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetMinimumHeight(uint32_t height)
{
  auto& size_limit = pimpl_->size_limit_;
  size_limit.min_height = height;
  glfwSetWindowSizeLimits(pimpl_->handle_, size_limit.min_width, size_limit.min_height, size_limit.max_width, size_limit.max_height);
}

void Window::SetFullScreenRefreshRate(uint64_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
}

void Window::SetWindowClosingCallback(std::function<void(Window*)> window_closing_callback)
{
  //  pimpl_->window_closing_user_callback_ = window_closing_callback;
  //  pimpl_->window_closing_internal_callback_ = [](GLFWwindow* glfw_window_ptr) -> void
  //  {
  //    auto window_ptr = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window_ptr));
  //    window_ptr->pimpl_->window_closing_user_callback_(window_ptr);
  //  };

  //glfwSetWindowCloseCallback(pimpl_->handle_, pimpl_->window_closing_internal_callback_)
}

void Window::Pimpl::ApplyWindowCreationHints(const WindowCreationHints& hints)
{
  //TODO might be good to keep track of some of those states
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

} // namespace genebits::engine