#include "plex/scheduler/stage.h"

#include <gtest/gtest.h>

namespace plex::tests
{
namespace
{
  template<size_t id>
  void SystemMock()
  {
    [[maybe_unused]] std::atomic_size_t vid = 0;
    vid = id;
  }
} // namespace

TEST(Stage_Tests, Constructor_Nothing_NoSystems)
{
  Stage stage;

  EXPECT_EQ(stage.GetSystemCount(), 0);
}

TEST(Stage_Tests, AddSystem_Single_SystemAdded)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);

  EXPECT_EQ(stage.GetSystemCount(), 1);
  EXPECT_TRUE(stage.ContainsSystem(SystemMock<0>));
  EXPECT_FALSE(stage.ContainsSystem(SystemMock<1>));
}

TEST(Stage_Tests, AddSystem_Multiple_SystemsAdded)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>);
  stage.AddSystem(SystemMock<2>);
  stage.AddSystem(SystemMock<3>);

  EXPECT_EQ(stage.GetSystemCount(), 4);
  EXPECT_TRUE(stage.ContainsSystem(SystemMock<0>));
  EXPECT_TRUE(stage.ContainsSystem(SystemMock<1>));
  EXPECT_TRUE(stage.ContainsSystem(SystemMock<2>));
  EXPECT_TRUE(stage.ContainsSystem(SystemMock<3>));
  EXPECT_FALSE(stage.ContainsSystem(SystemMock<4>));
}

TEST(Stage_Tests, IsExplicitOrder_NoExplicitOrdering_NoOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);

  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system0));
}

TEST(Stage_Tests, IsExplicitOrder_TwoSystemsAfterOrder_CorrectOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>).After(SystemMock<0>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);

  EXPECT_TRUE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system0));
}

TEST(Stage_Tests, IsExplicitOrder_ThreeSystemsAfterOrder_CorrectOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>).After(SystemMock<0>);
  stage.AddSystem(SystemMock<2>).After(SystemMock<1>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);
  auto system2 = stage.GetSystemObject(SystemMock<2>);

  EXPECT_TRUE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system0));
  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system2));
  EXPECT_FALSE(stage.HasExplicitOrder(*system2, *system0));
  EXPECT_TRUE(stage.HasExplicitOrder(*system1, *system2));
  EXPECT_FALSE(stage.HasExplicitOrder(*system2, *system1));
}

TEST(Stage_Tests, IsExplicitOrder_TwoSystemsBeforeOrder_CorrectOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>).Before(SystemMock<0>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);

  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_TRUE(stage.HasExplicitOrder(*system1, *system0));
}

TEST(Stage_Tests, IsExplicitOrder_ThreeSystemsBeforeOrder_CorrectOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>).Before(SystemMock<0>);
  stage.AddSystem(SystemMock<2>).Before(SystemMock<1>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);
  auto system2 = stage.GetSystemObject(SystemMock<2>);

  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_TRUE(stage.HasExplicitOrder(*system1, *system0));
  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system2));
  EXPECT_FALSE(stage.HasExplicitOrder(*system2, *system0));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system2));
  EXPECT_TRUE(stage.HasExplicitOrder(*system2, *system1));
}

TEST(Stage_Tests, IsExplicitOrder_MixedOrder_CorrectOrdering)
{
  Stage stage;

  stage.AddSystem(SystemMock<0>);
  stage.AddSystem(SystemMock<1>);
  stage.AddSystem(SystemMock<2>).Before(SystemMock<1>).After(SystemMock<0>);

  auto system0 = stage.GetSystemObject(SystemMock<0>);
  auto system1 = stage.GetSystemObject(SystemMock<1>);
  auto system2 = stage.GetSystemObject(SystemMock<2>);

  EXPECT_FALSE(stage.HasExplicitOrder(*system0, *system1));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system0));
  EXPECT_TRUE(stage.HasExplicitOrder(*system0, *system2));
  EXPECT_FALSE(stage.HasExplicitOrder(*system2, *system0));
  EXPECT_FALSE(stage.HasExplicitOrder(*system1, *system2));
  EXPECT_TRUE(stage.HasExplicitOrder(*system2, *system1));
}
} // namespace plex::tests