#include "genebits/engine/util/meta.h"

#include <gtest/gtest.h>

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

// FullName static tests

static_assert(Meta<int>::FullName() == "int");
static_assert(Meta<double>::FullName() == "double");
static_assert(Meta<float>::FullName() == "float");
static_assert(Meta<TestTypeA>::FullName() == "TestTypeA");
static_assert(Meta<test_namespace::TestTypeB>::FullName() == "test_namespace::TestTypeB");
static_assert(Meta<TestTypeC>::FullName() == "genebits::engine::tests::TestTypeC");

// Name static tests

static_assert(Meta<int>::Name() == "int");
static_assert(Meta<double>::Name() == "double");
static_assert(Meta<float>::Name() == "float");
static_assert(Meta<TestTypeA>::Name() == "TestTypeA");
static_assert(Meta<test_namespace::TestTypeB>::Name() == "TestTypeB");
static_assert(Meta<TestTypeC>::Name() == "TestTypeC");

// Hash static tests

static_assert(Meta<int>::Hash() == Meta<int>::Hash());
static_assert(Meta<TestTypeA>::Hash() == Meta<TestTypeA>::Hash());
static_assert(Meta<int>::Hash() != Meta<double>::Hash());
static_assert(Meta<float>::Hash() != Meta<double>::Hash());
static_assert(Meta<int>::Hash() != Meta<TestTypeA>::Hash());
static_assert(Meta<TestTypeC>::Hash() != Meta<TestTypeA>::Hash());

TEST(Meta_Tests, UniqueId_SameType_Equal)
{
  ASSERT_EQ(Meta<int>::UniqueId(), Meta<int>::UniqueId());
}

TEST(Meta_Tests, UniqueId_SameTypeStruct_Equal)
{
  ASSERT_EQ(Meta<TestTypeC>::UniqueId(), Meta<TestTypeC>::UniqueId());
}

TEST(Meta_Tests, UniqueId_DifferentType_NotEqual)
{
  ASSERT_NE(Meta<float>::UniqueId(), Meta<double>::UniqueId());
}

TEST(Meta_Tests, UniqueId_DifferentTypeStruct_NotEqual)
{
  ASSERT_NE(Meta<TestTypeA>::UniqueId(), Meta<test_namespace::TestTypeB>::UniqueId());
}

TEST(Meta_Tests, UniqueId_SameTypeDifferentTags_NotEqual)
{
  ASSERT_EQ(Meta<TestType<0>>::UniqueId<1>(), Meta<TestType<0>>::UniqueId<2>());
}

TEST(Meta_Tests, UniqueId_MultipleTypesSameTag_Increment)
{
  ASSERT_EQ(Meta<TestType<11>>::UniqueId<10>(), 0);
  ASSERT_EQ(Meta<TestType<12>>::UniqueId<10>(), 1);
  ASSERT_EQ(Meta<TestType<13>>::UniqueId<10>(), 2);
  ASSERT_EQ(Meta<TestType<14>>::UniqueId<10>(), 3);
  ASSERT_EQ(Meta<TestType<15>>::UniqueId<10>(), 4);
  ASSERT_EQ(Meta<TestType<16>>::UniqueId<10>(), 5);
  ASSERT_EQ(Meta<TestType<17>>::UniqueId<10>(), 6);
  ASSERT_EQ(Meta<TestType<18>>::UniqueId<10>(), 7);
  ASSERT_EQ(Meta<TestType<19>>::UniqueId<10>(), 8);
}

TEST(Meta_Tests, UniqueId_MultipleTypesTwoTags_Increment)
{
  ASSERT_EQ(Meta<TestType<21>>::UniqueId<20>(), 0);
  ASSERT_EQ(Meta<TestType<22>>::UniqueId<20>(), 1);

  ASSERT_EQ(Meta<TestType<31>>::UniqueId<30>(), 0);
  ASSERT_EQ(Meta<TestType<32>>::UniqueId<30>(), 1);
}

} // namespace genebits::engine::tests