#include "genebits/engine/utilities/erased_ptr.h"

#include <thread>

#include <gtest/gtest.h>

namespace plex::tests
{
namespace
{
  struct TestPoly
  {
    virtual ~TestPoly() = default;

    virtual size_t Test() const = 0;
  };

  template<size_t id>
  struct TestPolyType : public TestPoly
  {
    size_t Test() const override
    {
      return id;
    }
  };
} // namespace

TEST(ErasedPtr_Tests, DefaultConstructor_Trivial_DefaultState)
{
  ErasedPtr<void> ptr;

  EXPECT_FALSE(ptr);
}

TEST(ErasedPtr_Tests, ParametricConstructor_Trivial_NotNull)
{
  ErasedPtr<size_t> ptr(new size_t(10));

  EXPECT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, MoveConstructor_Trivial_CorrectValue)
{
  ErasedPtr<size_t> original(new size_t(10));

  ErasedPtr<size_t> moved(std::move(original));

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*moved.get(), 10u);
}

TEST(ErasedPtr_Tests, MoveAssignment_Trivial_CorrectValue)
{
  ErasedPtr<size_t> original(new size_t(10));

  ErasedPtr<size_t> moved = std::move(original);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*moved.get(), 10u);
}

TEST(ErasedPtr_Tests, DefaultConstructor_Void_DefaultState)
{
  ErasedPtr<void> ptr;

  EXPECT_FALSE(ptr);
}

TEST(ErasedPtr_Tests, ParametricConstructor_Void_NotNull)
{
  ErasedPtr<void> ptr(new size_t(10));

  EXPECT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, MoveConstructor_Void_CorrectValue)
{
  ErasedPtr<void> original(new size_t(10));

  ErasedPtr<void> moved(std::move(original));

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*static_cast<size_t*>(moved.get()), 10u);
}

TEST(ErasedPtr_Tests, MoveAssignment_Void_CorrectValue)
{
  ErasedPtr<void> original(new size_t(10));

  ErasedPtr<void> moved = std::move(original);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(*static_cast<size_t*>(moved.get()), 10u);
}

TEST(ErasedPtr_Tests, DefaultConstructor_Poly_DefaultState)
{
  ErasedPtr<TestPoly> ptr;

  EXPECT_FALSE(ptr);
}

TEST(ErasedPtr_Tests, ParametricConstructor_Poly_NotNull)
{
  ErasedPtr<TestPoly> ptr(new TestPolyType<0>());

  EXPECT_TRUE(ptr);
}

TEST(ErasedPtr_Tests, MoveConstructor_Poly_CorrectValue)
{
  ErasedPtr<TestPoly> original(new TestPolyType<10>());

  ErasedPtr<TestPoly> moved(std::move(original));

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(moved.get()->Test(), 10);
}

TEST(ErasedPtr_Tests, MoveAssignment_Poly_CorrectValue)
{
  ErasedPtr<TestPoly> original(new TestPolyType<10>());

  ErasedPtr<TestPoly> moved(std::move(original));

  EXPECT_TRUE(moved);
  EXPECT_FALSE(original);
  EXPECT_EQ(moved.get()->Test(), 10);
}

} // namespace plex::tests