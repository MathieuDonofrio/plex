#ifndef GENEBITS_ENGINE_GRAPHICS_GLFWWINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_GLFWWINDOW_H

#include <cstdint>
#include <functional>
#include <string>

#include "vulkan/vulkan_core.h"

#include "engine/graphics/window.h"
#include "engine/util/enum_flag.h"

// Forward declare
class GLFWwindow;

namespace genebits::engine
{
// TODO Add method to query those flags from the window
ENUM_FLAGS(WindowCreationHints, uint64_t) { None = 0,
  Resizable = 1 << 0,
  Visible = 1 << 1,
  Decorated = 1 << 2,
  Focused = 1 << 3,
  AutoIconified = 1 << 4,
  FocusingOnShow = 1 << 5,
  Floating = 1 << 6,
  Maximised = 1 << 7,
  CursorCentered = 1 << 8,
  TransparentFramebuffer = 1 << 9,
  ScalingToMonitor = 1 << 10,
  Defaults = ~0ull };

// TODO Create Abstract window class

class GLFWWindow : public Window
{
public:
  ///
  /// Window constructor.
  ///
  /// @param[in] title Title of the window.
  /// @param[in] width The width in pixels of the drawable area .
  /// @param[in] height The height in pixels of the drawable area.
  /// @param[in] window_creation_hints Hints used to create the window.
  ///
  GLFWWindow(const std::string& title,
    uint32_t width,
    uint32_t height,
    WindowCreationHints window_creation_hints = WindowCreationHints::Defaults);

  ~GLFWWindow();

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
  void WaitEvents(double timeout = 0.0) override;

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
  [[nodiscard]] bool IsMaximised() const override;

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
  void SetFullScreenRefreshRate(uint64_t refresh_rate) override;

  ///
  /// Creates a Vulkan surface for the window's drawable area.
  ///
  /// @param[in] instance Vulkan instance of the application.
  /// @param[out] surface Pointer to the Vulkan surface to create the surface at.
  ///
  void CreateVulkanWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:
  using GLFWWindowHandle = ::GLFWwindow*;

  ///
  /// Applies window creation hints to the next window to be created.
  ///
  /// @param hints[in] Hints to apply
  ///
  void ApplyWindowCreationHints(const WindowCreationHints& hints);

  ///
  /// Registers all the glfw window callbacks
  ///
  void RegisterGLFWWindowCallbacks();

  static void GLFWCloseEventCallback(GLFWWindowHandle window);

  static void GLFWMaximiseEventCallback(GLFWWindowHandle window, int32_t current_state);

  static void GLFWIconifyEventCallback(GLFWWindowHandle window, int32_t current_state);

  static void GLFWResizeEventCallback(GLFWWindowHandle window, int32_t new_width, int32_t new_height);

  static void GLFWFocusEventCallback(GLFWWindowHandle window, int32_t current_state);

private:
  GLFWWindowHandle handle_;

  std::string title_;
};

} // namespace genebits::engine

#endif
