#include "genebits/engine/system/context.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  template<size_t id>
  struct TestResource
  {
    TestResource() : value(0) {}
    TestResource(size_t arg) : value(arg) {}

    size_t value;

    friend bool operator==(const TestResource& lhs, const TestResource& rhs)
    {
      return lhs.value == rhs.value;
    }

    friend bool operator!=(const TestResource& lhs, const TestResource& rhs)
    {
      return !(lhs == rhs);
    }
  };
} // namespace

TEST(Context_Tests, DefaultConstructor_Nothing_Empty)
{
  Context manager;

  EXPECT_EQ(manager.Size(), 0);
}

TEST(Context_Tests, Contains_Nothing_False)
{
  Context manager;

  EXPECT_FALSE(manager.Contains<TestResource<0>>());
}

TEST(Context_Tests, Insert_Single_SizeIncrease)
{
  Context manager;

  manager.Emplace<TestResource<0>>();

  EXPECT_EQ(manager.Size(), 1);
}

TEST(Context_Tests, Contains_Exists_True)
{
  Context manager;

  manager.Emplace<TestResource<0>>();

  EXPECT_TRUE(manager.Contains<TestResource<0>>());
}

TEST(Context_Tests, Emplace_DefaultConstructed_DefaultValue)
{
  Context manager;

  manager.Emplace<TestResource<0>>();

  EXPECT_EQ(manager.Get<TestResource<0>>(), 0);
}

TEST(Context_Tests, Emplace_ValueConstructed_Value)
{
  Context manager;

  manager.Emplace<TestResource<0>>(10);

  EXPECT_EQ(manager.Get<TestResource<0>>(), 10);
}

TEST(Context_Tests, Insert_ExistingResource_Value)
{
  Context manager;

  auto resource = new TestResource<0>(10);
  manager.Insert<TestResource<0>>(resource);

  EXPECT_EQ(manager.Get<TestResource<0>>(), 10);
}

TEST(Context_Tests, Insert_ExistingResourceWithDestructor_Value)
{
  Context manager;

  auto resource = new TestResource<0>(10);
  manager.Insert<TestResource<0>>(resource, [](void* resource) { delete static_cast<TestResource<0>*>(resource); });

  EXPECT_EQ(manager.Get<TestResource<0>>(), 10);
}

TEST(Context_Tests, Remove_ExistingResource_DoesNotContain)
{
  Context manager;

  manager.Emplace<TestResource<0>>();
  manager.Remove<TestResource<0>>();

  EXPECT_FALSE(manager.Contains<TestResource<0>>());
}

TEST(Context_Tests, Emplace_Multiple_CorrectValues)
{
  Context manager;

  manager.Emplace<TestResource<1>>(1);
  EXPECT_EQ(manager.Get<TestResource<1>>(), 1);

  manager.Emplace<TestResource<2>>(2);
  EXPECT_EQ(manager.Get<TestResource<2>>(), 2);

  manager.Emplace<TestResource<3>>(3);
  EXPECT_EQ(manager.Get<TestResource<3>>(), 3);

  manager.Emplace<TestResource<4>>(4);
  EXPECT_EQ(manager.Get<TestResource<4>>(), 4);
}

} // namespace genebits::engine::tests