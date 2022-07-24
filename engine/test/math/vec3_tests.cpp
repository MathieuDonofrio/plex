#include "genebits/engine/math/vec3.h"

#include <gtest/gtest.h>

namespace plex::tests
{
static_assert(sizeof(float3) == sizeof(float) * 3, "float3 size is not 4 floats");

static_assert(std::is_trivially_copy_constructible_v<float3>, "Vec3 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float3>, "Vec3 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float3>, "Vec3 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float3>, "Vec3 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float3>, "Vec3 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float3>, "Vec3 Must be trivially copyable");

template<typename T>
class Vec3_Tests : public testing::Test
{};

using Vec3_Test_Types = testing::Types<int3, float3, double3>;

TYPED_TEST_SUITE(Vec3_Tests, Vec3_Test_Types);

TYPED_TEST(Vec3_Tests, Constructor_Default_ZeroInitialize)
{
  TypeParam vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 0);
  EXPECT_EQ(vec.z, 0);
}

TYPED_TEST(Vec3_Tests, Constructor_Scalar_Initialize)
{
  TypeParam vec(1);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 1);
}

TYPED_TEST(Vec3_Tests, Constructor_Value_Initialize)
{
  TypeParam vec(1, 2, 3);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
}

TYPED_TEST(Vec3_Tests, Constructor_Copy_Initialize)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, Constructor_CopyFromOtherType_Initialize)
{
  TypeParam vec(1, 2, 3);

  if constexpr (std::is_same_v<TypeParam, float3>)
  {
    int3 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
    EXPECT_EQ(vec2.z, 3);
  }
  else
  {
    float3 vec2(vec);
    EXPECT_EQ(vec2.x, 1);
    EXPECT_EQ(vec2.y, 2);
    EXPECT_EQ(vec2.z, 3);
  }
}

TYPED_TEST(Vec3_Tests, Constructor_Move)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(std::move(vec));
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, Assignment_Copy_Assign)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2;
  vec2 = vec;
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, Assignment_Move_Assign)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2;
  vec2 = std::move(vec);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, AddAssign_Scalar)
{
  TypeParam vec(1, 2, 3);
  vec += 1;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
}

TYPED_TEST(Vec3_Tests, AddAssign_Vec)
{
  TypeParam vec(1, 2, 3);
  vec += TypeParam(10, 11, 12);
  EXPECT_EQ(vec.x, 11);
  EXPECT_EQ(vec.y, 13);
  EXPECT_EQ(vec.z, 15);
}

TYPED_TEST(Vec3_Tests, SubAssign_Scalar)
{
  TypeParam vec(1, 2, 3);
  vec -= 1;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
}

TYPED_TEST(Vec3_Tests, SubAssign_Vec)
{
  TypeParam vec(1, 2, 3);
  vec -= TypeParam(2, 4, 6);
  EXPECT_EQ(vec.x, -1);
  EXPECT_EQ(vec.y, -2);
  EXPECT_EQ(vec.z, -3);
}

TYPED_TEST(Vec3_Tests, MultAssign_Scalar)
{
  TypeParam vec(1, 2, 3);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 6);
}

TYPED_TEST(Vec3_Tests, MultAssign_Vec)
{
  TypeParam vec(1, 2, 3);
  vec *= TypeParam(3, 2, 1);
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 3);
}

TYPED_TEST(Vec3_Tests, DivAssign_Scalar)
{
  TypeParam vec(2, 4, 6);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
}

TYPED_TEST(Vec3_Tests, DivAssign_Vec)
{
  TypeParam vec(2, 4, 6);
  vec /= TypeParam(2, 1, 2);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 3);
}

TYPED_TEST(Vec3_Tests, Increment_Pre)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = ++vec;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 4);
}

TYPED_TEST(Vec3_Tests, Increment_Post)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec++;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec.z, 4);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, Decrement_Pre)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = --vec;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
  EXPECT_EQ(vec2.z, 2);
}

TYPED_TEST(Vec3_Tests, Decrement_Post)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec--;
  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 1);
  EXPECT_EQ(vec.z, 2);
  EXPECT_EQ(vec2.x, 1);
  EXPECT_EQ(vec2.y, 2);
  EXPECT_EQ(vec2.z, 3);
}

