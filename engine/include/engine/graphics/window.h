#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <cstdint>
#include <functional>
#include <string>

namespace genebits::engine
{

enum class WindowCreationHints : uint64_t
{
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
};

constexpr WindowCreationHints operator&(const WindowCreationHints& left, const WindowCreationHints& right)
{
  using enum_type = std::underlying_type_t<WindowCreationHints>;
  return static_cast<WindowCreationHints>(static_cast<enum_type>(left) & static_cast<enum_type>(right));
}

constexpr WindowCreationHints operator|(const WindowCreationHints& left, const WindowCreationHints& right)
{
  using enum_type = std::underlying_type_t<WindowCreationHints>;
  return static_cast<WindowCreationHints>(static_cast<enum_type>(left) | static_cast<enum_type>(right));
}

constexpr bool operator==(const WindowCreationHints& left, const std::underlying_type_t<WindowCreationHints>& right)
{
  using enum_type = std::underlying_type_t<WindowCreationHints>;
  return static_cast<enum_type>(left) == right;
}

constexpr bool operator!=(const WindowCreationHints& left, const std::underlying_type_t<WindowCreationHints>& right)
{
  using enum_type = std::underlying_type_t<WindowCreationHints>;
  return static_cast<enum_type>(left) != right;
}

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

  void Close();

  void Resize(uint32_t width, uint32_t height, bool overwrite_max_dimensions = false);

  [[nodiscard]] bool IsClosed() const noexcept;

  //glfwSetWindowCloseCallback

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

  // Use 0 to disable refresh rate limit
  static void SetFullScreenRefreshRate(uint64_t refresh_rate);

  void SetWindowClosingCallback(WindowClosingCallback window_closing_callback);

  template<typename InstanceType, typename SurfaceType>
  void CreateWindowSurface(InstanceType instance, SurfaceType* surface);

private:
protected:
  struct Pimpl;
  Pimpl* pimpl_;
  static void ApplyWindowCreationHints(const WindowCreationHints& hints);
};

} // namespace genebits::engine

#endif
