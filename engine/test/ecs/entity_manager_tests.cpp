#include "genebits/engine/ecs/entity_manager.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(EntityManager_Tests, Generate_AfterInitialization_UniqueId)
{
  EntityManager<size_t> manager;

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.RecycledCount(), 0);

  ASSERT_EQ(manager.Generate(), 0);
  ASSERT_EQ(manager.Generate(), 1);
  ASSERT_EQ(manager.Generate(), 2);

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 3);
}

TEST(EntityManager_Tests, Release_AfterInitialization_IncreaseRecycledCount)
{
  EntityManager<size_t> manager;

  ASSERT_EQ(manager.RecycledCount(), 0);

  manager.Release(manager.Generate());

  ASSERT_EQ(manager.RecycledCount(), 1);
  ASSERT_EQ(manager.CirculatingCount(), 0);
}

TEST(EntityManager_Tests, Generate_AfterRelease_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Generate());

  (void)(manager.Generate());

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Generate_AfterRelease_CorrectlyRecycled)
{
  EntityManager<size_t> manager;

  manager.Release(manager.Generate());

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Generate(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Generate_ReleaseAll_DecreaseRecycleCount)
{
  EntityManager<size_t> manager;

  (void)(manager.Generate());
  (void)(manager.Generate());

  manager.Release(manager.Generate());

  ASSERT_EQ(manager.RecycledCount(), 1);
  ASSERT_EQ(manager.CirculatingCount(), 2);

  manager.ReleaseAll();

  ASSERT_EQ(manager.RecycledCount(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Generate(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

TEST(EntityManager_Tests, Generate_ReleaseAll_ResetGenerator)
{
  EntityManager<size_t> manager;

  (void)(manager.Generate());
  (void)(manager.Generate());

  manager.ReleaseAll();

  ASSERT_EQ(manager.CirculatingCount(), 0);
  ASSERT_EQ(manager.Generate(), 0);
  ASSERT_EQ(manager.CirculatingCount(), 1);
}

} // namespace genebits::engine::tests