TYPED_TEST(Vec3_Tests, ArrayAccess_Get)
{
  TypeParam vec(1, 2, 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

TYPED_TEST(Vec3_Tests, ArrayAccess_ConstGet)
{
  const TypeParam vec(1, 2, 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

TYPED_TEST(Vec3_Tests, ArrayAccess_Set)
{
  TypeParam vec(1, 2, 3);
  vec[0] = 3;
  vec[1] = 4;
  vec[2] = 5;
  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 5);
}

TYPED_TEST(Vec3_Tests, BoolConversion)
{
  TypeParam vec(1, 2, 3);
  EXPECT_TRUE(vec);
  vec = 0;
  EXPECT_FALSE(vec);
}

TYPED_TEST(Vec3_Tests, Add_Scalar)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec + 1;
  EXPECT_EQ(vec2.x, 2);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 4);
}

TYPED_TEST(Vec3_Tests, Add_Vec)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec + TypeParam(3, 1, 2);
  EXPECT_EQ(vec2.x, 4);
  EXPECT_EQ(vec2.y, 3);
  EXPECT_EQ(vec2.z, 5);
}

TYPED_TEST(Vec3_Tests, Sub_Scalar)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec - 1;
  EXPECT_EQ(vec2.x, 0);
  EXPECT_EQ(vec2.y, 1);
  EXPECT_EQ(vec2.z, 2);
}

TYPED_TEST(Vec3_Tests, Sub_Vec)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2 = vec - TypeParam(3, 2, 1);
  EXPECT_EQ(vec2.x, -2);
  EXPECT_EQ(vec2.y, 0);
  EXPECT_EQ(vec2.z, 2);
}

TYPED_TEST(Vec3_Tests, Mult_Scalar)
{
  TypeParam vec(1, 2, 3);
  vec *= 2;
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 6);
}

TYPED_TEST(Vec3_Tests, Mult_Vec)
{
  TypeParam vec(1, 2, 3);
  vec *= TypeParam(2, 3, 4);
  EXPECT_EQ(vec.x, 2);
  EXPECT_EQ(vec.y, 6);
  EXPECT_EQ(vec.z, 12);
}

TYPED_TEST(Vec3_Tests, Div_Scalar)
{
  TypeParam vec(2, 4, 6);
  vec /= 2;
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec.z, 3);
}

TYPED_TEST(Vec3_Tests, Div_Vec)
{
  TypeParam vec(2, 4, 6);
  vec /= TypeParam(2, 1, 3);
  EXPECT_EQ(vec.x, 1);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec.z, 2);
}

TYPED_TEST(Vec3_Tests, Equals_True)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(1, 2, 3);
  EXPECT_TRUE(vec == vec2);
}

TYPED_TEST(Vec3_Tests, Equals_False)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(2, 2, 3);
  EXPECT_FALSE(vec == vec2);
}

TYPED_TEST(Vec3_Tests, NotEquals_True)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(1, 2, 2);
  EXPECT_TRUE(vec != vec2);
}

TYPED_TEST(Vec3_Tests, NotEquals_False)
{
  TypeParam vec(1, 2, 3);
  TypeParam vec2(1, 2, 3);
  EXPECT_FALSE(vec != vec2);
}

// Bitwise tests

TEST(Vec3Bitwise_Tests, BitwiseAndAssign_Scalar)
{
  int3 a { 1, 2, 3 };

  a &= 3;

  EXPECT_EQ(a, (int3 { 1 & 3, 2 & 3, 3 & 3 }));
}

TEST(Vec3Bitwise_Tests, BitwiseAndAssign_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  a &= b;

  EXPECT_EQ(a, (int3 { 1 & 3, 2 & 4, 3 & 5 }));
}

TEST(Vec3Bitwise_Tests, BitwiseOrAssign_Scalar)
{
  int3 a { 1, 2, 3 };

  a |= 3;

  EXPECT_EQ(a, (int3 { 1 | 3, 2 | 3, 3 | 3 }));
}

TEST(Vec3Bitwise_Tests, BitwiseOrAssign_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  a |= b;

  EXPECT_EQ(a, (int3 { 1 | 3, 2 | 4, 3 | 5 }));
}

TEST(Vec3Bitwise_Tests, BitwiseXorAssign_Scalar)
{
  int3 a { 1, 2, 3 };

  a ^= 3;

  EXPECT_EQ(a, (int3 { 1 ^ 3, 2 ^ 3, 3 ^ 3 }));
}

TEST(Vec3Bitwise_Tests, BitwiseXorAssign_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  a ^= b;

  EXPECT_EQ(a, (int3 { 1 ^ 3, 2 ^ 4, 3 ^ 5 }));
}

