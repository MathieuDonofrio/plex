#include "genebits/engine/math/swizzle.h"

#include <gtest/gtest.h>

#include "genebits/engine/math/vec2.h"
#include "genebits/engine/math/vec3.h"
#include "genebits/engine/math/vec4.h"

namespace genebits::engine::tests
{

template<typename T>
class Swizzle_Tests : public testing::Test
{};

using Swizzle_Test_Types = testing::Types<int, float, double>;

TYPED_TEST_SUITE(Swizzle_Tests, Swizzle_Test_Types);

TYPED_TEST(Swizzle_Tests, ConvertVec2_2Members)
{
  using Vec2 = Vec<TypeParam, 2>;

  Vec2 vec { 1, 2 };
  Vec2 vec2 = vec.yx;

  EXPECT_EQ(vec2.x, vec.y);
  EXPECT_EQ(vec2.y, vec.x);
}

TYPED_TEST(Swizzle_Tests, ConvertVec2_3Members)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 = vec.xyx;

  EXPECT_EQ(vec2.x, vec.x);
  EXPECT_EQ(vec2.y, vec.y);
  EXPECT_EQ(vec2.z, vec.x);
}

TYPED_TEST(Swizzle_Tests, ConvertVec2_4Members)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec4 vec2 = vec.xyxy;

  EXPECT_EQ(vec2.x, vec.x);
  EXPECT_EQ(vec2.y, vec.y);
  EXPECT_EQ(vec2.z, vec.x);
  EXPECT_EQ(vec2.w, vec.y);
}

TYPED_TEST(Swizzle_Tests, ConvertVec3_2Members)
{
  using Vec3 = Vec<TypeParam, 3>;
  using Vec2 = Vec<TypeParam, 2>;

  Vec3 vec { 1, 2, 3 };
  Vec2 vec2 = vec.zx;

  EXPECT_EQ(vec2.x, vec.z);
  EXPECT_EQ(vec2.y, vec.x);
}

TYPED_TEST(Swizzle_Tests, ConvertVec3_3Members)
{
  using Vec3 = Vec<TypeParam, 3>;

  Vec3 vec { 1, 2, 3 };
  Vec3 vec2 = vec.zyx;

  EXPECT_EQ(vec2.x, vec.z);
  EXPECT_EQ(vec2.y, vec.y);
  EXPECT_EQ(vec2.z, vec.x);
}

TYPED_TEST(Swizzle_Tests, ConvertVec3_4Members)
{
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec3 vec { 1, 2, 3 };
  Vec4 vec2 = vec.zyxy;

  EXPECT_EQ(vec2.x, vec.z);
  EXPECT_EQ(vec2.y, vec.y);
  EXPECT_EQ(vec2.z, vec.x);
  EXPECT_EQ(vec2.w, vec.y);
}

TYPED_TEST(Swizzle_Tests, ConvertVec4_2Members)
{
  using Vec4 = Vec<TypeParam, 4>;
  using Vec2 = Vec<TypeParam, 2>;

  Vec4 vec { 1, 2, 3, 4 };
  Vec2 vec2 = vec.wz;

  EXPECT_EQ(vec2.x, vec.w);
  EXPECT_EQ(vec2.y, vec.z);
}

TYPED_TEST(Swizzle_Tests, ConvertVec4_3Members)
{
  using Vec4 = Vec<TypeParam, 4>;
  using Vec3 = Vec<TypeParam, 3>;

  Vec4 vec { 1, 2, 3, 4 };
  Vec3 vec2 = vec.wzy;

  EXPECT_EQ(vec2.x, vec.w);
  EXPECT_EQ(vec2.y, vec.z);
  EXPECT_EQ(vec2.z, vec.y);
}

TYPED_TEST(Swizzle_Tests, ConvertVec4_4Members)
{
  using Vec4 = Vec<TypeParam, 4>;

  Vec4 vec { 1, 2, 3, 4 };
  Vec4 vec2 = vec.wzyx;

  EXPECT_EQ(vec2.x, vec.w);
  EXPECT_EQ(vec2.y, vec.z);
  EXPECT_EQ(vec2.z, vec.y);
  EXPECT_EQ(vec2.w, vec.x);
}

