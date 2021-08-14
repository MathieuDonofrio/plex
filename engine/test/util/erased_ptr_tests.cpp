#include "engine/util/erased_ptr.h"

#include <thread>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestBase
  {
    virtual ~TestBase() = default;
  };

  struct Test1Base
  {
    virtual size_t Test1() const = 0;
  };

  struct Test2Base
  {
    virtual size_t Test2() const = 0;
  };

  struct TestType : Test1Base, Test2Base, TestBase
  {
    size_t Test1() const override
    {
      return 10;
    }

    size_t Test2() const override
    {
      return 20;
    }
  };
} // namespace

TEST(ErasedPtr_Tests, DefaultConstructor_Default_DefaultState)
{
  ErasedPtr ptr;

  ASSERT_FALSE(ptr);
}

TEST(ErasedPtr_Tests, ParametricConstructor_Trivial_NotNull)
{
  ErasedPtr ptr(new size_t(10));

  ASSERT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, Reset_Trivial_NotNull)
{
  ErasedPtr ptr;

  ptr.Reset(new size_t(10));

  ASSERT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, Cast_Trivial_CorrectValue)
{
  ErasedPtr ptr(new size_t(10));

  ASSERT_EQ(*ptr.Cast<size_t>(), 10);
}

TEST(ErasedPtr_Tests, ConstCast_Trivial_CorrectValue)
{
  const ErasedPtr ptr(new size_t(10));

  ASSERT_EQ(*ptr.Cast<size_t>(), 10);
}

TEST(ErasedPtr_Tests, MoveConstructor_Trivial_CorrectValue)
{
  ErasedPtr original(new size_t(10));

  ErasedPtr moved(std::move(original));

  ASSERT_TRUE(moved);
  ASSERT_FALSE(original);
  ASSERT_EQ(*moved.Cast<size_t>(), 10);
}

TEST(ErasedPtr_Tests, MoveAssignment_Trivial_CorrectValue)
{
  ErasedPtr original(new size_t(10));

  ErasedPtr moved = std::move(original);

  ASSERT_TRUE(moved);
  ASSERT_FALSE(original);
  ASSERT_EQ(*moved.Cast<size_t>(), 10);
}

TEST(ErasedPtr_Tests, Cast_VirtualTypeExactCast_CorrectCast)
{
  ErasedPtr ptr(new TestType());

  ASSERT_EQ(ptr.Cast<TestType>()->Test2(), 20);
}

TEST(ErasedPtr_Tests, DynamicCast_VirtualTypeBaseCast_CorrectCast)
{
  ErasedPtr<TestBase> ptr(new TestType());

  ASSERT_EQ(ptr.DynamicCast<Test2Base>()->Test2(), 20);
}

TEST(ErasedPtr_Tests, ConstDynamicCast_VirtualTypeBaseCast_CorrectCast)
{
  const ErasedPtr<TestBase> ptr(new TestType());

  ASSERT_EQ(ptr.DynamicCast<Test2Base>()->Test2(), 20);
}

} // namespace genebits::engine::tests