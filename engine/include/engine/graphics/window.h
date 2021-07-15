#ifndef GENEBITS_ENGINE_GRAPHICS_WINDOW_H
#define GENEBITS_ENGINE_GRAPHICS_WINDOW_H

#include <cstdint>
#include <string>

namespace genebits::engine
{
class Window
{
public:
  Window(const std::string& title, uint32_t width, uint32_t height);

  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

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

  template<typename InstanceType, typename SurfaceType>
  void createWindowSurface(InstanceType instance, SurfaceType* surface);

private:
  struct Pimpl;
  Pimpl* pimpl_;
};
} // namespace genebits::engine

#endif
