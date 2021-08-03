#include "engine/graphics/window.h"

#include <gtest/gtest.h>

namespace genebits::engine::test
{

class TestFixtureWindow : public ::testing::Test
{
protected:
  Window* GetWindowPtr()
  {
    return instance_;
  }
  void CreateWindowObject(const std::string& title, uint32_t width, uint32_t height, WindowCreationHints window_creation_hints)
  {
    instance_ = new Window(title, width, height, window_creation_hints);
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
  Window* instance_;
};

TEST_F(TestFixtureWindow, Constructor)
{
  std::string title = "Title";
  const uint64_t width = 512;
  const uint64_t height = 256;

  CreateWindowObject(title, width, height, WindowCreationHints::None);
  GetWindowPtr()->Create();

  ASSERT_EQ(GetWindowPtr()->GetTitle(), title);
  title = "different"; // To make sure the title is being copied
  ASSERT_NE(GetWindowPtr()->GetTitle(), title);

  ASSERT_EQ(GetWindowPtr()->GetWidth(), width);
  ASSERT_EQ(GetWindowPtr()->GetHeight(), height);
}

TEST_F(TestFixtureWindow, Hints_None)
{
  std::string title = "Title";
  const uint64_t width = 512;
  const uint64_t height = 256;

  CreateWindowObject(title, width, height, WindowCreationHints::None);
  GetWindowPtr()->Create();

  ASSERT_FALSE(GetWindowPtr()->IsFocused());
  ASSERT_FALSE(GetWindowPtr()->IsMaximised());
  ASSERT_FALSE(GetWindowPtr()->IsIconified());
}

TEST_F(TestFixtureWindow, Hints_All)
{
  std::string title = "Title";
  const uint64_t width = 512;
  const uint64_t height = 256;

  WindowCreationHints all_hints = WindowCreationHints::AutoIconified | WindowCreationHints::Maximised | WindowCreationHints::Focused | WindowCreationHints::Visible;
  CreateWindowObject(title, width, height, all_hints);
  GetWindowPtr()->Create();

  ASSERT_TRUE(GetWindowPtr()->IsFocused());
  ASSERT_TRUE(GetWindowPtr()->IsMaximised());
  ASSERT_FALSE(GetWindowPtr()->IsIconified());
}
} // namespace genebits::engine::test