TYPED_TEST(Swizzle_Tests, Assign_Scalar)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx = 3;
  vec2.zyx = 4;
  vec3.wzyx = 5;

  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec2.z, 4);
  EXPECT_EQ(vec2.y, 4);
  EXPECT_EQ(vec2.x, 4);
  EXPECT_EQ(vec3.w, 5);
  EXPECT_EQ(vec3.z, 5);
  EXPECT_EQ(vec3.y, 5);
  EXPECT_EQ(vec3.x, 5);
}

TYPED_TEST(Swizzle_Tests, Subscript)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  EXPECT_EQ(vec.yx[0], vec.y);
  EXPECT_EQ(vec.yx[1], vec.x);
  EXPECT_EQ(vec2.zyx[0], vec2.z);
  EXPECT_EQ(vec2.zyx[1], vec2.y);
  EXPECT_EQ(vec2.zyx[2], vec2.x);
  EXPECT_EQ(vec3.wzyx[0], vec3.w);
  EXPECT_EQ(vec3.wzyx[1], vec3.z);
  EXPECT_EQ(vec3.wzyx[2], vec3.y);
  EXPECT_EQ(vec3.wzyx[3], vec3.x);
}

TYPED_TEST(Swizzle_Tests, Assign_Vec)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx = Vec2(3, 4);
  vec2.zyx = Vec3(3, 4, 5);
  vec3.wzyx = Vec4(3, 4, 5, 6);

  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec2.z, 3);
  EXPECT_EQ(vec2.y, 4);
  EXPECT_EQ(vec2.x, 5);
  EXPECT_EQ(vec3.w, 3);
  EXPECT_EQ(vec3.z, 4);
  EXPECT_EQ(vec3.y, 5);
  EXPECT_EQ(vec3.x, 6);
}

TYPED_TEST(Swizzle_Tests, SubAsign_Scalar)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx -= 3;
  vec2.zyx -= 4;
  vec3.wzyx -= 5;

  EXPECT_EQ(vec.x, -2);
  EXPECT_EQ(vec.y, -1);
  EXPECT_EQ(vec2.z, -1);
  EXPECT_EQ(vec2.y, -2);
  EXPECT_EQ(vec2.x, -3);
  EXPECT_EQ(vec3.w, -1);
  EXPECT_EQ(vec3.z, -2);
  EXPECT_EQ(vec3.y, -3);
  EXPECT_EQ(vec3.x, -4);
}

TYPED_TEST(Swizzle_Tests, SubAsign_Vec)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 2, 3 };
  Vec3 vec2 { 2, 3, 5 };
  Vec4 vec3 { 2, 3, 5, 7 };

  vec.yx -= Vec2(1, 2);
  vec2.zyx -= Vec3(1, 2, 3);
  vec3.wzyx -= Vec4(1, 2, 3, 4);

  EXPECT_EQ(vec.x, 0);
  EXPECT_EQ(vec.y, 2);
  EXPECT_EQ(vec2.z, 4);
  EXPECT_EQ(vec2.y, 1);
  EXPECT_EQ(vec2.x, -1);
  EXPECT_EQ(vec3.w, 6);
  EXPECT_EQ(vec3.z, 3);
  EXPECT_EQ(vec3.y, 0);
  EXPECT_EQ(vec3.x, -2);
}

TYPED_TEST(Swizzle_Tests, AddAsign_Scalar)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx += 3;
  vec2.zyx += 4;
  vec3.wzyx += 5;

  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, 5);
  EXPECT_EQ(vec2.z, 7);
  EXPECT_EQ(vec2.y, 6);
  EXPECT_EQ(vec2.x, 5);
  EXPECT_EQ(vec3.w, 9);
  EXPECT_EQ(vec3.z, 8);
  EXPECT_EQ(vec3.y, 7);
  EXPECT_EQ(vec3.x, 6);
}

TYPED_TEST(Swizzle_Tests, AddAsign_Vec)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 2, 3 };
  Vec3 vec2 { 2, 3, 5 };
  Vec4 vec3 { 2, 3, 5, 7 };

  vec.yx += Vec2(1, 2);
  vec2.zyx += Vec3(1, 2, 3);
  vec3.wzyx += Vec4(1, 2, 3, 4);

  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, 4);
  EXPECT_EQ(vec2.z, 6);
  EXPECT_EQ(vec2.y, 5);
  EXPECT_EQ(vec2.x, 5);
  EXPECT_EQ(vec3.w, 8);
  EXPECT_EQ(vec3.z, 7);
  EXPECT_EQ(vec3.y, 6);
  EXPECT_EQ(vec3.x, 6);
}

