#include "genebits/engine/ecs/entity_manager.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(EntityManager_Tests, Obtain_AfterInitialization_UniqueId)
{
  EntityManager<size_t> manager;

  EXPECT_EQ(manager.CirculatingCount(), 0u);
  EXPECT_EQ(manager.RecycledCount(), 0u);

  EXPECT_EQ(manager.Obtain(), 0u);
  EXPECT_EQ(manager.Obtain(), 1u);
  EXPECT_EQ(manager.Obtain(), 2u);

  EXPECT_EQ(manager.RecycledCount(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 3u);
}

TEST(EntityManager_Tests, Release_AfterInitialization_IncreaseRecycledCount)
{
  EntityManager<size_t> manager;

  EXPECT_EQ(manager.RecycledCount(), 0u);

  manager.Release(manager.Obtain());

  EXPECT_EQ(manager.RecycledCount(), 1u);
  EXPECT_EQ(manager.CirculatingCount(), 0u);
}

TEST(EntityManager_Tests, Obtain_AfterRelease_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Obtain());

  (void)(manager.Obtain());

  EXPECT_EQ(manager.RecycledCount(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 1u);
}

TEST(EntityManager_Tests, Obtain_AfterRelease_CorrectlyRecycled)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Obtain());

  EXPECT_EQ(manager.CirculatingCount(), 0u);
  EXPECT_EQ(manager.Obtain(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 1u);
}

TEST(EntityManager_Tests, Obtain_ReleaseAll_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  (void)(manager.Obtain());
  (void)(manager.Obtain());

  manager.Release(manager.Obtain());

  EXPECT_EQ(manager.RecycledCount(), 1u);
  EXPECT_EQ(manager.CirculatingCount(), 2u);

  manager.ReleaseAll();

  EXPECT_EQ(manager.RecycledCount(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 0u);
  EXPECT_EQ(manager.Obtain(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 1u);
}

TEST(EntityManager_Tests, Obtain_ReleaseAll_ResetGenerator)
{
  EntityManager<size_t> manager;

  (void)(manager.Obtain());
  (void)(manager.Obtain());

  manager.ReleaseAll();

  EXPECT_EQ(manager.CirculatingCount(), 0u);
  EXPECT_EQ(manager.Obtain(), 0u);
  EXPECT_EQ(manager.CirculatingCount(), 1u);
}

} // namespace genebits::engine::tests