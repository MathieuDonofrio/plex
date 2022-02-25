#include "genebits/engine/utilities/meta.h"

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

static_assert(Meta<int>::Hash() == Meta<int>::Hash());
static_assert(Meta<TestTypeA>::Hash() == Meta<TestTypeA>::Hash());
static_assert(Meta<int>::Hash() != Meta<double>::Hash());
static_assert(Meta<float>::Hash() != Meta<double>::Hash());
static_assert(Meta<int>::Hash() != Meta<TestTypeA>::Hash());
static_assert(Meta<TestTypeC>::Hash() != Meta<TestTypeA>::Hash());

TEST(Meta_Tests, UniqueId_SameType_Equal)
{
  EXPECT_EQ(Meta<int>::UniqueId(), Meta<int>::UniqueId());
}

TEST(Meta_Tests, UniqueId_SameTypeStruct_Equal)
{
  EXPECT_EQ(Meta<TestTypeC>::UniqueId(), Meta<TestTypeC>::UniqueId());
}

TEST(Meta_Tests, UniqueId_DifferentType_NotEqual)
{
  EXPECT_NE(Meta<float>::UniqueId(), Meta<double>::UniqueId());
}

TEST(Meta_Tests, UniqueId_DifferentTypeStruct_NotEqual)
{
  EXPECT_NE(Meta<TestTypeA>::UniqueId(), Meta<test_namespace::TestTypeB>::UniqueId());
}

TEST(Meta_Tests, UniqueId_SameTypeDifferentTags_NotEqual)
{
  EXPECT_EQ(Meta<TestType<0>>::UniqueId<STS<1>>(), Meta<TestType<0>>::UniqueId<STS<2>>());
}

TEST(Meta_Tests, UniqueId_MultipleTypesSameTag_Increment)
{
  EXPECT_EQ(Meta<TestType<11>>::UniqueId<STS<10>>(), 0);
  EXPECT_EQ(Meta<TestType<12>>::UniqueId<STS<10>>(), 1);
  EXPECT_EQ(Meta<TestType<13>>::UniqueId<STS<10>>(), 2);
  EXPECT_EQ(Meta<TestType<14>>::UniqueId<STS<10>>(), 3);
  EXPECT_EQ(Meta<TestType<15>>::UniqueId<STS<10>>(), 4);
  EXPECT_EQ(Meta<TestType<16>>::UniqueId<STS<10>>(), 5);
  EXPECT_EQ(Meta<TestType<17>>::UniqueId<STS<10>>(), 6);
  EXPECT_EQ(Meta<TestType<18>>::UniqueId<STS<10>>(), 7);
  EXPECT_EQ(Meta<TestType<19>>::UniqueId<STS<10>>(), 8);
}

TEST(Meta_Tests, UniqueId_MultipleTypesTwoTags_Increment)
{
  EXPECT_EQ(Meta<TestType<21>>::UniqueId<STS<20>>(), 0);
  EXPECT_EQ(Meta<TestType<22>>::UniqueId<STS<20>>(), 1);

  EXPECT_EQ(Meta<TestType<31>>::UniqueId<STS<30>>(), 0);
  EXPECT_EQ(Meta<TestType<32>>::UniqueId<STS<30>>(), 1);
}

} // namespace genebits::engine::tests