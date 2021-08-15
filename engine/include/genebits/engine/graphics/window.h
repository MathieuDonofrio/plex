#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <string>

#include "genebits/engine/util/enum_flag.h"

namespace genebits::engine
{
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

///
/// Window interface.
///
class Window
{
public:
  ///
  /// Default constructor
  ///
  Window() = default;

  ///
  /// Destructor
  ///
  virtual ~Window() = default;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  ///
  /// Poll the OS for events associated with this window.
  ///
  /// @note
  ///     Polling of events should be conducted every now and then to let the OS know
  ///     that the process is still responsive.
  ///
  virtual void PollEvents() = 0;

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep.
  ///
  virtual void WaitEvents() = 0;

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep.
  ///
  /// @param[in] timeout Maximum time to wait in seconds.
  ///
  virtual void WaitEvents(double timeout) = 0;

  ///
  /// Bring the window in focus.
  ///
  virtual void Focus() = 0;

  ///
  /// Maximise the size of the window according to its maximum size or the entire
  /// screen if there is no limits.
  ///
  virtual void Maximize() = 0;

  ///
  /// Minimise the size of the window according to its minimum size or the default
  /// minimum size if there is no limits.
  ///
  virtual void Iconify() = 0;

  ///
  /// Restore the window if it was iconified or maximized.
  ///
  virtual void Restore() = 0;

  ///
  /// Request the attention of the user in an non-interrupting way.
  ///
  virtual void RequestAttention() = 0;

  ///
  /// Put the window into a closing state.
  ///
  /// @note
  ///     The visual window will not be closed by this function. It only sets closing
  ///     state flags. Window destruction happens upon destruction of the window object.
  ///
  virtual void Close() = 0;

  ///
  /// Resize the window drawable area
  ///
  /// @param[in] width New width in pixels of the drawable area.
  /// @param[in] height New height in pixels of the drawable area.
  ///
  virtual void Resize(uint32_t width, uint32_t height) = 0;

  ///
  /// Set the title of the window.
  ///
  /// @param[in] title New title of the window.
  ///
  virtual void SetTitle(const std::string& title) = 0;

  ///
  /// Get the title of the window
  ///
  /// @return const string ref of the window title
  ///
  [[nodiscard]] virtual const std::string& GetTitle() const = 0;

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
  virtual void SetIcon(uint8_t* pixels, uint32_t width, uint32_t height) = 0;

  ///
  /// Get the width in screen coordinate of the current monitor the window is on.
  ///
  /// @return Width in screen coordinate of the monitor.
  ///
  [[nodiscard]] virtual uint32_t GetMonitorWidth() const = 0;

  ///
  /// Get the height in screen coordinate of the current monitor the window is on.
  ///
  /// @return Height in screen coordinate of the monitor.
  ///
  [[nodiscard]] virtual uint32_t GetMonitorHeight() const = 0;

  ///
  /// Get the current width in pixels of the drawable area.
  ///
  /// @return Current width in pixels of the drawable area.
  ///
  [[nodiscard]] virtual uint32_t GetWidth() const = 0;

  ///
  /// Get the current height in pixels of the drawable area.
  ///
  /// @return Current height in pixels of the drawable area.
  ///
  [[nodiscard]] virtual uint32_t GetHeight() const = 0;

  ///
  /// Get the closed state of the window.
  ///
  /// @return Closed state of the window.
  ///
  [[nodiscard]] virtual bool IsClosing() const = 0;

  ///
  /// Get the iconified state of the window.
  ///
  /// @return Iconified state of the window.
  ///
  [[nodiscard]] virtual bool IsIconified() const = 0;

  ///
  /// Get the maximized state of the window.
  ///
  /// @return Maximized state of the window.
  ///
  [[nodiscard]] virtual bool IsMaximized() const = 0;

  ///
  /// Get the focused state of the window.
  ///
  /// @return Focused state of the window.
  ///
  [[nodiscard]] virtual bool IsFocused() const = 0;

  ///
  /// Get the visible state of the window.
  ///
  /// @return Visible state of the window.
  ///
  [[nodiscard]] virtual bool IsVisible() const = 0;

  ///
  /// Set the refresh rate of the window when fullscreen.
  ///
  /// @param[in] refresh_rate Rate of refresh in frames per second.
  ///
  /// @note A value of 0 will disable the refresh rate limit.
  ///
  virtual void SetFullScreenRefreshRate(uint32_t refresh_rate) = 0;
};

///
/// Window event base.
///
/// Contains the window pointer.
///
struct WindowEvent
{
  Window* window;
};

///
/// Window resize event.
///
/// Published when a window resizes.
///
struct WindowResizeEvent : public WindowEvent
{
  uint32_t width;
  uint32_t height;
};

///
/// Window close event.
///
/// Published when a user attempts to close a window.
///
struct WindowCloseEvent : public WindowEvent
{};

///
/// Window focus event.
///
/// Published when a window gains or loses input focus.
///
struct WindowFocusEvent : public WindowEvent
{
  enum class FocusState
  {
    Lost = 0,
    Gained = 1
  };

  FocusState state;
};

///
/// Window iconify event.
///
/// Published when a window is iconified or restored
///
struct WindowIconifyEvent : public WindowEvent
{
  bool iconified;
};

///
/// Window maximize event.
///
/// Published when a window is maximized or restored.
///
struct WindowMaximizeEvent : public WindowEvent
{
  bool maximized;
};

///
/// Creates a window.
///
/// Factory method for creating windows.
///
/// @param title Title of the window.
/// @param width Width of the window.
/// @param height Height of the window.
/// @param hints Window creation hints.
///
/// @return Window instance pointer.
///
Window* CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints = WindowCreationHints::Defaults);

} // namespace genebits::engine
#endif
