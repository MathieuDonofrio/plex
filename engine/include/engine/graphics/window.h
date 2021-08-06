#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <cstdint>
#include <functional>
#include <string>

#include <engine/util/enum_flag.h>

namespace genebits::engine
{
// TODO Add method to query those flags from the window
ENUM_FLAGS(WindowCreationHints, uint64_t) {
  None = 0,
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
  Defaults = ~0ull
};

class Window
{
public:
  using WindowClosingCallback = std::function<void(Window*)>;

  ///
  /// Window constructor.
  ///
  /// @param[in] title Title of the window.
  /// @param[in] width The width in pixels of the drawable area .
  /// @param[in] height The height in pixels of the drawable area.
  /// @param[in] window_creation_hints Hints used to create the window.
  ///
  Window(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints = WindowCreationHints::Defaults);

  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  ///
  /// Poll the OS for events associated with this window.
  ///
  /// @note Polling of events should be conducted every now and then to let the OS know that the process is still responsive.
  ///
  void PollEvents();

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep.
  ///
  /// @param[in] Optional maximum time to wait in seconds.
  ///
  void WaitEvents(double timeout = 0.0);

  ///
  /// Bring the window in focus.
  ///
  void Focus();

  ///
  /// Maximise the size of the window according to its maximum size or the entire screen if there is no limits.
  ///
  void Maximize();

  ///
  /// Minimise the size of the window according to its minimum size or the default minimum size if there is no limits.
  ///
  void Iconify();

  ///
  /// Restore the window if it was iconified or maximized.
  ///
  void Restore();

  ///
  /// Request the attention of the user in an non-interrupting way.
  ///
  void RequestAttention();

  ///
  /// Put the window into a closing state.
  ///
  /// @note The visual window will not be closed by this function.
  ///       It only sets closing state flags.
  ///       Window destruction happens upon destruction of the window object.
  ///
  void Close();

  ///
  /// Resize the window drawable area
  ///
  /// @param[in] width New width in pixels of the drawable area.
  /// @param[in] height New height in pixels of the drawable area.
  /// @param[in] overwrite_max_dimension Optional, overwrite the maximum dimensions.
  ///
  /// @note Does nothing if the window is maximised.
  ///
  void Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions = false);

  ///
  /// Set the title of the window.
  ///
  /// @param[in] title New title of the window.
  ///
  void SetTitle(const std::string& title);

  ///
  /// Get the title of the window
  ///
  /// @return const string ref of the window title
  ///
  [[nodiscard]] const std::string& GetTitle() const;

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
  void SetIcon(uint8_t* pixels, uint32_t width, uint32_t height);

  ///
  /// Get the width in screen coordinate of the current monitor the window is on.
  ///
  /// @return Width in screen coordinate of the monitor.
  ///
  [[nodiscard]] uint32_t GetMonitorWidth() const;

  ///
  /// Get the height in screen coordinate of the current monitor the window is on.
  ///
  /// @return Height in screen coordinate of the monitor.
  ///
  [[nodiscard]] uint32_t GetMonitorHeight() const;

  ///
  /// Get the current width in pixels of the drawable area.
  ///
  /// @return Current width in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetWidth() const;

  ///
  /// Get the current height in pixels of the drawable area.
  ///
  /// @return Current height in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetHeight() const;

  ///
  /// Get the minimum width in pixels of the drawable area.
  ///
  /// @return Minimum width in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetMinimumWidth() const noexcept;

  ///
  /// Get the minimum height in pixels of the drawable area.
  ///
  /// @return Minimum height in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetMinimumHeight() const noexcept;

  ///
  /// Get the maximum width in pixels of the drawable area.
  ///
  /// @return Maximum width in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetMaximumWidth() const;

  ///
  /// Get the maximum height in pixels of the drawable area.
  ///
  /// @return Maximum height in pixels of the drawable area.
  ///
  [[nodiscard]] uint32_t GetMaximumHeight() const;

  ///
  /// Set the maximum width in pixels of the drawable area.
  ///
  /// @param[in] width New maximum width in pixels of the drawable area.
  ///
  void SetMaximumWidth(uint32_t width);

  ///
  /// Set the maximum height in pixels of the drawable area.
  ///
  /// @param[in] height New maximum height in pixels of the drawable area.
  ///
  void SetMaximumHeight(uint32_t height);

  ///
  /// Set the minimum width in pixels of the drawable area.
  ///
  /// @param[in] height New minimum width in pixels of the drawable area.
  ///
  void SetMinimumWidth(uint32_t width);

  ///
  /// Set the minimum height in pixels of the drawable area.
  ///
  /// @param[in] height New minimum height in pixels of the drawable area.
  ///
  void SetMinimumHeight(uint32_t height);

  ///
  /// Get the closed state of the window.
  ///
  /// @return Closed state of the window.
  ///
  [[nodiscard]] bool IsClosing() const noexcept;

  ///
  /// Get the iconified state of the window.
  ///
  /// @return Iconified state of the window.
  ///
  [[nodiscard]] bool IsIconified() const;

  ///
  /// Get the maximised state of the window.
  ///
  /// @return Maximised state of the window.
  ///
  [[nodiscard]] bool IsMaximised() const;

  ///
  /// Get the focused state of the window.
  ///
  /// @return Focused state of the window.
  ///
  [[nodiscard]] bool IsFocused() const;

  ///
  /// Get the visible state of the window.
  ///
  /// @return Visible state of the window.
  ///
  [[nodiscard]] bool IsVisible() const;

  ///
  /// Set the refresh rate of the window when fullscreen.
  ///
  /// @param[in] refresh_rate Rate of refresh in frames per second.
  ///
  /// @note A value of 0 will disable the refresh rate limit.
  ///
  void SetFullScreenRefreshRate(uint64_t refresh_rate);

  ///
  /// Set the function that will be called when the window enters a closing state.
  ///
  /// @param[in] Function to be called when the window enters a closing state.
  ///
  /// @Note The callback does not occur if Close() is called. Only closing initiated by the user will do the callback.
  ///
  void SetWindowClosingCallback(WindowClosingCallback window_closing_callback);

  ///
  /// Creates a Vulkan surface for the window's drawable area.
  ///
  /// @param[in] instance Vulkan instance of the application.
  /// @param[out] surface Pointer to the Vulkan surface to create the surface at.
  ///
  template<typename InstanceType, typename SurfaceType>
  void CreateWindowSurface(InstanceType instance, SurfaceType* surface);

private:
  struct Pimpl;
  Pimpl* pimpl_;
};

} // namespace genebits::engine

#endif
