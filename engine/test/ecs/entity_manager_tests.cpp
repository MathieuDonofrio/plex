#include "genebits/engine/ecs/entity_manager.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(EntityManager_Tests, Obtain_AfterInitialization_UniqueId)
{
  EntityManager<size_t> manager;

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.RecycledCount(), 0);

  ASSERT_EQ(manager.Obtain(), 0);
  ASSERT_EQ(manager.Obtain(), 1);
  ASSERT_EQ(manager.Obtain(), 2);

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 3);
}

TEST(EntityManager_Tests, Release_AfterInitialization_IncreaseRecycledCount)
{
  EntityManager<size_t> manager;

  ASSERT_EQ(manager.RecycledCount(), 0);

  manager.Release(manager.Obtain());

  ASSERT_EQ(manager.RecycledCount(), 1);
  ASSERT_EQ(manager.CirculatingCount(), 0);
}

TEST(EntityManager_Tests, Obtain_AfterRelease_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Obtain());

  (void)(manager.Obtain());

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Obtain_AfterRelease_CorrectlyRecycled)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Obtain());

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Obtain(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Obtain_ReleaseAll_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  (void)(manager.Obtain());
  (void)(manager.Obtain());

  manager.Release(manager.Obtain());

  ASSERT_EQ(manager.RecycledCount(), 1);
  ASSERT_EQ(manager.CirculatingCount(), 2);

  manager.ReleaseAll();

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Obtain(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Obtain_ReleaseAll_ResetGenerator)
{
  EntityManager<size_t> manager;

  (void)(manager.Obtain());
  (void)(manager.Obtain());

  manager.ReleaseAll();

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Obtain(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

} // namespace genebits::engine::tests