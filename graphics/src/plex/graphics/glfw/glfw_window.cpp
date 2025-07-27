#include "plex/graphics/glfw/glfw_window.h"

#include <atomic>

#include "plex/debug/logging.h"

#define GLFW_INCLUDE_NONE // Removes OpenGL
#include <GLFW/glfw3.h>

namespace
{
///
/// Error callback for GLFW.
///
/// Called each time a glfw error occurs.
///
/// @param[in] error_code
/// @param[in] error_description
///
void GlfwErrorCallback([[maybe_unused]] int error_code, [[maybe_unused]] const char* error_description)
{
  LOG_ERROR("GLFW Error {}: {}", error_code, error_description);
}

///
/// Manages safe GLFW initialization and termination.
///
/// There should only be one glfw instance.
///
class GLFWInstance
{
public:
  ///
  /// Attempts to initialize GLFW and increase the reference count.
  ///
  /// @return True if initialized is successful, false otherwise
  ///
  [[nodiscard]] bool Ref()
  {
    std::scoped_lock lock { glfw_mutex };

    bool success = true;

    if (glfw_ref_count.load(std::memory_order_relaxed) == 0)
    {
#ifndef NDEBUG
      // Error callback can be set before initialization.
      // This is useful because errors in initialization can also be caught.
      glfwSetErrorCallback(GlfwErrorCallback);
#endif

      success = glfwInit() == GLFW_TRUE;

      if (success) LOG_INFO("GLFW initialized");
      else
      {
        LOG_ERROR("GLFW failed to be initialized");
      }
    }

    glfw_ref_count.fetch_add(1, std::memory_order_relaxed);

    return success;
  }

  ///
  /// Decreases the reference count to GLFW.
  ///
  /// If there are no more references to GLFW, GLFW will be terminated.
  ///
  /// Terminating GLFW is important because sometimes GLFW changes global system settings
  /// and these may not be restored without termination.
  ///
  void Unref()
  {
    ASSERT(glfw_ref_count.load(std::memory_order_relaxed) > 0, "There are no references");

    std::scoped_lock lock { glfw_mutex };

    if (glfw_ref_count.load(std::memory_order_relaxed) == 1)
    {
      glfwTerminate();

      glfwSetErrorCallback(nullptr);

      glfw_ref_count.store(0, std::memory_order_relaxed);

      LOG_INFO("GLFW terminated");
    }
    else
    {
      glfw_ref_count.fetch_sub(1, std::memory_order_relaxed);
    }
  }

private:
  std::mutex glfw_mutex;
  std::atomic_uint glfw_ref_count = 0;
};

///
/// Returns the glfw singleton instance.
///
/// @return GLFW instance.
///
GLFWInstance& GetGLFW()
{
  static GLFWInstance instance;

  return instance;
}

} // namespace

namespace plex
{
GLFWWindow::GLFWWindow(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints)
  : title_(title)
{
  // Make sure glfw is initialized

  bool initialized = GetGLFW().Ref();
  ASSERT(initialized, "GLFW failed to initialize");

  if (!initialized) return; // Don't continue if glfw is not initialized for more readable errors.

  // Create window handle

  ApplyWindowCreationHints(hints);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  handle_ = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);
  ASSERT(handle_, "GLFW window creation failed");

  // Setup window

  glfwSetWindowUserPointer(handle_, this);

