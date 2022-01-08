#include "genebits/engine/util/object_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ObjectPool_Tests, Aquire_NothingInPool_NewObject)
{
  ObjectPool<size_t> pool;

  size_t* value = pool.Acquire(99);

  EXPECT_EQ(*value, 99);

  pool.Release(value);
}

TEST(ObjectPool_Tests, Release_AddsObjectToPool_RetreiveSameObject)
{
  ObjectPool<size_t> pool;

  size_t* value1 = pool.Acquire(99);

  pool.Release(value1);

  size_t* value2 = pool.AcquireUninitialized();

  EXPECT_EQ(*value1, *value2);
}
} // namespace genebits::engine::tests