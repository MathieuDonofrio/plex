#include "genebits/engine/math/vec2.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(std::is_trivially_copy_constructible_v<float2>, "Vec2 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float2>, "Vec2 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float2>, "Vec2 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float2>, "Vec2 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float2>, "Vec2 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float2>, "Vec2 Must be trivially copyable");

template<typename T>
class Vec2_Tests : public testing::Test
{};

using Vec2_Test_Types = testing::Types<int2, float2, double2>;

TYPED_TEST_SUITE(Vec2_Tests, Vec2_Test_Types);

TYPED_TEST(Vec2_Tests, Constructor_Default_ZeroInitialize)
{
  TypeParam vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 0);
}

TYPED_TEST(Vec2_Tests, Constructor_Scalar_Initialize)
{
  TypeParam vec(1);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 1);
}

TYPED_TEST(Vec2_Tests, Constructor_Value_Initialize)
{
  TypeParam vec(1, 2);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
}

TYPED_TEST(Vec2_Tests, Constructor_Copy_Initialize)
{
  TypeParam vec(1, 2);
  TypeParam vec2(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, Constructor_CopyFromOtherType_Initialize)
{
  TypeParam vec(1, 2);

  if constexpr (std::is_same_v<TypeParam, float2>)
  {
    int2 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
  }
  else
  {
    float2 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
  }
}

TYPED_TEST(Vec2_Tests, Constructor_Move)
{
  TypeParam vec(1, 2);
  TypeParam vec2(std::move(vec));
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, Assignment_Copy_Assign)
{
  TypeParam vec(1, 2);
  TypeParam vec2;
  vec2 = vec;
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, Assignment_Move_Assign)
{
  TypeParam vec(1, 2);
  TypeParam vec2;
  vec2 = std::move(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, AddAssign_Scalar)
{
  TypeParam vec(1, 2);
  vec += 1;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
}

TYPED_TEST(Vec2_Tests, AddAssign_Vec)
{
  TypeParam vec(1, 2);
  vec += TypeParam(2, 3);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 5);
}

TYPED_TEST(Vec2_Tests, SubAssign_Scalar)
{
  TypeParam vec(1, 2);
  vec -= 1;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
}

TYPED_TEST(Vec2_Tests, SubAssign_Vec)
{
  TypeParam vec(1, 2);
  vec -= TypeParam(0, 2);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 0);
}

TYPED_TEST(Vec2_Tests, MultAssign_Scalar)
{
  TypeParam vec(1, 2);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
}

TYPED_TEST(Vec2_Tests, MultAssign_Vec)
{
  TypeParam vec(1, 2);
  vec *= TypeParam(3, 2);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
}

TYPED_TEST(Vec2_Tests, DivAssign_Scalar)
{
  TypeParam vec(2, 4);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
}

TYPED_TEST(Vec2_Tests, DivAssign_Vec)
{
  TypeParam vec(2, 4);
  vec /= TypeParam(1, 2);
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 2);
}

TYPED_TEST(Vec2_Tests, Increment_Pre)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = ++vec;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
}

TYPED_TEST(Vec2_Tests, Increment_Post)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = vec++;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, Decrement_Pre)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = --vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
}

TYPED_TEST(Vec2_Tests, Decrement_Post)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = vec--;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
}

TYPED_TEST(Vec2_Tests, ArrayAccess_Get)
{
  TypeParam vec(1, 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}

TYPED_TEST(Vec2_Tests, ArrayAccess_ConstGet)
{
  const TypeParam vec(1, 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}

TYPED_TEST(Vec2_Tests, ArrayAccess_Set)
{
  TypeParam vec(1, 2);
  vec[0] = 3;
  vec[1] = 4;
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
}

TYPED_TEST(Vec2_Tests, BoolConversion)
{
  TypeParam vec(1, 2);
  EXPECT_TRUE(vec);
  vec = 0;
  EXPECT_FALSE(vec);
}

TYPED_TEST(Vec2_Tests, Add_Scalar)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = vec + 1;
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
}

TYPED_TEST(Vec2_Tests, Add_Vec)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = vec + TypeParam(3, 1);
  EXPECT_EQ(vec2.x, 4);
  EXPECT_EQ(vec2.y, 3);
}

TYPED_TEST(Vec2_Tests, Sub_Scalar)
{
  TypeParam vec(1, 2);
  TypeParam vec2 = vec - 1;
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
}

TYPED_TEST(Vec2_Tests, Sub_Vec)
{
  TypeParam vec(1, 3);
  TypeParam vec2 = vec - TypeParam(1, 2);
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
}

TYPED_TEST(Vec2_Tests, Mult_Scalar)
{
  TypeParam vec(1, 2);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
}

TYPED_TEST(Vec2_Tests, Mult_Vec)
{
  TypeParam vec(1, 2);
  vec *= TypeParam(2, 3);
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 6);
}

TYPED_TEST(Vec2_Tests, Div_Scalar)
{
  TypeParam vec(2, 4);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
}

TYPED_TEST(Vec2_Tests, Div_Vec)
{
  TypeParam vec(2, 4);
  vec /= TypeParam(2, 1);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 4);
}

