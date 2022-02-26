#include "genebits/engine/utilities/erased_ptr.h"

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
  ErasedPtr<void> ptr;

  EXPECT_FALSE(ptr);
}

TEST(ErasedPtr_Tests, ParametricConstructor_Trivial_NotNull)
{
  ErasedPtr<void> ptr(new size_t(10));

  EXPECT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, MoveConstructor_Trivial_CorrectValue)
{
  ErasedPtr<void> original(new size_t(10));

  ErasedPtr<void> moved(std::move(original));

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*static_cast<size_t*>(moved.Get()), 10u);
}

TEST(ErasedPtr_Tests, MoveAssignment_Trivial_CorrectValue)
{
  ErasedPtr<void> original(new size_t(10));

  ErasedPtr<void> moved = std::move(original);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*static_cast<size_t*>(moved.Get()), 10u);
}

} // namespace genebits::engine::tests