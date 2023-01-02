#ifndef PLEX_GRAPHICS_GLFW_WINDOW_H
#define PLEX_GRAPHICS_GLFW_WINDOW_H

#include <cstdint>
#include <string>

#include "plex/containers/vector.h"
#include "plex/graphics/vulkan_capable_window.h"
#include "plex/graphics/window.h"
#include "plex/utilities/enum_flag.h"

namespace
{
// Forward declare
struct GLFWwindow; // from glfw
} // namespace

namespace plex
{
///
/// Vulkan capable GLFW window.
///
class GLFWWindow : public Window, public VulkanCapableWindow
{
public:
  // Useful resources:
  // - https://www.glfw.org/docs/3.3/group__window.html
  // - https://www.glfw.org/docs/3.3.1/vulkan_guide.html

  GLFWWindow(const std::string& title,
    uint32_t width,
    uint32_t height,
    EventBus* bus,
    WindowCreationHints hints = WindowCreationHints::Defaults);

  ~GLFWWindow() override;

  void PollEvents() override;

  void WaitEvents() override;

  void WaitEvents(double timeout) override;

  void Focus() override;

  void Maximize() override;

  void Iconify() override;

  void Restore() override;

  void RequestAttention() override;

  void Close() override;

  void Resize(uint32_t width, uint32_t height) override;

  void SetTitle(const std::string& title) override;

  [[nodiscard]] const std::string& GetTitle() const override;

  void SetIcon(uint8_t* pixels, uint32_t width, uint32_t height) override;

  [[nodiscard]] uint32_t GetMonitorWidth() const override;

  [[nodiscard]] uint32_t GetMonitorHeight() const override;

  [[nodiscard]] uint32_t GetWidth() const override;

  [[nodiscard]] uint32_t GetHeight() const override;

  [[nodiscard]] bool IsClosing() const override;

  [[nodiscard]] bool IsIconified() const override;

  [[nodiscard]] bool IsMaximized() const override;

  [[nodiscard]] bool IsFocused() const override;

  [[nodiscard]] bool IsVisible() const override;

  void SetFullScreenRefreshRate(uint32_t refresh_rate) override;

  [[nodiscard]] std::pair<int32_t, int32_t> GetFrameBufferSize() const override;

  void AddWindowResizeEventCallback(WindowEventCallback<WindowResizeEvent> callback) override;
  void AddWindowCloseEventCallback(WindowEventCallback<WindowCloseEvent> callback) override;
  void AddWindowFocusEventCallback(WindowEventCallback<WindowFocusEvent> callback) override;
  void AddWindowIconifyEventCallback(WindowEventCallback<WindowIconifyEvent> callback) override;
  void AddWindowMaximizeEventCallback(WindowEventCallback<WindowMaximizeEvent> callback) override;
  void AddWindowKeyboardEventCallback(WindowEventCallback<WindowKeyboardEvent> callback) override;
  void AddWindowCursorMoveEventCallback(WindowEventCallback<WindowCursorMoveEvent> callback) override;
  void AddWindowCursorEnterEventCallback(WindowEventCallback<WindowCursorEnterEvent> callback) override;
  void AddWindowMouseButtonEventCallback(WindowEventCallback<WindowMouseButtonEvent> callback) override;
  void AddWindowMouseScrollEventCallback(WindowEventCallback<WindowMouseScrollEvent> callback) override;
  void AddWindowFramebufferResizeEventCallback(WindowEventCallback<WindowFramebufferResizeEvent> callback) override;
  void RemoveWindowResizeEventCallback(WindowEventCallback<WindowResizeEvent> callback) override;
  void RemoveWindowCloseEventCallback(WindowEventCallback<WindowCloseEvent> callback) override;
  void RemoveWindowFocusEventCallback(WindowEventCallback<WindowFocusEvent> callback) override;
  void RemoveWindowIconifyEventCallback(WindowEventCallback<WindowIconifyEvent> callback) override;
  void RemoveWindowMaximizeEventCallback(WindowEventCallback<WindowMaximizeEvent> callback) override;
  void RemoveWindowKeyboardEventCallback(WindowEventCallback<WindowKeyboardEvent> callback) override;
  void RemoveWindowCursorMoveEventCallback(WindowEventCallback<WindowCursorMoveEvent> callback) override;
  void RemoveWindowCursorEnterEventCallback(WindowEventCallback<WindowCursorEnterEvent> callback) override;
  void RemoveWindowMouseButtonEventCallback(WindowEventCallback<WindowMouseButtonEvent> callback) override;
  void RemoveWindowMouseScrollEventCallback(WindowEventCallback<WindowMouseScrollEvent> callback) override;
  void RemoveWindowFramebufferResizeEventCallback(WindowEventCallback<WindowFramebufferResizeEvent> callback) override;

private:
  // Vulkan Capable Window

  ///
  /// Creates a Vulkan surface for the window's drawable area.
  ///
  /// @param[in] instance Vulkan instance of the application.
  ///
  /// @Return Vulkan surface
  ///
  VkSurfaceKHR CreateWindowSurface(VkInstance instance) override;

  ///
  /// Returns the names of vulkan instance extensions required by the window API to create
  /// vulkan surfaces for the window.
  ///
  /// @return Vulkan instance extensions required for the window.
  ///
  std::vector<const char*> GetRequiredInstanceExtensions() override;

