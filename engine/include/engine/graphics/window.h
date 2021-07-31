#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <cstdint>
#include <functional>
#include <string>

#include <engine/util/enum_flag.h>

namespace genebits::engine
{

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
  Defaults = Resizable | Visible | Decorated | Focused | AutoIconified | CursorCentered | FocusingOnShow
};

class Window
{
public:
  using WindowClosingCallback = std::function<void(Window*)>;

  Window(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints = WindowCreationHints::None);

  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  void Create();

  void Focus();

  void Maximize();

  void Minimize();

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

  void Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions = false);

  ///
  /// Set the title of the window.
  ///
  /// @param[in] title New title of the window.
  ///
  void SetTitle(const std::string& title);

  [[nodiscard]] const std::string& GetTitle() const;

  void SetIcon(uint8_t* pixels, uint32_t width, uint32_t height);

  [[nodiscard]] uint32_t GetMonitorWidth() const;

  [[nodiscard]] uint32_t GetMonitorHeight() const;

  [[nodiscard]] uint32_t GetWidth() const;

  [[nodiscard]] uint32_t GetHeight() const;

  [[nodiscard]] uint32_t GetMinimumWidth() const noexcept;

  [[nodiscard]] uint32_t GetMinimumHeight() const noexcept;

  [[nodiscard]] uint32_t GetMaximumWidth() const;

  [[nodiscard]] uint32_t GetMaximumHeight() const;

  void SetMaximumWidth(uint32_t width);

  void SetMaximumHeight(uint32_t height);

  void SetMinimumWidth(uint32_t width);

  void SetMinimumHeight(uint32_t height);

  ///
  /// Get the closed state of the window.
  ///
  /// @return Closed state of the window.
  ///
  [[nodiscard]] bool IsClosed() const noexcept;

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

  void SetWindowClosingCallback(WindowClosingCallback window_closing_callback);

  template<typename InstanceType, typename SurfaceType>
  void CreateWindowSurface(InstanceType instance, SurfaceType* surface);

private:
  struct Pimpl;
  Pimpl* pimpl_;
};

} // namespace genebits::engine

#endif