TEST(Vec3Bitwise_Tests, BitwiseLeftShiftAssign_Scalar)
{
  int3 a { 1, 2, 3 };

  a <<= 3;

  EXPECT_EQ(a, (int3 { 1 << 3, 2 << 3, 3 << 3 }));
}

TEST(Vec3Bitwise_Tests, BitwiseLeftShiftAssign_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  a <<= b;

  EXPECT_EQ(a, (int3 { 1 << 3, 2 << 4, 3 << 5 }));
}

TEST(Vec3Bitwise_Tests, BitwiseRightShiftAssign_Scalar)
{
  int3 a { 1, 2, 3 };

  a >>= 3;

  EXPECT_EQ(a, (int3 { 1 >> 3, 2 >> 3, 3 >> 3 }));
}

TEST(Vec3Bitwise_Tests, BitwiseRightShiftAssign_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  a >>= b;

  EXPECT_EQ(a, (int3 { 1 >> 3, 2 >> 4, 3 >> 5 }));
}

TEST(Vec3Bitwise_Tests, BitwiseAnd_Scalar)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
  EXPECT_EQ(c.z, 3 & 5);
}

TEST(Vec3Bitwise_Tests, BitwiseAnd_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a & b;

  EXPECT_EQ(c.x, 1 & 3);
  EXPECT_EQ(c.y, 2 & 4);
  EXPECT_EQ(c.z, 3 & 5);
}

TEST(Vec3Bitwise_Tests, BitwiseOr_Scalar)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
  EXPECT_EQ(c.z, 3 | 5);
}

TEST(Vec3Bitwise_Tests, BitwiseOr_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a | b;

  EXPECT_EQ(c.x, 1 | 3);
  EXPECT_EQ(c.y, 2 | 4);
  EXPECT_EQ(c.z, 3 | 5);
}

TEST(Vec3Bitwise_Tests, BitwiseXor_Scalar)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
  EXPECT_EQ(c.z, 3 ^ 5);
}

TEST(Vec3Bitwise_Tests, BitwiseXor_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a ^ b;

  EXPECT_EQ(c.x, 1 ^ 3);
  EXPECT_EQ(c.y, 2 ^ 4);
  EXPECT_EQ(c.z, 3 ^ 5);
}

TEST(Vec3Bitwise_Tests, BitwiseLeftShift_Scalar)
{
  int3 a { 1, 2, 3 };

  int3 c = a << 1;

  EXPECT_EQ(c.x, 1 << 1);
  EXPECT_EQ(c.y, 2 << 1);
  EXPECT_EQ(c.z, 3 << 1);
}

TEST(Vec3Bitwise_Tests, BitwiseLeftShift_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a << b;

  EXPECT_EQ(c.x, 1 << 3);
  EXPECT_EQ(c.y, 2 << 4);
  EXPECT_EQ(c.z, 3 << 5);
}

TEST(Vec3Bitwise_Tests, BitwiseRightShift_Scalar)
{
  int3 a { 1, 2, 3 };

  int3 c = a >> 1;

  EXPECT_EQ(c.x, 1 >> 1);
  EXPECT_EQ(c.y, 2 >> 1);
  EXPECT_EQ(c.z, 3 >> 1);
}

TEST(Vec3Bitwise_Tests, BitwiseRightShift_Vec)
{
  int3 a { 1, 2, 3 };
  int3 b { 3, 4, 5 };

  int3 c = a >> b;

  EXPECT_EQ(c.x, 1 >> 3);
  EXPECT_EQ(c.y, 2 >> 4);
  EXPECT_EQ(c.z, 3 >> 5);
}

TEST(Vec3Bitwise_Tests, BitwiseNot_Vec)
{
  int3 a { 1, 2, 3 };

  int3 c = ~a;

  EXPECT_EQ(c.x, ~1);
  EXPECT_EQ(c.y, ~2);
  EXPECT_EQ(c.z, ~3);
}

// Bool tests

TEST(Vec3Bool_Tests, And)
{
  bool3 a = bool3(true, false, true);
  bool3 b = bool3(true, true, false);
  EXPECT_EQ(bool3(true, false, false), a && b);
}

TEST(Vec3Bool_Tests, Or)
{
  bool3 a = bool3(true, false, false);
  bool3 b = bool3(true, true, false);
  EXPECT_EQ(bool3(true, true, false), a || b);
}
} // namespace plex::tests