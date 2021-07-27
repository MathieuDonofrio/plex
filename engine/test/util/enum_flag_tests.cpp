#include <engine/util/enum_flag.h>

#include <concepts>

#include <gtest/gtest.h>

namespace genebits::engine
{

enum class MockEnumSpecialized : uint64_t
{
  value1 = 0 << 0
};

template<>
struct EnableBitwiseOperators<MockEnumSpecialized>
{};

enum class MockEnumNotSpecialized : uint64_t
{
  value1 = 0 << 0,
};

struct NotAnEnum
{
  int value1 = 0;
};

template<typename T>
concept EnumBitwiseAnd = requires(T enum_flag)
{
  {
    enum_flag & enum_flag
  };
};

template<typename T>
concept EnumBitwiseOr = requires(T enum_flag)
{
  {
    enum_flag | enum_flag
  };
};

template<typename E, typename T>
concept EnumLogicNotEqual = requires(E enum_flag, T compared_type)
{
  {
    enum_flag != compared_type
  }
  ->std::same_as<bool>;
};

template<typename E, typename T>
concept EnumLogicEqual = requires(E enum_flag, T compared_type)
{
  {
    enum_flag == compared_type
  }
  ->std::same_as<bool>;
};

TEST(Enum_Flag_Tests, OperatorOverload_Specialized)
{
  ASSERT_TRUE(EnumBitwiseAnd<MockEnumSpecialized>);
  ASSERT_TRUE(EnumBitwiseOr<MockEnumSpecialized>);
  ASSERT_TRUE((EnumLogicNotEqual<MockEnumSpecialized, uint64_t>));
  ASSERT_TRUE((EnumLogicEqual<MockEnumSpecialized, uint64_t>));
}

TEST(Enum_Flag_Tests, OperatorOverload_NotSpecialized)
{
  ASSERT_FALSE(EnumBitwiseAnd<MockEnumNotSpecialized>);
  ASSERT_FALSE(EnumBitwiseOr<MockEnumNotSpecialized>);
  ASSERT_FALSE((EnumLogicNotEqual<MockEnumNotSpecialized, uint64_t>));
  ASSERT_FALSE((EnumLogicEqual<MockEnumNotSpecialized, uint64_t>));
}

TEST(Enum_Flag_Tests, OperatorOverload_WrongType)
{
  ASSERT_FALSE(EnumBitwiseAnd<NotAnEnum>);
  ASSERT_FALSE(EnumBitwiseOr<NotAnEnum>);
  ASSERT_FALSE((EnumLogicNotEqual<NotAnEnum, uint64_t>));
  ASSERT_FALSE((EnumLogicEqual<NotAnEnum, uint64_t>));
}

} // namespace genebits::engine