#ifndef GENEBITS_ENGINE_GRAPHICS_GLFWWINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_GLFWWINDOW_H

#include <cstdint>
#include <string>

#include "genebits/engine/graphics/vulkan_capable_window.h"
#include "genebits/engine/graphics/window.h"
#include "genebits/engine/util/enum_flag.h"

// Forward declare
class GLFWwindow; // from glfw

namespace genebits::engine
{
///
/// Vulkan capable GLFW window.
///
/// @see https://www.glfw.org/docs/3.3/group__window.html
/// @see https://www.glfw.org/docs/3.3.1/vulkan_guide.html
///
class GLFWWindow : public Window, public VulkanCapableWindow
{
public:
  ///
  /// Parametric constructor.
  ///
  /// @param[in] title Title of the window.
  /// @param[in] width The width in pixels of the drawable area .
  /// @param[in] height The height in pixels of the drawable area.
  /// @param[in] hints Hints used to create the window.
  ///
  GLFWWindow(const std::string& title,
    uint32_t width,
    uint32_t height,
    WindowCreationHints hints = WindowCreationHints::Defaults);

  ///
  /// Destructor
  ///
  ~GLFWWindow() override;

  ///
  /// Poll the OS for events associated with this window.
  ///
  /// @note Polling of events should be conducted every now and then to let the OS know that the process is still
  /// responsive.
  ///
  void PollEvents() override;

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep.
  ///
  void WaitEvents() override;

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep.
  ///
  /// @param[in] timeout Maximum time to wait in seconds.
  ///
  void WaitEvents(double timeout) override;

  ///
  /// Bring the window in focus.
  ///
  void Focus() override;

  ///
  /// Maximise the size of the window according to its maximum size or the entire screen if there is no limits.
  ///
  void Maximize() override;

  ///
  /// Minimise the size of the window according to its minimum size or the default minimum size if there is no limits.
  ///
  void Iconify() override;

  ///
  /// Restore the window if it was iconified or maximized.
  ///
  void Restore() override;

  ///
  /// Request the attention of the user in an non-interrupting way.
  ///
  void RequestAttention() override;

  ///
  /// Put the window into a closing state.
  ///
  /// @note The visual window will not be closed by this function.
  ///       It only sets closing state flags.
  ///       Window destruction happens upon destruction of the window object.
  ///
  void Close() override;

  ///
  /// Resize the window drawable area
  ///
  /// @param[in] width New width in pixels of the drawable area.
  /// @param[in] height New height in pixels of the drawable area.
  ///
  void Resize(uint32_t width, uint32_t height) override;

  ///
  /// Set the title of the window.
  ///
  /// @param[in] title New title of the window.
  ///
  void SetTitle(const std::string& title) override;

  ///
  /// Get the title of the window
  ///
  /// @return const string ref of the window title
  ///
  [[nodiscard]] const std::string& GetTitle() const override;

  ///
  /// Set the icon for the window.
  ///
  /// @param[in] pixels Pointer to an array of pixels in the RGBA format.
  /// @param[in] width Width in pixels of the icon.
  /// @param[in] height height in pixels of the icon.
  ///
  /// @note Preferred sizes are: 16x16, 32x32 and 48x48.
  ///
  /// @note To remove to the window's icon, pass in a nullptr for the pixels pointer
  ///
  void SetIcon(uint8_t* pixels, uint32_t width, uint32_t height) override;

  ///
  /// Get the width in screen coordinate of the current monitor the window is on.
  ///
  /// @return Width in screen coordinate of the monitor.
  ///
  [[nodiscard]] uint32_t GetMonitorWidth() const override;

  ///
  /// Get the height in screen coordinate of the current monitor the window is on.
  ///
  /// @return Height in screen coordinate of the monitor.
  ///
  [[nodiscard]] uint32_t GetMonitorHeight() const override;

  ///
  /// Get the current width in pixels of the drawable area.
  ///
  /// @return Current width in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetWidth() const override;

  ///
  /// Get the current height in pixels of the drawable area.
  ///
  /// @return Current height in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetHeight() const override;

  ///
  /// Get the closed state of the window.
  ///
  /// @return Closed state of the window.
  ///
  [[nodiscard]] bool IsClosing() const override;

  ///
  /// Get the iconified state of the window.
  ///
  /// @return Iconified state of the window.
  ///
  [[nodiscard]] bool IsIconified() const override;

  ///
  /// Get the maximised state of the window.
  ///
  /// @return Maximised state of the window.
  ///
  [[nodiscard]] bool IsMaximized() const override;

  ///
  /// Get the focused state of the window.
  ///
  /// @return Focused state of the window.
  ///
  [[nodiscard]] bool IsFocused() const override;

  ///
  /// Get the visible state of the window.
  ///
  /// @return Visible state of the window.
  ///
  [[nodiscard]] bool IsVisible() const override;

  ///
  /// Set the refresh rate of the window when fullscreen.
  ///
  /// @param[in] refresh_rate Rate of refresh in frames per second.
  ///
  /// @note A value of 0 will disable the refresh rate limit.
  ///
  void SetFullScreenRefreshRate(uint32_t refresh_rate) override;

  ///
  /// Creates a Vulkan surface for the window's drawable area.
  ///
  /// @param[in] instance Vulkan instance of the application.
  ///
  /// @Return Vulkan surface
  ///
  VkSurfaceKHR* CreateWindowSurface(VkInstance instance) override;

  ///
  /// Returns the names of vulkan instance extensions required by the window API to create
  /// vulkan surfaces for the window.
  ///
  /// @return Vulkan instance extensions required for the window.
  ///
  VulkanInstanceExtensions GetRequiredInstanceExtensions() override;

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

private:
  GLFWWindowHandle handle_;

  std::string title_;
};

} // namespace genebits::engine

#endif