TYPED_TEST(Vec2_Tests, Equals_True)
{
  TypeParam vec(1, 2);
  TypeParam vec2(1, 2);
  EXPECT_TRUE(vec == vec2);
}

TYPED_TEST(Vec2_Tests, Equals_False)
{
  TypeParam vec(1, 2);
  TypeParam vec2(2, 2);
  EXPECT_FALSE(vec == vec2);
}

TYPED_TEST(Vec2_Tests, NotEquals_True)
{
  TypeParam vec(1, 2);
  TypeParam vec2(2, 2);
  EXPECT_TRUE(vec != vec2);
}

TYPED_TEST(Vec2_Tests, NotEquals_False)
{
  TypeParam vec(1, 2);
  TypeParam vec2(1, 2);
  EXPECT_FALSE(vec != vec2);
}

// Bitwise tests

TEST(Vec2Bitwise_Tests, BitwiseAndAssign_Scalar)
{
  int2 a { 1, 2 };

  a &= 3;

  EXPECT_EQ(a, (int2 { 1 & 3, 2 & 3 }));
}

TEST(Vec2Bitwise_Tests, BitwiseAndAssign_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  a &= b;

  EXPECT_EQ(a, (int2 { 1 & 3, 2 & 4 }));
}

TEST(Vec2Bitwise_Tests, BitwiseOrAssign_Scalar)
{
  int2 a { 1, 2 };

  a |= 3;

  EXPECT_EQ(a, (int2 { 1 | 3, 2 | 3 }));
}

TEST(Vec2Bitwise_Tests, BitwiseOrAssign_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  a |= b;

  EXPECT_EQ(a, (int2 { 1 | 3, 2 | 4 }));
}

TEST(Vec2Bitwise_Tests, BitwiseXorAssign_Scalar)
{
  int2 a { 1, 2 };

  a ^= 3;

  EXPECT_EQ(a, (int2 { 1 ^ 3, 2 ^ 3 }));
}

TEST(Vec2Bitwise_Tests, BitwiseXorAssign_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  a ^= b;

  EXPECT_EQ(a, (int2 { 1 ^ 3, 2 ^ 4 }));
}

TEST(Vec2Bitwise_Tests, BitwiseLeftShiftAssign_Scalar)
{
  int2 a { 1, 2 };

  a <<= 3;

  EXPECT_EQ(a, (int2 { 1 << 3, 2 << 3 }));
}

TEST(Vec2Bitwise_Tests, BitwiseLeftShiftAssign_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  a <<= b;

  EXPECT_EQ(a, (int2 { 1 << 3, 2 << 4 }));
}

TEST(Vec2Bitwise_Tests, BitwiseRightShiftAssign_Scalar)
{
  int2 a { 1, 2 };

  a >>= 3;

  EXPECT_EQ(a, (int2 { 1 >> 3, 2 >> 3 }));
}

TEST(Vector2_Tests, BitwiseRightShiftAssign_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  a >>= b;

  EXPECT_EQ(a, (int2 { 1 >> 3, 2 >> 4 }));
}

TEST(Vec2Bitwise_Tests, BitwiseAnd_Scalar)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
}

TEST(Vec2Bitwise_Tests, BitwiseAnd_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
}

TEST(Vec2Bitwise_Tests, BitwiseOr_Scalar)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
}

TEST(Vec2Bitwise_Tests, BitwiseOr_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
}

TEST(Vec2Bitwise_Tests, BitwiseXor_Scalar)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
}

TEST(Vec2Bitwise_Tests, BitwiseXor_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
}

TEST(Vec2Bitwise_Tests, BitwiseLeftShift_Scalar)
{
  int2 a { 1, 2 };

  int2 c = a << 1;

  EXPECT_EQ(c.x, 1 << 1);
  EXPECT_EQ(c.y, 2 << 1);
}

TEST(Vec2Bitwise_Tests, BitwiseLeftShift_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a << b;

  EXPECT_EQ(c.x, 1 << 3);
  EXPECT_EQ(c.y, 2 << 4);
}

TEST(Vec2Bitwise_Tests, BitwiseRightShift_Scalar)
{
  int2 a { 1, 2 };

  int2 c = a >> 1;

  EXPECT_EQ(c.x, 1 >> 1);
  EXPECT_EQ(c.y, 2 >> 1);
}

TEST(Vec2Bitwise_Tests, BitwiseRightShift_Vec)
{
  int2 a { 1, 2 };
  int2 b { 3, 4 };

  int2 c = a >> b;

  EXPECT_EQ(c.x, 1 >> 3);
  EXPECT_EQ(c.y, 2 >> 4);
}

TEST(Vec2Bitwise_Tests, BitwiseNot_Vec)
{
  int2 a { 1, 2 };

  int2 c = ~a;

  EXPECT_EQ(c.x, ~1);
  EXPECT_EQ(c.y, ~2);
}

// Bool tests

TEST(Vec2Bool_Tests, And)
{
  bool2 a = bool2(true, false);
  bool2 b = bool2(true, true);
  EXPECT_EQ(bool2(true, false), a && b);
}

TEST(Vec2Bool_Tests, Or)
{
  bool2 a = bool2(true, false);
  bool2 b = bool2(true, true);
  EXPECT_EQ(bool2(true, true), a || b);
}
} // namespace genebits::engine::tests