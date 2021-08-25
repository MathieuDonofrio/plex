#include "genebits/engine/graphics/glfw_window.h"

#include <atomic>

#include "genebits/engine/core/environment.h"
#include "genebits/engine/debug/assertion.h"

#define GLFW_INCLUDE_NONE // Removes OpenGL
#include "GLFW/glfw3.h"

namespace
{
///
/// Asserts the last glfw call.
///
inline void AssertLastGlfwCall()
{
  const char* error_description;
  const int error_code = glfwGetError(&error_description);

  if (error_description) {}

  ASSERT(error_code == GLFW_NO_ERROR, "GLFW error occurred");

  (void)(error_code); // Suppress warnings
}

///
/// Keeps a count of all references to GLFW.
///
/// This is important to properly terminate GLFW when it is no longer required.
///
std::atomic_uint glfw_ref_count = 0;

///
/// Attempts to initialize GLFW and increase the reference count.
///
/// @return True if initialized is succesful, false otherwise
///
inline bool ReferenceGlfw()
{
  int glfw_init_result = glfwInit();

  if (glfw_init_result == GLFW_TRUE) { glfw_ref_count.fetch_add(1, std::memory_order_relaxed); }

  ASSERT(glfw_init_result == GLFW_TRUE, "GLFW initialization failed");

  return glfw_init_result == GLFW_TRUE;
}

///
/// Decreases the reference count to GLFW.
///
/// If there are no more references to GLFW, GLFW will be terminated.
///
/// Terminating GLFW is important because sometimes GLFW changes global system settings
/// and these may not be restored without termination.
///
inline void UnreferenceGlfw()
{
  if (glfw_ref_count.load(std::memory_order_relaxed) <= 1) glfwTerminate();
  else
  {
    glfw_ref_count.fetch_sub(1, std::memory_order_relaxed);
  }
}

} // namespace

// GLFW assertion is useful for obtaining descriptive error handling. Obtaining the last error does incur some
// overhead so two macros are provided:
// - GLFW_ASSERT: For non-performance critical, handles errors even for release builds.
// - GLFW_ASSERT_DEBUG_ONLY: For performance critical, handles errors only for debug builds.

#define GLFW_ASSERT AssertLastGlfwCall()

#ifdef NDEBUG
#define GLFW_ASSERT_DEBUG_ONLY (void)(0)
#else
#define GLFW_ASSERT_DEBUG_ONLY GLFW_ASSERT
#endif

namespace genebits::engine
{
GLFWWindow::GLFWWindow(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints)
  : title_(title)
{
  if (!ReferenceGlfw()) return;

  ApplyWindowCreationHints(hints);
  GLFW_ASSERT;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFW_ASSERT;

  handle_ = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);
  GLFW_ASSERT;
  ASSERT(handle_, "GLFW window creation failed");

