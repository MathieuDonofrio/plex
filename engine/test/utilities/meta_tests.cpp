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

static_assert(TypeInfo<int>::HashCode() == TypeInfo<int>::HashCode());
static_assert(TypeInfo<TestTypeA>::HashCode() == TypeInfo<TestTypeA>::HashCode());
static_assert(TypeInfo<int>::HashCode() != TypeInfo<double>::HashCode());
static_assert(TypeInfo<float>::HashCode() != TypeInfo<double>::HashCode());
static_assert(TypeInfo<int>::HashCode() != TypeInfo<TestTypeA>::HashCode());
static_assert(TypeInfo<TestTypeC>::HashCode() != TypeInfo<TestTypeA>::HashCode());

TEST(Meta_Tests, UniqueId_SameType_Equal)
{
  EXPECT_EQ(TypeInfo<int>::Index(), TypeInfo<int>::Index());
}

TEST(Meta_Tests, UniqueId_SameTypeStruct_Equal)
{
  EXPECT_EQ(TypeInfo<TestTypeC>::Index(), TypeInfo<TestTypeC>::Index());
}

TEST(Meta_Tests, UniqueId_DifferentType_NotEqual)
{
  EXPECT_NE(TypeInfo<float>::Index(), TypeInfo<double>::Index());
}

TEST(Meta_Tests, UniqueId_DifferentTypeStruct_NotEqual)
{
  EXPECT_NE(TypeInfo<TestTypeA>::Index(), TypeInfo<test_namespace::TestTypeB>::Index());
}

TEST(Meta_Tests, UniqueId_SameTypeDifferentTags_NotEqual)
{
  EXPECT_EQ(TypeInfo<TestType<0>>::Index<STS<1>>(), TypeInfo<TestType<0>>::Index<STS<2>>());
}

TEST(Meta_Tests, UniqueId_MultipleTypesSameTag_Increment)
{
  EXPECT_EQ(TypeInfo<TestType<11>>::Index<STS<10>>(), 0);
  EXPECT_EQ(TypeInfo<TestType<12>>::Index<STS<10>>(), 1);
  EXPECT_EQ(TypeInfo<TestType<13>>::Index<STS<10>>(), 2);
  EXPECT_EQ(TypeInfo<TestType<14>>::Index<STS<10>>(), 3);
  EXPECT_EQ(TypeInfo<TestType<15>>::Index<STS<10>>(), 4);
  EXPECT_EQ(TypeInfo<TestType<16>>::Index<STS<10>>(), 5);
  EXPECT_EQ(TypeInfo<TestType<17>>::Index<STS<10>>(), 6);
  EXPECT_EQ(TypeInfo<TestType<18>>::Index<STS<10>>(), 7);
  EXPECT_EQ(TypeInfo<TestType<19>>::Index<STS<10>>(), 8);
}

TEST(Meta_Tests, UniqueId_MultipleTypesTwoTags_Increment)
{
  EXPECT_EQ(TypeInfo<TestType<21>>::Index<STS<20>>(), 0);
  EXPECT_EQ(TypeInfo<TestType<22>>::Index<STS<20>>(), 1);

  EXPECT_EQ(TypeInfo<TestType<31>>::Index<STS<30>>(), 0);
  EXPECT_EQ(TypeInfo<TestType<32>>::Index<STS<30>>(), 1);
}

} // namespace genebits::engine::tests