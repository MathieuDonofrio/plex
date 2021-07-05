#include "engine/util/meta.h"

#include <gtest/gtest.h>

struct TestTypeA
{
};

namespace test_namespace
{
class TestTypeB
{
};
} // namespace test_namespace

namespace genebits::engine::tests
{
enum TestTypeC
{
};

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
} // namespace genebits::engine::tests