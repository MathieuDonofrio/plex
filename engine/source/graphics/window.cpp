
#include "engine/graphics/window.h"

#include "GLFW/glfw3.h"

namespace genebits::engine
{
struct Window::Pimpl
{
  GLFWwindow* handle_;

  std::string title_;
};

Window::Window(const std::string& title, uint32_t width, uint32_t height)
  : pimpl_(new Pimpl)
{
  pimpl_->title_ = title;

  glfwInit(); // Assert this

  pimpl_->handle_ = glfwCreateWindow(
    static_cast<int>(width), static_cast<int>(height), pimpl_->title_.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(pimpl_->handle_);
}

Window::~Window()
{
  glfwDestroyWindow(pimpl_->handle_);

  delete pimpl_;
  pimpl_ = nullptr;
}

const std::string& Window::GetTitle() const
{
  return pimpl_->title_;
}

void Window::SetTitle(const std::string& title)
{
  pimpl_->title_ = title;

  glfwSetWindowTitle(pimpl_->handle_, pimpl_->title_.c_str());
}

uint32_t Window::GetWidth() const
{
  int width;

  glfwGetWindowSize(pimpl_->handle_, &width, nullptr);

  return static_cast<uint32_t>(width);
}

uint32_t Window::GetHeight() const
{
  int height;

  glfwGetWindowSize(pimpl_->handle_, nullptr, &height);

  return static_cast<uint32_t>(height);
}

void Window::Resize(uint32_t width, uint32_t height)
{
  glfwSetWindowSize(pimpl_->handle_, width, height);
}

} // namespace genebits::engine