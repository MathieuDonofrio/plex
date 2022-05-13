#include "genebits/engine/containers/type_map.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestValue
  {
    TestValue() : value(0) {}

    size_t value;

    friend bool operator==(const TestValue& lhs, const TestValue& rhs)
    {
      return lhs.value == rhs.value;
    }

    friend bool operator!=(const TestValue& lhs, const TestValue& rhs)
    {
      return !(lhs == rhs);
    }
  };

  template<size_t Tag = 0>
  struct TestKey
  {};

  struct NonDefaultConstructible
  {
    NonDefaultConstructible(size_t value) : value_(value) {}

    size_t value_;
  };
} // namespace

TEST(TypeMap_Tests, Assure_FirstTime_DefaultState)
{
  TypeMap<TestValue> map;

  EXPECT_EQ(map.Assure<TestKey<0>>().value, 0);
}

TEST(TypeMap_Tests, Assure_AfterStore_StoredValue)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;

  EXPECT_EQ(map.Assure<TestKey<0>>().value, 10);
}

TEST(TypeMap_Tests, Assure_AfterStoreOfDifferentType_DefaultState)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;

  EXPECT_EQ(map.Assure<TestKey<1>>().value, 0);
}

TEST(TypeMap_Tests, Assure_MultipleSets_StoredValues)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 0;
  map.Assure<TestKey<1>>().value = 1;
  map.Assure<TestKey<2>>().value = 2;
  map.Assure<TestKey<3>>().value = 3;
  map.Assure<TestKey<4>>().value = 4;
  map.Assure<TestKey<5>>().value = 5;
  map.Assure<TestKey<6>>().value = 6;
  map.Assure<TestKey<7>>().value = 7;

  EXPECT_EQ(map.Assure<TestKey<0>>().value, 0);
  EXPECT_EQ(map.Assure<TestKey<1>>().value, 1);
  EXPECT_EQ(map.Assure<TestKey<2>>().value, 2);
  EXPECT_EQ(map.Assure<TestKey<3>>().value, 3);
  EXPECT_EQ(map.Assure<TestKey<4>>().value, 4);
  EXPECT_EQ(map.Assure<TestKey<5>>().value, 5);
  EXPECT_EQ(map.Assure<TestKey<6>>().value, 6);
  EXPECT_EQ(map.Assure<TestKey<7>>().value, 7);
}

TEST(TypeMap_Tests, Get_AfterAssureStore_SameAsStored)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;

  EXPECT_EQ(map.Get<TestKey<0>>().value, 10);
}

TEST(TypeMap_Tests, Get_AfterAssureStoreOfDifferentType_Zero)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;
  map.Assure<TestKey<1>>();

  EXPECT_EQ(map.Get<TestKey<1>>().value, 0);
}

TEST(TypeMap_Tests, ContainsNonDefault_Empty_False)
{
  TypeMap<TestValue> map;

  EXPECT_FALSE(map.ContainsNonDefault<TestKey<0>>());
}

TEST(TypeMap_Tests, ContainsNonDefault_ContainsWithDefault_False)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>();

  EXPECT_FALSE(map.ContainsNonDefault<TestKey<0>>());
}

TEST(TypeMap_Tests, ContainsNonDefault_ContainsWithValue_True)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;

  EXPECT_TRUE(map.ContainsNonDefault<TestKey<0>>());
}

TEST(TypeMap_Tests, SetDefault_WithValue_DoesNotContain)
{
  TypeMap<TestValue> map;

  map.Assure<TestKey<0>>().value = 10;
  map.SetDefault<TestKey<0>>();

  EXPECT_FALSE(map.ContainsNonDefault<TestKey<0>>());
}
} // namespace genebits::engine::tests