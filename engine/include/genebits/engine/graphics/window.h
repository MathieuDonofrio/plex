#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <string>

#include "genebits/engine/graphics/key_codes.h"
#include "genebits/engine/util/enumerator.h"

namespace genebits::engine
{

///
/// Enum flags used to specify features to be enabled or disabled during the creation of the window
///
enum class WindowCreationHints : uint64_t
{
  None = 0,
  Resizable = BitFlag(0),
  Visible = BitFlag(1),
  Decorated = BitFlag(2),
  Focused = BitFlag(3),
  AutoIconified = BitFlag(4),
  FocusingOnShow = BitFlag(5),
  Floating = BitFlag(6),
  Maximised = BitFlag(7),
  CursorCentered = BitFlag(8),
  TransparentFramebuffer = BitFlag(9),
  ScalingToMonitor = BitFlag(10),
  Defaults = ~0ull
};

DEFINE_ENUM_FLAG_OPERATORS(WindowCreationHints)

///
/// Window interface. Specifies the methods to be implemented for a fully functional window implementation.
/// User input handling, window manipulation and creation are the principal features of a fully fledged window.
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
  /// Same as PollEvents() but waits for events to occur by making the thread sleep indefinitely.
  /// Events like user inputs (example: focus, minimise, close, maximise, cursor movements...) will wake the thread and
  /// unblock this method.
  ///
  /// @warning This will block the thread indefinitely if no events are generated
  ///
  virtual void WaitEvents() = 0;

  ///
  /// Same as PollEvents() but waits for events to occur by making the thread sleep until a specified amount of time has
  /// elapsed.
  /// Events like user inputs (example: focus, minimise, close, maximise, cursor movements...) will wake the thread and
  /// unblock this method.
  ///
  /// @param[in] timeout Maximum time to wait in seconds.
  ///
  /// @note Safer to use than WaitEvents() without timeout as this method is guaranteed to unblock after the timeout if
  /// no events occurred
  ///
  virtual void WaitEvents(double timeout) = 0;

  ///
  /// Bring the window in front of all other windows present on the desktop.
  ///
  virtual void Focus() = 0;

  ///
  /// Maximize the size of the window according to its maximum size or the entire
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
  /// Request the attention of the user in an non-interrupting way by signaling (visually and/or audibly) the user that
  /// the window needs to be looked upon.
  ///
  /// @note The way in which attention is requested is OS dependant, which means that the behavior can differ or that no
  /// mechanism is in place.
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
/// Window button event base
///
/// Contains common data for button related events
///
struct ButtonEvent
{

  ///
  /// Flags depicting the states of modifiers keys
  ///
  enum class ModifierKeys : uint32_t
  {
    Shift = BitFlag(0),
    Control = BitFlag(1),
    Alt = BitFlag(2),
    Super = BitFlag(3),
    CapsLock = BitFlag(4),
    NumLock = BitFlag(5)
  };

  ///
  /// Enum depicting the action a key made
  ///
  enum class Action : uint32_t
  {
    Released = 1,
    Pressed = 2,
    Repeated = 3
  };

  ModifierKeys modifiers;
  Action action;
};

DEFINE_ENUM_FLAG_OPERATORS(ButtonEvent::ModifierKeys);
DEFINE_ENUM_OPERATORS(ButtonEvent::Action);

///
/// Window keyboard event.
///
/// Published when a window is in focus and receives a keyboard input.
///
struct WindowKeyboardEvent : public WindowEvent, public ButtonEvent
{
  KeyCode keycode;
  uint32_t scancode;
};

///
/// Window cursor movement event.
///
/// Published when a window is in focus and receives a cursor movement input.
///
struct WindowCursorMoveEvent : public WindowEvent
{
  int32_t pos_x;
  int32_t pos_y;
};

///
/// Window cursor enter/leave event.
///
/// Published when the cursor leaves or enter the window
///
struct WindowCursorEnterEvent : public WindowEvent
{
  ///
  /// Enum depicting if the cursor entered or left the window drawable area
  ///
  enum class CursorHoverState : uint32_t
  {
    Left = 0,
    Entered = 1,
  };

  CursorHoverState cursor_hover_state;
};

DEFINE_ENUM_OPERATORS(WindowCursorEnterEvent::CursorHoverState)

///
/// Window mouse button event.
///
/// Published when a button is pressed or released on the mouse while in the window
///
struct WindowMouseButtonEvent : public WindowEvent, public ButtonEvent
{
  ///
  /// Enum depicting which button of the mouse had caused the event
  ///
  enum class CursorButton : uint32_t
  {
    Left = 0,
    Right = 1,
    Middle = 2
  };

  CursorButton button;
};

DEFINE_ENUM_OPERATORS(WindowMouseButtonEvent::CursorButton)

///
/// Window mouse scroll event.
///
/// Published when the mouse's scroll-wheel is scrolled
///
struct WindowMouseScrollEvent : public WindowEvent
{
  int32_t vertical_offset;
};

///
/// Creates a window.
///
/// Factory method for creating windows.
///
/// @param[in] title Title of the window.
/// @param[in] width Width of the window.
/// @param[in] height Height of the window.
/// @param[in] hints Window creation hints.
///
/// @return Window instance pointer.
///
Window* CreateWindow(
  const std::string& title, uint32_t width, uint32_t height, WindowCreationHints hints = WindowCreationHints::Defaults);

} // namespace genebits::engine
#endif
