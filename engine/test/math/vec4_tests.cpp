#include "genebits/engine/math/vec4.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(sizeof(float4) == sizeof(float) * 4, "float4 size is not 4 floats");

static_assert(std::is_trivially_copy_constructible_v<float4>, "Vec4 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float4>, "Vec4 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float4>, "Vec4 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float4>, "Vec4 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float4>, "Vec4 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float4>, "Vec4 Must be trivially copyable");

template<typename T>
class Vec4_Tests : public testing::Test
{};

using Vec4_Test_Types = testing::Types<int4, float4, double4>;

TYPED_TEST_SUITE(Vec4_Tests, Vec4_Test_Types);

TYPED_TEST(Vec4_Tests, Constructor_Default_ZeroInitialize)
{
  TypeParam vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 0);
  EXPECT_EQ(vec.z, 0);
  EXPECT_EQ(vec.w, 0);
}

TYPED_TEST(Vec4_Tests, Constructor_Scalar_Initialize)
{
  TypeParam vec(1);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 1);
  EXPECT_EQ(vec.w, 1);
}

TYPED_TEST(Vec4_Tests, Constructor_Value_Initialize)
{
  TypeParam vec(1, 2, 3, 4);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
  EXPECT_EQ(vec.w, 4);
}

TYPED_TEST(Vec4_Tests, Constructor_Copy_Initialize)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, Constructor_CopyFromOtherType_Initialize)
{
  TypeParam vec(1, 2, 3, 4);

  if constexpr (std::is_same_v<TypeParam, float4>)
  {
    int4 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
    EXPECT_EQ(vec2.z, 3);
    EXPECT_EQ(vec2.w, 4);
  }
  else
  {
    float4 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
    EXPECT_EQ(vec2.z, 3);
    EXPECT_EQ(vec2.w, 4);
  }
}

TYPED_TEST(Vec4_Tests, Constructor_Move)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(std::move(vec));
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, Assignment_Copy_Assign)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2;
  vec2 = vec;
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, Assignment_Move_Assign)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2;
  vec2 = std::move(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, AddAssign_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  vec += 1;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
  EXPECT_EQ(vec.w, 5);
}

TYPED_TEST(Vec4_Tests, AddAssign_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  vec += TypeParam(10, 11, 12, 13);
  EXPECT_EQ(vec.x, 11);
  EXPECT_EQ(vec.y, 13);
  EXPECT_EQ(vec.z, 15);
  EXPECT_EQ(vec.w, 17);
}

TYPED_TEST(Vec4_Tests, SubAssign_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  vec -= 1;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec.w, 3);
}

TYPED_TEST(Vec4_Tests, SubAssign_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  vec -= TypeParam(2, 4, 6, 8);
  EXPECT_EQ(vec.x, -1);
  EXPECT_EQ(vec.y, -2);
  EXPECT_EQ(vec.z, -3);
  EXPECT_EQ(vec.w, -4);
}

TYPED_TEST(Vec4_Tests, MultAssign_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 6);
  EXPECT_EQ(vec.w, 8);
}

TYPED_TEST(Vec4_Tests, MultAssign_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  vec *= TypeParam(3, 2, 1, 4);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 3);
  EXPECT_EQ(vec.w, 16);
}

TYPED_TEST(Vec4_Tests, DivAssign_Scalar)
{
  TypeParam vec(2, 4, 6, 8);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
  EXPECT_EQ(vec.w, 4);
}

TYPED_TEST(Vec4_Tests, DivAssign_Vec)
{
  TypeParam vec(2, 4, 6, 8);
  vec /= TypeParam(2, 1, 2, 4);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 3);
  EXPECT_EQ(vec.w, 2);
}

TYPED_TEST(Vec4_Tests, Increment_Pre)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = ++vec;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
  EXPECT_EQ(vec.w, 5);
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 4);
  EXPECT_EQ(vec2.w, 5);
}

TYPED_TEST(Vec4_Tests, Increment_Post)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec++;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
  EXPECT_EQ(vec.w, 5);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, Decrement_Pre)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = --vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec.w, 3);
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
  EXPECT_EQ(vec2.z, 2);
  EXPECT_EQ(vec2.w, 3);
}

TYPED_TEST(Vec4_Tests, Decrement_Post)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec--;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec.w, 3);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.w, 4);
}

TYPED_TEST(Vec4_Tests, ArrayAccess_Get)
{
  TypeParam vec(1, 2, 3, 4);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
}

TYPED_TEST(Vec4_Tests, ArrayAccess_ConstGet)
{
  const TypeParam vec(1, 2, 3, 4);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
}

