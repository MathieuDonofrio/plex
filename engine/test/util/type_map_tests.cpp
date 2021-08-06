#include "engine/util/type_map.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestValue
  {
    size_t value = 0;
  };

  template<size_t Tag = 0>
  struct TestKey
  {
  };
} // namespace

TEST(TypeMap_Tests, Get_FirstTime_DefaultState)
{
  TypeMap<TestValue> map;

  ASSERT_EQ(map.Get<TestKey<0>>().value, 0);
}

TEST(TypeMap_Tests, Get_AfterStore_StoredValue)
{
  TypeMap<TestValue> map;

  map.Get<TestKey<0>>().value = 10;

  ASSERT_EQ(map.Get<TestKey<0>>().value, 10);
}

TEST(TypeMap_Tests, Get_AfterStoreOfDifferentType_DefaultState)
{
  TypeMap<TestValue> map;

  map.Get<TestKey<0>>().value = 10;

  ASSERT_EQ(map.Get<TestKey<1>>().value, 0);
}

TEST(TypeMap_Tests, Get_Load100Collisions_StoredValues)
{
  TypeMap<TestValue, 8> map;

  map.Get<TestKey<0>>().value = 0;
  map.Get<TestKey<1>>().value = 1;
  map.Get<TestKey<2>>().value = 2;
  map.Get<TestKey<3>>().value = 3;
  map.Get<TestKey<4>>().value = 4;
  map.Get<TestKey<5>>().value = 5;
  map.Get<TestKey<6>>().value = 6;
  map.Get<TestKey<7>>().value = 7;

  ASSERT_EQ(map.Get<TestKey<0>>().value, 0);
  ASSERT_EQ(map.Get<TestKey<1>>().value, 1);
  ASSERT_EQ(map.Get<TestKey<2>>().value, 2);
  ASSERT_EQ(map.Get<TestKey<3>>().value, 3);
  ASSERT_EQ(map.Get<TestKey<4>>().value, 4);
  ASSERT_EQ(map.Get<TestKey<5>>().value, 5);
  ASSERT_EQ(map.Get<TestKey<6>>().value, 6);
  ASSERT_EQ(map.Get<TestKey<7>>().value, 7);
}

} // namespace genebits::engine::tests