TYPED_TEST(Swizzle_Tests, MultAssign_Scalar)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx *= 3;
  vec2.zyx *= 4;
  vec3.wzyx *= 5;

  EXPECT_EQ(vec.x, 3);
  EXPECT_EQ(vec.y, 6);
  EXPECT_EQ(vec2.z, 12);
  EXPECT_EQ(vec2.y, 8);
  EXPECT_EQ(vec2.x, 4);
  EXPECT_EQ(vec3.w, 20);
  EXPECT_EQ(vec3.z, 15);
  EXPECT_EQ(vec3.y, 10);
  EXPECT_EQ(vec3.x, 5);
}

TYPED_TEST(Swizzle_Tests, MultAssign_Vec)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 2, 3 };
  Vec3 vec2 { 2, 3, 5 };
  Vec4 vec3 { 2, 3, 5, 7 };

  vec.yx *= Vec2(1, 2);
  vec2.zyx *= Vec3(1, 2, 3);
  vec3.wzyx *= Vec4(1, 2, 3, 4);

  EXPECT_EQ(vec.x, 4);
  EXPECT_EQ(vec.y, 3);
  EXPECT_EQ(vec2.z, 5);
  EXPECT_EQ(vec2.y, 6);
  EXPECT_EQ(vec2.x, 6);
  EXPECT_EQ(vec3.w, 7);
  EXPECT_EQ(vec3.z, 10);
  EXPECT_EQ(vec3.y, 9);
  EXPECT_EQ(vec3.x, 8);
}

TYPED_TEST(Swizzle_Tests, DivAssign_Scalar)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 1, 2 };
  Vec3 vec2 { 1, 2, 3 };
  Vec4 vec3 { 1, 2, 3, 4 };

  vec.yx /= 3;
  vec2.zyx /= 4;
  vec3.wzyx /= 5;

  EXPECT_EQ(vec.x, 1 / static_cast<TypeParam>(3));
  EXPECT_EQ(vec.y, 2 / static_cast<TypeParam>(3));
  EXPECT_EQ(vec2.z, 3 / static_cast<TypeParam>(4));
  EXPECT_EQ(vec2.y, 2 / static_cast<TypeParam>(4));
  EXPECT_EQ(vec2.x, 1 / static_cast<TypeParam>(4));
  EXPECT_EQ(vec3.w, 4 / static_cast<TypeParam>(5));
  EXPECT_EQ(vec3.z, 3 / static_cast<TypeParam>(5));
  EXPECT_EQ(vec3.y, 2 / static_cast<TypeParam>(5));
  EXPECT_EQ(vec3.x, 1 / static_cast<TypeParam>(5));
}

TYPED_TEST(Swizzle_Tests, DivAssign_Vec)
{
  using Vec2 = Vec<TypeParam, 2>;
  using Vec3 = Vec<TypeParam, 3>;
  using Vec4 = Vec<TypeParam, 4>;

  Vec2 vec { 2, 3 };
  Vec3 vec2 { 2, 3, 5 };
  Vec4 vec3 { 2, 3, 5, 7 };

  vec.yx /= Vec2(1, 2);
  vec2.zyx /= Vec3(1, 2, 3);
  vec3.wzyx /= Vec4(1, 2, 3, 4);

  EXPECT_EQ(vec.x, 2 / static_cast<TypeParam>(2));
  EXPECT_EQ(vec.y, 3 / static_cast<TypeParam>(1));
  EXPECT_EQ(vec2.z, 5 / static_cast<TypeParam>(1));
  EXPECT_EQ(vec2.y, 3 / static_cast<TypeParam>(2));
  EXPECT_EQ(vec2.x, 2 / static_cast<TypeParam>(3));
  EXPECT_EQ(vec3.w, 7 / static_cast<TypeParam>(1));
  EXPECT_EQ(vec3.z, 5 / static_cast<TypeParam>(2));
  EXPECT_EQ(vec3.y, 3 / static_cast<TypeParam>(3));
  EXPECT_EQ(vec3.x, 2 / static_cast<TypeParam>(4));
}

} // namespace genebits::engine::tests