TYPED_TEST(Vec4_Tests, ArrayAccess_Set)
{
  TypeParam vec(1, 2, 3, 4);
  vec[0] = 3;
  vec[1] = 4;
  vec[2] = 5;
  vec[3] = 6;
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 5);
  EXPECT_EQ(vec.w, 6);
}

TYPED_TEST(Vec4_Tests, BoolConversion)
{
  TypeParam vec(1, 2, 3, 4);
  EXPECT_TRUE(vec);
  vec = 0;
  EXPECT_FALSE(vec);
}

TYPED_TEST(Vec4_Tests, Add_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec + 1;
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 4);
  EXPECT_EQ(vec2.w, 5);
}

TYPED_TEST(Vec4_Tests, Add_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec + TypeParam(3, 1, 2, 3);
  EXPECT_EQ(vec2.x, 4);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 5);
  EXPECT_EQ(vec2.w, 7);
}

TYPED_TEST(Vec4_Tests, Sub_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec - 1;
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
  EXPECT_EQ(vec2.z, 2);
  EXPECT_EQ(vec2.w, 3);
}

TYPED_TEST(Vec4_Tests, Sub_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2 = vec - TypeParam(3, 2, 1, 3);
  EXPECT_EQ(vec2.x, -2);
  EXPECT_EQ(vec2.y, 0);
  EXPECT_EQ(vec2.z, 2);
  EXPECT_EQ(vec2.w, 1);
}

TYPED_TEST(Vec4_Tests, Mult_Scalar)
{
  TypeParam vec(1, 2, 3, 4);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 6);
  EXPECT_EQ(vec.w, 8);
}

TYPED_TEST(Vec4_Tests, Mult_Vec)
{
  TypeParam vec(1, 2, 3, 4);
  vec *= TypeParam(2, 3, 4, 5);
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 6);
  EXPECT_EQ(vec.z, 12);
  EXPECT_EQ(vec.w, 20);
}

TYPED_TEST(Vec4_Tests, Div_Scalar)
{
  TypeParam vec(2, 4, 6, 8);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
  EXPECT_EQ(vec.w, 4);
}

TYPED_TEST(Vec4_Tests, Div_Vec)
{
  TypeParam vec(2, 4, 6, 8);
  vec /= TypeParam(2, 1, 3, 4);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec.w, 2);
}

TYPED_TEST(Vec4_Tests, Equals_True)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(1, 2, 3, 4);
  EXPECT_TRUE(vec == vec2);
}

TYPED_TEST(Vec4_Tests, Equals_False)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(2, 2, 3, 1);
  EXPECT_FALSE(vec == vec2);
}

TYPED_TEST(Vec4_Tests, NotEquals_True)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(1, 2, 2, 4);
  EXPECT_TRUE(vec != vec2);
}

TYPED_TEST(Vec4_Tests, NotEquals_False)
{
  TypeParam vec(1, 2, 3, 4);
  TypeParam vec2(1, 2, 3, 4);
  EXPECT_FALSE(vec != vec2);
}

// Bitwise tests

TEST(Vec4Bitwise_Tests, BitwiseAndAssign_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  a &= 3;

  EXPECT_EQ(a, (int4 { 1 & 3, 2 & 3, 3 & 3, 4 & 3 }));
}

TEST(Vec4Bitwise_Tests, BitwiseAndAssign_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  a &= b;

  EXPECT_EQ(a, (int4 { 1 & 3, 2 & 4, 3 & 5, 4 & 6 }));
}

TEST(Vec4Bitwise_Tests, BitwiseOrAssign_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  a |= 3;

  EXPECT_EQ(a, (int4 { 1 | 3, 2 | 3, 3 | 3, 4 | 3 }));
}

TEST(Vec4Bitwise_Tests, BitwiseOrAssign_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  a |= b;

  EXPECT_EQ(a, (int4 { 1 | 3, 2 | 4, 3 | 5, 4 | 6 }));
}

TEST(Vec4Bitwise_Tests, BitwiseXorAssign_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  a ^= 3;

  EXPECT_EQ(a, (int4 { 1 ^ 3, 2 ^ 3, 3 ^ 3, 4 ^ 3 }));
}

TEST(Vec4Bitwise_Tests, BitwiseXorAssign_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  a ^= b;

  EXPECT_EQ(a, (int4 { 1 ^ 3, 2 ^ 4, 3 ^ 5, 4 ^ 6 }));
}

