#include "../../source/graphics/glfw_window.h"

#include <gtest/gtest.h>

namespace genebits::engine::test
{

class TestFixtureWindow : public ::testing::Test
{
protected:
  uint32_t GetDefaultWidth()
  {
    return width_;
  }

  uint32_t GetDefaultHeight()
  {
    return height_;
  }

  std::string GetDefaultTitle()
  {
    return title_;
  }

  GLFWWindow* GetWindowPtr()
  {
    return instance_;
  }

  void CreateWindow(
    const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  {
    instance_ = new GLFWWindow(title, width, height, window_creation_hints);
  }

  void CreateWindow(WindowCreationHints hints = WindowCreationHints::Defaults)
  {
    CreateWindow(GetDefaultTitle(), GetDefaultWidth(), GetDefaultHeight(), hints);
  }

  void TearDown() override
  {
    if (instance_ != nullptr)
    {
      delete instance_;
      instance_ = nullptr;
    }
  }

private:
  GLFWWindow* instance_;

  const std::string title_ = "Title";

  const uint32_t width_ = 512;
  const uint32_t height_ = 256;
};

TEST_F(TestFixtureWindow, CreateWindow_Construction_CorrectValues)
{
  std::string title = "Title";
  const uint32_t width = 512;
  const uint32_t height = 256;

  CreateWindow(title, width, height, WindowCreationHints::None);

  ASSERT_EQ(GetWindowPtr()->GetTitle(), title);
  title = "different"; // To make sure the title is being copied
  ASSERT_NE(GetWindowPtr()->GetTitle(), title);

  ASSERT_EQ(GetWindowPtr()->GetWidth(), width);
  ASSERT_EQ(GetWindowPtr()->GetHeight(), height);
}

TEST_F(TestFixtureWindow, SetTitle_NewTitle_CorrectTitle)
{
  const std::string new_title = "DifferentTitle";
  CreateWindow();
  GetWindowPtr()->SetTitle(new_title);
  ASSERT_TRUE(GetWindowPtr()->GetTitle() == new_title);
}

TEST_F(TestFixtureWindow, Resize_ResizeWidth_CorrectWidth)
{
  const uint32_t new_width = 800;
  CreateWindow();
  GetWindowPtr()->Resize(new_width, GetDefaultHeight());
  ASSERT_EQ(GetWindowPtr()->GetWidth(), new_width);
}

TEST_F(TestFixtureWindow, Resize_ResizeHeight_CorrectHeight)
{
  const uint32_t new_height = 500;
  CreateWindow();
  GetWindowPtr()->Resize(GetDefaultWidth(), new_height);
  ASSERT_EQ(GetWindowPtr()->GetHeight(), new_height);
}

TEST_F(TestFixtureWindow, CreateWindow_FocusFlag_Focused)
{
  auto hints = WindowCreationHints::Visible;
  hints |= WindowCreationHints::Focused;
  hints |= WindowCreationHints::FocusingOnShow;
  CreateWindow(hints);
  ASSERT_TRUE(GetWindowPtr()->IsFocused());
}

TEST_F(TestFixtureWindow, CreateWindow_NoFocusFlag_NotFocused)
{
  auto hints = WindowCreationHints::Visible;
  hints &= ~WindowCreationHints::Focused;
  hints &= ~WindowCreationHints::FocusingOnShow;
  CreateWindow(hints);
  ASSERT_FALSE(GetWindowPtr()->IsFocused());
}

TEST_F(TestFixtureWindow, CreateWindow_MaximiseFlag_Maximised)
{
  auto hints = WindowCreationHints::Visible;
  hints |= WindowCreationHints::Maximised;
  CreateWindow(hints);
  ASSERT_TRUE(GetWindowPtr()->IsMaximised());
}

TEST_F(TestFixtureWindow, CreateWindow_NoMaximiseHint_NotMaximised)
{
  auto hints = WindowCreationHints::Visible;
  hints &= ~WindowCreationHints::Maximised;
  CreateWindow(hints);
  ASSERT_FALSE(GetWindowPtr()->IsMaximised());
}

TEST_F(TestFixtureWindow, CreateWindow_VisibleFlag_Visible)
{
  auto hints = WindowCreationHints::Visible;
  hints |= WindowCreationHints::Maximised;
  CreateWindow(hints);
  ASSERT_TRUE(GetWindowPtr()->IsVisible());
}

TEST_F(TestFixtureWindow, CreateWindow_NoVisibleFlag_NotVisible)
{
  auto hints = WindowCreationHints::None;
  CreateWindow(hints);
  ASSERT_FALSE(GetWindowPtr()->IsVisible());
}

TEST_F(TestFixtureWindow, Focus_FocusWindow_Focused)
{
  auto hints = WindowCreationHints::Visible;
  hints &= ~WindowCreationHints::Focused;
  hints &= ~WindowCreationHints::FocusingOnShow;
  CreateWindow(hints);
  GetWindowPtr()->Focus();
  ASSERT_TRUE(GetWindowPtr()->IsFocused());
}

TEST_F(TestFixtureWindow, IsMaximised_DefaultContruction_NotMaximised)
{
  CreateWindow();
  ASSERT_FALSE(GetWindowPtr()->IsMaximised());
}

TEST_F(TestFixtureWindow, Maximise_MaximiseWindow_Maximised)
{
  CreateWindow();
  GetWindowPtr()->Maximize();
  ASSERT_TRUE(GetWindowPtr()->IsMaximised());
}

TEST_F(TestFixtureWindow, CreateWindow_DefaultContruction_NotIconified)
{
  CreateWindow();
  ASSERT_FALSE(GetWindowPtr()->IsIconified());
}

TEST_F(TestFixtureWindow, Iconify_IconifyWindow_Iconified)
{
  CreateWindow();
  GetWindowPtr()->Iconify();
  ASSERT_TRUE(GetWindowPtr()->IsIconified());
}

TEST_F(TestFixtureWindow, Restore_RestoreWindow_Restored)
{
  auto hints = WindowCreationHints::None;
  CreateWindow(hints);
  GetWindowPtr()->Restore();
  ASSERT_FALSE(GetWindowPtr()->IsIconified());
}

TEST_F(TestFixtureWindow, Close_CloseWindow_Closing)
{
  CreateWindow();
  GetWindowPtr()->Close();
  ASSERT_TRUE(GetWindowPtr()->IsClosing());
}

} // namespace genebits::engine::test