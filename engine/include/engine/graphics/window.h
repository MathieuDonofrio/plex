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

  //void Focus();

  //void Maximize();

  //void Minimize();

  //void Restore();

  //void Close();

  void Resize(uint32_t width, uint32_t height);

  void SetTitle(const std::string& title);

  const std::string& GetTitle() const;

  //uint32_t GetMonitorWidth();

  //uint32_t GetMonitorHeight();

  uint32_t GetWidth() const;

  uint32_t GetHeight() const;

  //uint32_t GetMinimumWidth();

  //uint32_t GetMinimumHeight();

  //uint32_t GetMaximumWidth();

  //uint32_t GetMaximumHeight();

private:
  struct Pimpl;
  Pimpl* pimpl_;
};
} // namespace genebits::engine

#endif
