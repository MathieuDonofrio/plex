#include <genebits/engine/util/enumerator.h>

#include <concepts>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{

namespace
{
  enum class TestEnumWithOperators
  {
    value1 = 0 << 0
  };

  DEFINE_ENUM_FLAG_OPERATORS(TestEnumWithOperators)

  enum class TestEnumNoOperators
  {
    value1 = 0 << 0,
  };

  struct TestStruct
  {
    int value1 = 0;
  };
} // namespace

static_assert(!EnumFlag<TestStruct>, "Struct should not be an enum flag");
static_assert(!EnumFlag<TestEnumNoOperators>, "Enum with no operators should not an enum flag");
static_assert(EnumFlag<TestEnumWithOperators>, "Enum with operators should be an enum flag");

// TODO runtime test all operators for TestEnumWithOperators

TEST(EnumFlag_Tests, OperatorBitwiseOr_CorrectArithmetic) {}

} // namespace genebits::engine::tests