TEST(Vec4Bitwise_Tests, BitwiseLeftShiftAssign_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  a <<= 3;

  EXPECT_EQ(a, (int4 { 1 << 3, 2 << 3, 3 << 3, 4 << 3 }));
}

TEST(Vec4Bitwise_Tests, BitwiseLeftShiftAssign_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  a <<= b;

  EXPECT_EQ(a, (int4 { 1 << 3, 2 << 4, 3 << 5, 4 << 6 }));
}

TEST(Vec4Bitwise_Tests, BitwiseRightShiftAssign_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  a >>= 3;

  EXPECT_EQ(a, (int4 { 1 >> 3, 2 >> 3, 3 >> 3, 4 >> 3 }));
}

TEST(Vec4Bitwise_Tests, BitwiseRightShiftAssign_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  a >>= b;

  EXPECT_EQ(a, (int4 { 1 >> 3, 2 >> 4, 3 >> 5, 4 >> 6 }));
}

TEST(Vec4Bitwise_Tests, BitwiseAnd_Scalar)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
  EXPECT_EQ(c.z, 3 & 5);
  EXPECT_EQ(c.w, 4 & 6);
}

TEST(Vec4Bitwise_Tests, BitwiseAnd_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
  EXPECT_EQ(c.z, 3 & 5);
  EXPECT_EQ(c.w, 4 & 6);
}

TEST(Vec4Bitwise_Tests, BitwiseOr_Scalar)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
  EXPECT_EQ(c.z, 3 | 5);
  EXPECT_EQ(c.w, 4 | 6);
}

TEST(Vec4Bitwise_Tests, BitwiseOr_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
  EXPECT_EQ(c.z, 3 | 5);
  EXPECT_EQ(c.w, 4 | 6);
}

TEST(Vec4Bitwise_Tests, BitwiseXor_Scalar)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
  EXPECT_EQ(c.z, 3 ^ 5);
  EXPECT_EQ(c.w, 4 ^ 6);
}

TEST(Vec4Bitwise_Tests, BitwiseXor_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
  EXPECT_EQ(c.z, 3 ^ 5);
  EXPECT_EQ(c.w, 4 ^ 6);
}

TEST(Vec4Bitwise_Tests, BitwiseLeftShift_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  int4 c = a << 1;

  EXPECT_EQ(c.x, 1 << 1);
  EXPECT_EQ(c.y, 2 << 1);
  EXPECT_EQ(c.z, 3 << 1);
  EXPECT_EQ(c.w, 4 << 1);
}

TEST(Vec4Bitwise_Tests, BitwiseLeftShift_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a << b;

  EXPECT_EQ(c.x, 1 << 3);
  EXPECT_EQ(c.y, 2 << 4);
  EXPECT_EQ(c.z, 3 << 5);
  EXPECT_EQ(c.w, 4 << 6);
}

TEST(Vec4Bitwise_Tests, BitwiseRightShift_Scalar)
{
  int4 a { 1, 2, 3, 4 };

  int4 c = a >> 1;

  EXPECT_EQ(c.x, 1 >> 1);
  EXPECT_EQ(c.y, 2 >> 1);
  EXPECT_EQ(c.z, 3 >> 1);
  EXPECT_EQ(c.w, 4 >> 1);
}

TEST(Vec4Bitwise_Tests, BitwiseRightShift_Vec)
{
  int4 a { 1, 2, 3, 4 };
  int4 b { 3, 4, 5, 6 };

  int4 c = a >> b;

  EXPECT_EQ(c.x, 1 >> 3);
  EXPECT_EQ(c.y, 2 >> 4);
  EXPECT_EQ(c.z, 3 >> 5);
  EXPECT_EQ(c.w, 4 >> 6);
}

TEST(Vec4Bitwise_Tests, BitwiseNot_Vec)
{
  int4 a { 1, 2, 3, 4 };

  int4 c = ~a;

  EXPECT_EQ(c.x, ~1);
  EXPECT_EQ(c.y, ~2);
  EXPECT_EQ(c.z, ~3);
  EXPECT_EQ(c.w, ~4);
}

// Bool tests

TEST(Vec4Bool_Tests, And)
{
  bool4 a = bool4(true, false, true, true);
  bool4 b = bool4(true, true, false, true);
  EXPECT_EQ(bool4(true, false, false, true), a && b);
}

TEST(Vec4Bool_Tests, Or)
{
  bool4 a = bool4(true, false, false, true);
  bool4 b = bool4(true, true, false, false);
  EXPECT_EQ(bool4(true, true, false, true), a || b);
}
} // namespace genebits::engine::tests