  glfwSetWindowSizeLimits(handle_, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwSetInputMode(handle_, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

  glfwSetWindowSizeCallback(handle_, GLFWResizeEventCallback);
  glfwSetWindowCloseCallback(handle_, GLFWCloseEventCallback);
  glfwSetWindowMaximizeCallback(handle_, GLFWMaximizeEventCallback);
  glfwSetWindowIconifyCallback(handle_, GLFWIconifyEventCallback);
  glfwSetWindowFocusCallback(handle_, GLFWFocusEventCallback);
  glfwSetKeyCallback(handle_, GLFWKeyCallback);
  glfwSetCursorPosCallback(handle_, GLFWCursorPosCallback);
  glfwSetCursorEnterCallback(handle_, GLFWCursorEnterCallback);
  glfwSetMouseButtonCallback(handle_, GLFWMouseButtonCallback);
  glfwSetScrollCallback(handle_, GLFWMouseScrollCallback);
  glfwSetFramebufferSizeCallback(handle_, GLFWFramebufferResizeCallback);

  LOG_INFO("Window created: {}", title_);
}

GLFWWindow::~GLFWWindow()
{
  glfwDestroyWindow(handle_);

  LOG_INFO("Window destroyed: {}", title_);

  GetGLFW().Unref();
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
  glfwSetWindowShouldClose(handle_, 1);

  WindowCloseEvent event;
  event.window = this;

  for (auto& callback : close_event_callbacks_)
  {
    callback(event);
  }
}

bool GLFWWindow::IsClosing() const
{
  bool closing = glfwWindowShouldClose(handle_);

  return closing;
}

void GLFWWindow::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
{
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

///
/// Retrieve the monitor on which the window is overlapping the most.
///
/// @param[in] handle glfw window handle of the window to retrieve the monitor of
///
/// @return glfw monitor pointer of the monitor on which the window is overlapping the most
///
GLFWmonitor* GetWindowMonitor(GLFWwindow* handle)
{
  int32_t monitor_count = 0;
  GLFWmonitor** monitor_ptrs = glfwGetMonitors(&monitor_count);

  struct MonitorInfo
  {
    GLFWmonitor* ptr;

    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
  };

  std::vector<MonitorInfo> monitors;
  monitors.reserve(monitor_count);

  for (auto it = monitor_ptrs; it != monitor_ptrs + monitor_count; it++)
  {
    MonitorInfo monitor { *it };

    glfwGetMonitorPos(monitor.ptr, &monitor.x, &monitor.y);

    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor.ptr);
    monitor.width = video_mode->width;
    monitor.height = video_mode->height;

    monitors.push_back(monitor);
  }

  int32_t window_x, window_y, window_width, window_height;

  glfwGetWindowPos(handle, &window_x, &window_y);
  glfwGetWindowSize(handle, &window_width, &window_height);

  for (const MonitorInfo& monitor : monitors)
  {
    // Does not measure how much the window is overlapping the monitor but if the center of the window is
    // inside the monitor. This is much simpler and can be done because of the symmetric nature of the window.
    // In other words: If the center of the window is inside the monitor then that monitor is the one that contains the
    // most window area of all the monitors and can be deemed as the window's monitor

    const int32_t window_center_pos_x = window_x + (window_width / 2);
    const int32_t window_center_pos_y = window_y + (window_height / 2);

    bool inside_h = window_center_pos_x >= monitor.x && window_center_pos_x < monitor.x + monitor.width;
    bool inside_v = window_center_pos_y >= monitor.y && window_center_pos_y < monitor.y + monitor.height;

    if (inside_h && inside_v) return monitor.ptr;
  }

  return nullptr;
}

uint32_t GLFWWindow::GetMonitorWidth() const
{
  GLFWmonitor* monitor = GetWindowMonitor(handle_);

  const uint32_t width = glfwGetVideoMode(monitor)->width;

  return width;
}

uint32_t GLFWWindow::GetMonitorHeight() const
{
  GLFWmonitor* monitor = GetWindowMonitor(handle_);

  const uint32_t height = glfwGetVideoMode(monitor)->height;

  return height;
}

bool GLFWWindow::IsIconified() const
{
  const bool iconified = glfwGetWindowAttrib(handle_, GLFW_ICONIFIED);

  return iconified;
}

bool GLFWWindow::IsMaximized() const
{
  const bool maximized = glfwGetWindowAttrib(handle_, GLFW_MAXIMIZED);

  return maximized;
}

bool GLFWWindow::IsFocused() const
{
  const bool focused = glfwGetWindowAttrib(handle_, GLFW_FOCUSED);

  return focused;
}

bool GLFWWindow::IsVisible() const
{
  const bool visible = glfwGetWindowAttrib(handle_, GLFW_VISIBLE);

  return visible;
}

void GLFWWindow::SetFullScreenRefreshRate(uint32_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, static_cast<int>(refresh_rate));
}

std::pair<int32_t, int32_t> GLFWWindow::GetFrameBufferSize() const
{
  int width = 0;
  int height = 0;

  glfwGetFramebufferSize(handle_, &width, &height);

  return { width, height };
}

VkSurfaceKHR GLFWWindow::CreateWindowSurface(VkInstance instance)
{
  VkSurfaceKHR surface;

  [[maybe_unused]] VkResult result = glfwCreateWindowSurface(instance, handle_, nullptr, &surface);

  ASSERT(result == VK_SUCCESS, "Failed to create Vulkan window surface");

  return surface;
}

std::vector<const char*> GLFWWindow::GetRequiredInstanceExtensions()
{
  uint32_t glfw_extension_count = 0;
  const char** glfw_extension_string_ptrs;
  glfw_extension_string_ptrs = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  return { glfw_extension_string_ptrs, glfw_extension_string_ptrs + glfw_extension_count };
}

bool GLFWWindow::GetPhysicalDevicePresentationSupport(
  VkInstance instance, VkPhysicalDevice physical_device, uint32_t queue_family_index)
{
  bool supported = glfwGetPhysicalDevicePresentationSupport(instance, physical_device, queue_family_index);

  return supported;
}

void GLFWWindow::ApplyWindowCreationHints(const WindowCreationHints& hints)
{
  if (hints == WindowCreationHints::Defaults)
  {
    glfwDefaultWindowHints();

    return;
  }

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

void GLFWWindow::AddWindowResizeEventCallback(WindowEventCallback<WindowResizeEvent> callback)
{
  resize_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowCloseEventCallback(WindowEventCallback<WindowCloseEvent> callback)
{
  close_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowFocusEventCallback(WindowEventCallback<WindowFocusEvent> callback)
{
  focus_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowIconifyEventCallback(WindowEventCallback<WindowIconifyEvent> callback)
{
  iconify_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowMaximizeEventCallback(WindowEventCallback<WindowMaximizeEvent> callback)
{
  maximize_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowKeyboardEventCallback(WindowEventCallback<WindowKeyboardEvent> callback)
{
  keyboard_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowCursorMoveEventCallback(WindowEventCallback<WindowCursorMoveEvent> callback)
{
  cursor_move_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowCursorEnterEventCallback(WindowEventCallback<WindowCursorEnterEvent> callback)
{
  cursor_enter_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowMouseButtonEventCallback(WindowEventCallback<WindowMouseButtonEvent> callback)
{
  mouse_button_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowMouseScrollEventCallback(WindowEventCallback<WindowMouseScrollEvent> callback)
{
  mouse_scroll_event_callbacks_.push_back(callback);
}

void GLFWWindow::AddWindowFramebufferResizeEventCallback(WindowEventCallback<WindowFramebufferResizeEvent> callback)
{
  framebuffer_resize_event_callbacks_.push_back(callback);
}

void GLFWWindow::RemoveWindowResizeEventCallback(WindowEventCallback<WindowResizeEvent> callback)
{
  auto it = std::find(resize_event_callbacks_.begin(), resize_event_callbacks_.end(), callback);

  if (it != resize_event_callbacks_.end())
  {
    resize_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowCloseEventCallback(WindowEventCallback<WindowCloseEvent> callback)
{
  auto it = std::find(close_event_callbacks_.begin(), close_event_callbacks_.end(), callback);

  if (it != close_event_callbacks_.end())
  {
    close_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowFocusEventCallback(WindowEventCallback<WindowFocusEvent> callback)
{
  auto it = std::find(focus_event_callbacks_.begin(), focus_event_callbacks_.end(), callback);

  if (it != focus_event_callbacks_.end())
  {
    focus_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowIconifyEventCallback(WindowEventCallback<WindowIconifyEvent> callback)
{
  auto it = std::find(iconify_event_callbacks_.begin(), iconify_event_callbacks_.end(), callback);

  if (it != iconify_event_callbacks_.end())
  {
    iconify_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowMaximizeEventCallback(WindowEventCallback<WindowMaximizeEvent> callback)
{
  auto it = std::find(maximize_event_callbacks_.begin(), maximize_event_callbacks_.end(), callback);

  if (it != maximize_event_callbacks_.end())
  {
    maximize_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowKeyboardEventCallback(WindowEventCallback<WindowKeyboardEvent> callback)
{
  auto it = std::find(keyboard_event_callbacks_.begin(), keyboard_event_callbacks_.end(), callback);

  if (it != keyboard_event_callbacks_.end())
  {
    keyboard_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowCursorMoveEventCallback(WindowEventCallback<WindowCursorMoveEvent> callback)
{
  auto it = std::find(cursor_move_event_callbacks_.begin(), cursor_move_event_callbacks_.end(), callback);

  if (it != cursor_move_event_callbacks_.end())
  {
    cursor_move_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowCursorEnterEventCallback(WindowEventCallback<WindowCursorEnterEvent> callback)
{
  auto it = std::find(cursor_enter_event_callbacks_.begin(), cursor_enter_event_callbacks_.end(), callback);

  if (it != cursor_enter_event_callbacks_.end())
  {
    cursor_enter_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowMouseButtonEventCallback(WindowEventCallback<WindowMouseButtonEvent> callback)
{
  auto it = std::find(mouse_button_event_callbacks_.begin(), mouse_button_event_callbacks_.end(), callback);

  if (it != mouse_button_event_callbacks_.end())
  {
    mouse_button_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowMouseScrollEventCallback(WindowEventCallback<WindowMouseScrollEvent> callback)
{
  auto it = std::find(mouse_scroll_event_callbacks_.begin(), mouse_scroll_event_callbacks_.end(), callback);

  if (it != mouse_scroll_event_callbacks_.end())
  {
    mouse_scroll_event_callbacks_.erase(it);
  }
}

void GLFWWindow::RemoveWindowFramebufferResizeEventCallback(WindowEventCallback<WindowFramebufferResizeEvent> callback)
{
  auto it = std::find(framebuffer_resize_event_callbacks_.begin(), framebuffer_resize_event_callbacks_.end(), callback);

  if (it != framebuffer_resize_event_callbacks_.end())
  {
    framebuffer_resize_event_callbacks_.erase(it);
  }
}

void GLFWWindow::GLFWCloseEventCallback(GLFWWindowHandle handle)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowCloseEvent event;
    event.window = window;

    for (auto& callback : window->close_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWMaximizeEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowMaximizeEvent event;
    event.window = window;
    event.maximized = current_state == GLFW_TRUE;

    for (auto& callback : window->maximize_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWIconifyEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowIconifyEvent event;
    event.window = window;
    event.iconified = current_state == GLFW_TRUE;

    for (auto& callback : window->iconify_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWResizeEventCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowResizeEvent event;
    event.window = window;
    event.width = static_cast<uint32_t>(new_width);
    event.height = static_cast<uint32_t>(new_height);

    for (auto& callback : window->resize_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWFocusEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowFocusEvent event;
    event.window = window;
    event.state = static_cast<WindowFocusEvent::FocusState>(current_state);

    for (auto& callback : window->focus_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWKeyCallback(GLFWWindowHandle handle, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowKeyboardEvent event;
    event.window = window;
    event.keycode = static_cast<KeyCode>(key);
    event.modifiers = static_cast<ButtonEvent::ModifierKeys>(mods);
    event.scancode = scancode;
    event.action = static_cast<ButtonEvent::Action>(action);

    for (auto& callback : window->keyboard_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWCursorPosCallback(GLFWWindowHandle handle, double x_pos, double y_pos)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowCursorMoveEvent event;
    event.window = window;
    event.pos_x = static_cast<int32_t>(x_pos);
    event.pos_y = static_cast<int32_t>(y_pos);

    for (auto& callback : window->cursor_move_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWCursorEnterCallback(GLFWWindowHandle handle, int32_t entered)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowCursorEnterEvent event;
    event.window = window;
    event.cursor_hover_state = static_cast<WindowCursorEnterEvent::CursorHoverState>(entered);

    for (auto& callback : window->cursor_enter_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWMouseButtonCallback(GLFWWindowHandle handle, int32_t button, int32_t action, int32_t mods)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowMouseButtonEvent event;
    event.window = window;
    event.button = static_cast<WindowMouseButtonEvent::CursorButton>(button);
    event.action = static_cast<ButtonEvent::Action>(action);
    event.modifiers = static_cast<ButtonEvent::ModifierKeys>(mods);

    for (auto& callback : window->mouse_button_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWMouseScrollCallback(GLFWWindow::GLFWWindowHandle handle, double, double y_offset)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowMouseScrollEvent event;
    event.window = window;
    event.vertical_offset = static_cast<int32_t>(y_offset);

    for (auto& callback : window->mouse_scroll_event_callbacks_)
    {
      callback(event);
    }
  }
}

void GLFWWindow::GLFWFramebufferResizeCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height)
{
  auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

  if (window)
  {
    WindowFramebufferResizeEvent event;
    event.window = window;
    event.width = static_cast<uint32_t>(new_width);
    event.height = static_cast<uint32_t>(new_height);

    for (auto& callback : window->framebuffer_resize_event_callbacks_)
    {
      callback(event);
    }
  }
}

} // namespace plex