  ///
  /// Checks whether or not a specific queue family of a physical device supports image
  /// presentation.
  ///
  /// @param[in] instance Vulkan instance.
  /// @param[in] physical_device Vulkan physical device.
  /// @param[in] queue_family_index Queue family index.
  ///
  /// @return True if queue family of physical device supports image presentation, false otherwise.
  ///
  bool GetPhysicalDevicePresentationSupport(
    VkInstance instance, VkPhysicalDevice physical_device, uint32_t queue_family_index) override;

private:
  using GLFWWindowHandle = ::GLFWwindow*;

  ///
  /// Applies window creation hints to the next window to be created.
  ///
  /// @param[in] hints Hints to apply.
  ///
  static void ApplyWindowCreationHints(const WindowCreationHints& hints);

  ///
  /// Callback for when the window is resized.
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] new_width The new width in screen coordinates.
  /// @param[in] new_height The new height in screen coordinates.
  ///
  static void GLFWResizeEventCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height);

  ///
  /// Callback for when the user attempts to close the window. Example, by clicking the close widget
  /// in the title bar.
  ///
  /// @param[in] handle GLFW window handle for the event.
  ///
  static void GLFWCloseEventCallback(GLFWWindowHandle handle);

  ///
  /// Callback for when the window gains or loses input focus.
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] current_state State of the focus.
  ///
  static void GLFWFocusEventCallback(GLFWWindowHandle handle, int32_t current_state);

  ///
  /// Callback for when the window is iconified or restored.
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] current_state State of the iconification.
  ///
  static void GLFWIconifyEventCallback(GLFWWindowHandle handle, int32_t current_state);

  ///
  /// Callback for when the window is maximized or restored.
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] current_state State of the maximization.
  ///
  static void GLFWMaximizeEventCallback(GLFWWindowHandle handle, int32_t current_state);

  ///
  /// Callback for when the window receives keyboard inputs
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] key Key code from the USB HID Usage Tables v1.12(p. 53-60) but re-arranged to map to 7-bit ASCII for
  /// printable keys
  /// @param[in] scancode Value emitted by the keyboard (hardware)
  /// @param[in] action What state is the key in: Pressed, Released or Repeated
  /// @param[in] mods Flags representing the state of modifier keys (ALT, CTRL, SHIFT, ect...)
  ///
  static void GLFWKeyCallback(GLFWWindowHandle handle, int32_t key, int32_t scancode, int32_t action, int32_t mods);

  ///
  /// Callback for when the window receives cursor movement inputs
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] x_pos Position in x coordinate of the cursor
  /// @param[in] y_pos Position in y coordinate of the cursor
  ///
  static void GLFWCursorPosCallback(GLFWWindowHandle handle, double x_pos, double y_pos);

  ///
  /// Callback for when the cursor leaves or enter the window
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] x_pos Position in x coordinate of the mouse
  /// @param[in] y_pos Position in y coordinate of the mouse
  ///
  static void GLFWCursorEnterCallback(GLFWWindowHandle handle, int32_t entered);

  ///
  /// Callback for when the window receives mouse button inputs
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] button Which button triggered the event
  /// @param[in] action What state the button is in: Pressed, Released
  /// @param[in] mods Flags representing the state of modifier keys (ALT, CTRL, SHIFT, ect...)
  ///
  static void GLFWMouseButtonCallback(GLFWWindowHandle handle, int32_t button, int32_t action, int32_t mods);

  ///
  /// Callback for when the window receives mouse scroll inputs
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] x_offset Non-conventional horizontal offset for special mouse's
  /// @param[in] y_offset  Vertical offset of the scroll
  ///
  static void GLFWMouseScrollCallback(GLFWWindowHandle handle, double, double y_offset);

  ///
  /// Callback for when the framebuffer of the window is resized.
  ///
  /// @param[in] handle GLFW window handle for the event.
  /// @param[in] new_width The new width of the buffer in pixels.
  /// @param[in] new_height The new height of the buffer in pixels.
  ///
  static void GLFWFramebufferResizeCallback(GLFWWindowHandle handle, int32_t new_width, int32_t new_height);

private:
  GLFWWindowHandle handle_;

  std::string title_;

  EventBus* bus_;

  Vector<WindowEventCallback<WindowResizeEvent>> resize_event_callbacks_;
  Vector<WindowEventCallback<WindowCloseEvent>> close_event_callbacks_;
  Vector<WindowEventCallback<WindowFocusEvent>> focus_event_callbacks_;
  Vector<WindowEventCallback<WindowIconifyEvent>> iconify_event_callbacks_;
  Vector<WindowEventCallback<WindowMaximizeEvent>> maximize_event_callbacks_;
  Vector<WindowEventCallback<WindowKeyboardEvent>> keyboard_event_callbacks_;
  Vector<WindowEventCallback<WindowCursorMoveEvent>> cursor_move_event_callbacks_;
  Vector<WindowEventCallback<WindowCursorEnterEvent>> cursor_enter_event_callbacks_;
  Vector<WindowEventCallback<WindowMouseButtonEvent>> mouse_button_event_callbacks_;
  Vector<WindowEventCallback<WindowMouseScrollEvent>> mouse_scroll_event_callbacks_;
  Vector<WindowEventCallback<WindowFramebufferResizeEvent>> framebuffer_resize_event_callbacks_;
};

} // namespace plex

#endif
