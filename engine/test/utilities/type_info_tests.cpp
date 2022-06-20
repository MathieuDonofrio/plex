#include "genebits/engine/utilities/type_info.h"

#include <gtest/gtest.h>

template<size_t Size>
struct STS
{};

struct TestTypeA
{};

namespace test_namespace
{
class TestTypeB
{};
} // namespace test_namespace

namespace genebits::engine::tests
{
enum TestTypeC
{
};

template<size_t tag>
class TestType
{};

// Hash static tests

static_assert(TypeHash<int>() == TypeHash<int>());
static_assert(TypeHash<TestTypeA>() == TypeHash<TestTypeA>());
static_assert(TypeHash<int>() != TypeHash<double>());
static_assert(TypeHash<float>() != TypeHash<double>());
static_assert(TypeHash<int>() != TypeHash<TestTypeA>());
static_assert(TypeHash<TestTypeC>() != TypeHash<TestTypeA>());

TEST(Meta_Tests, UniqueId_SameType_Equal)
{
  EXPECT_EQ(TypeHash<int>(), TypeHash<int>());
}

TEST(Meta_Tests, UniqueId_SameTypeStruct_Equal)
{
  EXPECT_EQ(TypeHash<TestTypeC>(), TypeHash<TestTypeC>());
}

TEST(Meta_Tests, UniqueId_DifferentType_NotEqual)
{
  EXPECT_NE(TypeHash<float>(), TypeHash<double>());
}

TEST(Meta_Tests, UniqueId_DifferentTypeStruct_NotEqual)
{
  EXPECT_NE(TypeHash<TestTypeA>(), TypeHash<test_namespace::TestTypeB>());
}

TEST(Meta_Tests, UniqueId_SameTypeDifferentTags_NotEqual)
{
  EXPECT_EQ((TypeIndex<TestType<0>, STS<1>>()), (TypeIndex<TestType<0>, STS<2>>()));
}

TEST(Meta_Tests, UniqueId_MultipleTypesSameTag_Increment)
{
  EXPECT_EQ((TypeIndex<TestType<11>, STS<10>>()), 0);
  EXPECT_EQ((TypeIndex<TestType<12>, STS<10>>()), 1);
  EXPECT_EQ((TypeIndex<TestType<13>, STS<10>>()), 2);
  EXPECT_EQ((TypeIndex<TestType<14>, STS<10>>()), 3);
  EXPECT_EQ((TypeIndex<TestType<15>, STS<10>>()), 4);
  EXPECT_EQ((TypeIndex<TestType<16>, STS<10>>()), 5);
  EXPECT_EQ((TypeIndex<TestType<17>, STS<10>>()), 6);
  EXPECT_EQ((TypeIndex<TestType<18>, STS<10>>()), 7);
  EXPECT_EQ((TypeIndex<TestType<19>, STS<10>>()), 8);
}

TEST(Meta_Tests, UniqueId_MultipleTypesTwoTags_Increment)
{
  EXPECT_EQ((TypeIndex<TestType<21>, STS<20>>()), 0);
  EXPECT_EQ((TypeIndex<TestType<22>, STS<20>>()), 1);

  EXPECT_EQ((TypeIndex<TestType<31>, STS<30>>()), 0);
  EXPECT_EQ((TypeIndex<TestType<32>, STS<30>>()), 1);
}

} // namespace genebits::engine::tests