  glfwSetWindowSizeLimits(handle_, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
  GLFW_ASSERT;

  glfwSetWindowUserPointer(handle_, this);
  GLFW_ASSERT;

  glfwSetInputMode(
    handle_, GLFW_LOCK_KEY_MODS, GLFW_TRUE); // Tell glfw that we want the state of "caps lock" and "num lock"
  // when receiving keyboard events
  GLFW_ASSERT;

  glfwSetWindowSizeCallback(handle_, GLFWResizeEventCallback);
  GLFW_ASSERT;
  glfwSetWindowCloseCallback(handle_, GLFWCloseEventCallback);
  GLFW_ASSERT;
  glfwSetWindowMaximizeCallback(handle_, GLFWMaximizeEventCallback);
  GLFW_ASSERT;
  glfwSetWindowIconifyCallback(handle_, GLFWIconifyEventCallback);
  GLFW_ASSERT;
  glfwSetWindowFocusCallback(handle_, GLFWFocusEventCallback);
  GLFW_ASSERT;
  glfwSetKeyCallback(handle_, GLFWKeyCallback);
  GLFW_ASSERT;
  glfwSetCursorPosCallback(handle_, GLFWCursorPosCallback);
  GLFW_ASSERT;
  glfwSetCursorEnterCallback(handle_, GLFWCursorEnterCallback);
  GLFW_ASSERT;
  glfwSetMouseButtonCallback(handle_, GLFWMouseButtonCallback);
  GLFW_ASSERT;
  glfwSetScrollCallback(handle_, GLFWMouseScrollCallback);
  GLFW_ASSERT;
}

GLFWWindow::~GLFWWindow()
{
  glfwDestroyWindow(handle_);
  GLFW_ASSERT;

  UnreferenceGlfw();
}

void GLFWWindow::PollEvents()
{
  glfwPollEvents();
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::WaitEvents()
{
  glfwWaitEvents();
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::WaitEvents(double timeout)
{
  glfwWaitEventsTimeout(timeout);
  GLFW_ASSERT_DEBUG_ONLY;
}

const std::string& GLFWWindow::GetTitle() const
{
  return title_;
}

void GLFWWindow::SetTitle(const std::string& title)
{
  title_ = title;

  glfwSetWindowTitle(handle_, title.c_str());
  GLFW_ASSERT_DEBUG_ONLY;
}

uint32_t GLFWWindow::GetWidth() const
{
  int width;

  glfwGetWindowSize(handle_, &width, nullptr);
  GLFW_ASSERT_DEBUG_ONLY;

  return static_cast<uint32_t>(width);
}

uint32_t GLFWWindow::GetHeight() const
{
  int height;

  glfwGetWindowSize(handle_, nullptr, &height);
  GLFW_ASSERT_DEBUG_ONLY;

  return static_cast<uint32_t>(height);
}

void GLFWWindow::Resize(uint32_t width, uint32_t height)
{
  glfwSetWindowSize(handle_, static_cast<int>(width), static_cast<int>(height));
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::Focus()
{
  glfwFocusWindow(handle_);
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::Maximize()
{
  glfwMaximizeWindow(handle_);
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::Iconify()
{
  glfwIconifyWindow(handle_);
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::Restore()
{
  glfwRestoreWindow(handle_);
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::RequestAttention()
{
  glfwRequestWindowAttention(handle_);
  GLFW_ASSERT_DEBUG_ONLY;
}

void GLFWWindow::Close()
{
  // TODO Matt: Brainstorm ideas for closing protocol
  glfwSetWindowShouldClose(handle_, 1);

  GetEnvironment().GetEventBus().Publish(WindowCloseEvent {});

  GLFW_ASSERT_DEBUG_ONLY;
}

bool GLFWWindow::IsClosing() const
{
  bool closing = glfwWindowShouldClose(handle_);
  GLFW_ASSERT_DEBUG_ONLY;

  return closing;
}

void GLFWWindow::SetIcon(uint8_t* pixels, uint32_t width, uint32_t height)
{
  // TODO Matt: Simplify

  // TODO add LOD icons (small, med, big)
  if (pixels != nullptr)
  {
    GLFWimage icon { static_cast<int>(width), static_cast<int>(height), pixels };
    glfwSetWindowIcon(handle_, 1, &icon);
    GLFW_ASSERT_DEBUG_ONLY;
  }
  else
  {
    glfwSetWindowIcon(handle_, 0, nullptr);
    GLFW_ASSERT_DEBUG_ONLY;
  }
}

uint32_t GLFWWindow::GetMonitorWidth() const
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  GLFW_ASSERT_DEBUG_ONLY;

  const uint32_t width = glfwGetVideoMode(monitor)->width;
  GLFW_ASSERT_DEBUG_ONLY;

  return width;
}

uint32_t GLFWWindow::GetMonitorHeight() const
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  GLFW_ASSERT_DEBUG_ONLY;

  const uint32_t height = glfwGetVideoMode(monitor)->height;
  GLFW_ASSERT_DEBUG_ONLY;

  return height;
}

bool GLFWWindow::IsIconified() const
{
  const bool iconified = glfwGetWindowAttrib(handle_, GLFW_ICONIFIED);
  GLFW_ASSERT_DEBUG_ONLY;

  return iconified;
}

bool GLFWWindow::IsMaximized() const
{
  const bool maximized = glfwGetWindowAttrib(handle_, GLFW_MAXIMIZED);
  GLFW_ASSERT_DEBUG_ONLY;

  return maximized;
}

bool GLFWWindow::IsFocused() const
{
  const bool focused = glfwGetWindowAttrib(handle_, GLFW_FOCUSED);
  GLFW_ASSERT_DEBUG_ONLY;

  return focused;
}

bool GLFWWindow::IsVisible() const
{
  const bool visible = glfwGetWindowAttrib(handle_, GLFW_VISIBLE);
  GLFW_ASSERT_DEBUG_ONLY;

  return visible;
}

void GLFWWindow::SetFullScreenRefreshRate(uint32_t refresh_rate)
{
  glfwWindowHint(GLFW_REFRESH_RATE, static_cast<int>(refresh_rate));
  GLFW_ASSERT_DEBUG_ONLY;
}

VkSurfaceKHR* GLFWWindow::CreateWindowSurface(VkInstance instance)
{
  VkSurfaceKHR* surface = nullptr;

  //  VkResult result = glfwCreateWindowSurface(instance, handle_, nullptr, surface);
  //  GLFW_ASSERT;
  //
  //  ASSERT(result == VK_SUCCESS, "Vulkan window surface creation failed");
  //  (void)result; // Suppress warning

  return surface;
}

VulkanInstanceExtensions GLFWWindow::GetRequiredInstanceExtensions()
{
  VulkanInstanceExtensions extensions { nullptr, 0 };
  // Not implemented yet
  //  extensions.extensions = glfwGetRequiredInstanceExtensions(&extensions.count);
  //  GLFW_ASSERT;

  return extensions;
}

bool GLFWWindow::GetPhysicalDevicePresentationSupport(
  VkInstance instance, VkPhysicalDevice physical_device, uint32_t queue_family_index)
{
  // Not implemented yet
  // bool supported = glfwGetPhysicalDevicePresentationSupport(instance, physical_device, queue_family_index);
  GLFW_ASSERT;

  bool supported = false;
  return supported;
}

void GLFWWindow::ApplyWindowCreationHints(const WindowCreationHints& hints)
{
  if (hints == WindowCreationHints::Defaults)
  {
    glfwDefaultWindowHints();
    GLFW_ASSERT;

    return;
  }

  glfwWindowHint(GLFW_RESIZABLE, (hints & WindowCreationHints::Resizable) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_VISIBLE, (hints & WindowCreationHints::Visible) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_DECORATED, (hints & WindowCreationHints::Decorated) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_FOCUSED, (hints & WindowCreationHints::Focused) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_AUTO_ICONIFY, (hints & WindowCreationHints::AutoIconified) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_FLOATING, (hints & WindowCreationHints::Floating) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_MAXIMIZED, (hints & WindowCreationHints::Maximised) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_CENTER_CURSOR, (hints & WindowCreationHints::CursorCentered) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (hints & WindowCreationHints::TransparentFramebuffer) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, (hints & WindowCreationHints::FocusingOnShow) != 0);
  GLFW_ASSERT;
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, (hints & WindowCreationHints::ScalingToMonitor) != 0);
  GLFW_ASSERT;
}

void GLFWWindow::GLFWCloseEventCallback(GLFWWindowHandle handle)
{
  WindowCloseEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWMaximizeEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowMaximizeEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.maximized = current_state == GLFW_TRUE;

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWIconifyEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowIconifyEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.iconified = current_state == GLFW_TRUE;

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWResizeEventCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height)
{
  WindowResizeEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.width = static_cast<uint32_t>(new_width);
  event.height = static_cast<uint32_t>(new_height);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWFocusEventCallback(GLFWWindowHandle handle, int32_t current_state)
{
  WindowFocusEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.state = static_cast<WindowFocusEvent::FocusState>(current_state);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWKeyCallback(GLFWWindowHandle handle, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
  WindowKeyboardEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.keycode = static_cast<KeyCode>(key);
  event.modifiers = static_cast<ButtonEvent::ModifierKeys>(mods);
  event.scancode = scancode;
  event.action = static_cast<ButtonEvent::ButtonAction>(action);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWCursorPosCallback(GLFWWindowHandle handle, double x_pos, double y_pos)
{
  WindowCursorMoveEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.x_pos = static_cast<uint32_t>(x_pos);
  event.y_pos = static_cast<uint32_t>(y_pos);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWCursorEnterCallback(GLFWWindowHandle handle, int32_t entered)
{
  WindowCursorEnterEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.cursor_hover_state = static_cast<WindowCursorEnterEvent::CursorHoverState>(entered);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWMouseButtonCallback(GLFWWindowHandle handle, int32_t button, int32_t action, int32_t mods)
{
  WindowMouseButtonEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;
  event.button = static_cast<WindowMouseButtonEvent::CursorButton>(button);
  event.action = static_cast<ButtonEvent::ButtonAction>(action);
  event.modifiers = static_cast<ButtonEvent::ModifierKeys>(mods);

  GetEnvironment().GetEventBus().Publish(event);
}

void GLFWWindow::GLFWMouseScrollCallback(GLFWWindow::GLFWWindowHandle handle, double, double y_offset)
{
  WindowMouseScrollEvent event;

  event.window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));
  GLFW_ASSERT_DEBUG_ONLY;

  event.vertical_offset = static_cast<uint32_t>(y_offset);

  GetEnvironment().GetEventBus().Publish(event);
}

} // namespace genebits::engine