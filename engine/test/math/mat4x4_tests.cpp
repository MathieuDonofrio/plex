#include "genebits/engine/math/mat4x4.h"

#include <gtest/gtest.h>

namespace plex::tests
{

static_assert(std::is_trivially_copy_constructible_v<float4x4>, "Mat4x4 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float4x4>, "Mat4x4 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float4x4>, "Mat4x4 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float4x4>, "Mat4x4 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float4x4>, "Mat4x4 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float4x4>, "Mat4x4 Must be trivially copyable");

template<typename T>
class Mat4x4_Tests : public testing::Test
{};

using Mat4x4_Test_Types = testing::Types<int4x4, float4x4, double4x4>;

TYPED_TEST_SUITE(Mat4x4_Tests, Mat4x4_Test_Types);

TYPED_TEST(Mat4x4_Tests, Constructor_Default)
{
  TypeParam mat;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[0][3], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 1);
  EXPECT_EQ(mat[1][2], 0);
  EXPECT_EQ(mat[1][3], 0);
  EXPECT_EQ(mat[2][0], 0);
  EXPECT_EQ(mat[2][1], 0);
  EXPECT_EQ(mat[2][2], 1);
  EXPECT_EQ(mat[2][3], 0);
  EXPECT_EQ(mat[3][0], 0);
  EXPECT_EQ(mat[3][1], 0);
  EXPECT_EQ(mat[3][2], 0);
  EXPECT_EQ(mat[3][3], 1);
}

TYPED_TEST(Mat4x4_Tests, Constructor_Scalar)
{
  TypeParam mat(2);
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[0][3], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 2);
  EXPECT_EQ(mat[1][2], 0);
  EXPECT_EQ(mat[1][3], 0);
  EXPECT_EQ(mat[2][0], 0);
  EXPECT_EQ(mat[2][1], 0);
  EXPECT_EQ(mat[2][2], 2);
  EXPECT_EQ(mat[2][3], 0);
  EXPECT_EQ(mat[3][0], 0);
  EXPECT_EQ(mat[3][1], 0);
  EXPECT_EQ(mat[3][2], 0);
  EXPECT_EQ(mat[3][3], 2);
}

TYPED_TEST(Mat4x4_Tests, Constructor_Values)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[0][3], 4);
  EXPECT_EQ(mat[1][0], 5);
  EXPECT_EQ(mat[1][1], 6);
  EXPECT_EQ(mat[1][2], 7);
  EXPECT_EQ(mat[1][3], 8);
  EXPECT_EQ(mat[2][0], 9);
  EXPECT_EQ(mat[2][1], 10);
  EXPECT_EQ(mat[2][2], 11);
  EXPECT_EQ(mat[2][3], 12);
  EXPECT_EQ(mat[3][0], 13);
  EXPECT_EQ(mat[3][1], 14);
  EXPECT_EQ(mat[3][2], 15);
  EXPECT_EQ(mat[3][3], 16);
}

TYPED_TEST(Mat4x4_Tests, Constructor_Columns)
{
  TypeParam mat({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 });
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[0][3], 4);
  EXPECT_EQ(mat[1][0], 5);
  EXPECT_EQ(mat[1][1], 6);
  EXPECT_EQ(mat[1][2], 7);
  EXPECT_EQ(mat[1][3], 8);
  EXPECT_EQ(mat[2][0], 9);
  EXPECT_EQ(mat[2][1], 10);
  EXPECT_EQ(mat[2][2], 11);
  EXPECT_EQ(mat[2][3], 12);
  EXPECT_EQ(mat[3][0], 13);
  EXPECT_EQ(mat[3][1], 14);
  EXPECT_EQ(mat[3][2], 15);
  EXPECT_EQ(mat[3][3], 16);
}

TYPED_TEST(Mat4x4_Tests, Constructor_CopyFromOtherType)
{
  TypeParam mat(5);

  if constexpr (std::is_same_v<TypeParam, float4x4>)
  {
    int4x4 mat2(mat);
    EXPECT_EQ(mat[0][0], 5);
    EXPECT_EQ(mat[0][1], 0);
    EXPECT_EQ(mat[0][2], 0);
    EXPECT_EQ(mat[0][3], 0);
    EXPECT_EQ(mat[1][0], 0);
    EXPECT_EQ(mat[1][1], 5);
    EXPECT_EQ(mat[1][2], 0);
    EXPECT_EQ(mat[1][3], 0);
    EXPECT_EQ(mat[2][0], 0);
    EXPECT_EQ(mat[2][1], 0);
    EXPECT_EQ(mat[2][2], 5);
    EXPECT_EQ(mat[2][3], 0);
    EXPECT_EQ(mat[3][0], 0);
    EXPECT_EQ(mat[3][1], 0);
    EXPECT_EQ(mat[3][2], 0);
    EXPECT_EQ(mat[3][3], 5);
  }
  else
  {
    float4x4 mat2(mat);
    EXPECT_EQ(mat[0][0], 5);
    EXPECT_EQ(mat[0][1], 0);
    EXPECT_EQ(mat[0][2], 0);
    EXPECT_EQ(mat[0][3], 0);
    EXPECT_EQ(mat[1][0], 0);
    EXPECT_EQ(mat[1][1], 5);
    EXPECT_EQ(mat[1][2], 0);
    EXPECT_EQ(mat[1][3], 0);
    EXPECT_EQ(mat[2][0], 0);
    EXPECT_EQ(mat[2][1], 0);
    EXPECT_EQ(mat[2][2], 5);
    EXPECT_EQ(mat[2][3], 0);
    EXPECT_EQ(mat[3][0], 0);
    EXPECT_EQ(mat[3][1], 0);
    EXPECT_EQ(mat[3][2], 0);
    EXPECT_EQ(mat[3][3], 5);
  }
}

TYPED_TEST(Mat4x4_Tests, AddAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  mat += 2;
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[0][2], 5);
  EXPECT_EQ(mat[0][3], 6);
  EXPECT_EQ(mat[1][0], 7);
  EXPECT_EQ(mat[1][1], 8);
  EXPECT_EQ(mat[1][2], 9);
  EXPECT_EQ(mat[1][3], 10);
  EXPECT_EQ(mat[2][0], 11);
  EXPECT_EQ(mat[2][1], 12);
  EXPECT_EQ(mat[2][2], 13);
  EXPECT_EQ(mat[2][3], 14);
  EXPECT_EQ(mat[3][0], 15);
  EXPECT_EQ(mat[3][1], 16);
  EXPECT_EQ(mat[3][2], 17);
  EXPECT_EQ(mat[3][3], 18);
}

TYPED_TEST(Mat4x4_Tests, AddAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  mat += TypeParam(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17);
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 5);
  EXPECT_EQ(mat[0][2], 7);
  EXPECT_EQ(mat[0][3], 9);
  EXPECT_EQ(mat[1][0], 11);
  EXPECT_EQ(mat[1][1], 13);
  EXPECT_EQ(mat[1][2], 15);
  EXPECT_EQ(mat[1][3], 17);
  EXPECT_EQ(mat[2][0], 19);
  EXPECT_EQ(mat[2][1], 21);
  EXPECT_EQ(mat[2][2], 23);
  EXPECT_EQ(mat[2][3], 25);
  EXPECT_EQ(mat[3][0], 27);
  EXPECT_EQ(mat[3][1], 29);
  EXPECT_EQ(mat[3][2], 31);
  EXPECT_EQ(mat[3][3], 33);
}

TYPED_TEST(Mat4x4_Tests, SubAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  mat -= 2;
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 1);
  EXPECT_EQ(mat[0][3], 2);
  EXPECT_EQ(mat[1][0], 3);
  EXPECT_EQ(mat[1][1], 4);
  EXPECT_EQ(mat[1][2], 5);
  EXPECT_EQ(mat[1][3], 6);
  EXPECT_EQ(mat[2][0], 7);
  EXPECT_EQ(mat[2][1], 8);
  EXPECT_EQ(mat[2][2], 9);
  EXPECT_EQ(mat[2][3], 10);
  EXPECT_EQ(mat[3][0], 11);
  EXPECT_EQ(mat[3][1], 12);
  EXPECT_EQ(mat[3][2], 13);
  EXPECT_EQ(mat[3][3], 14);
}

TYPED_TEST(Mat4x4_Tests, SubAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  mat -= TypeParam(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17);
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], -1);
  EXPECT_EQ(mat[0][2], -1);
  EXPECT_EQ(mat[0][3], -1);
  EXPECT_EQ(mat[1][0], -1);
  EXPECT_EQ(mat[1][1], -1);
  EXPECT_EQ(mat[1][2], -1);
  EXPECT_EQ(mat[1][3], -1);
  EXPECT_EQ(mat[2][0], -1);
  EXPECT_EQ(mat[2][1], -1);
  EXPECT_EQ(mat[2][2], -1);
  EXPECT_EQ(mat[2][3], -1);
  EXPECT_EQ(mat[3][0], -1);
  EXPECT_EQ(mat[3][1], -1);
  EXPECT_EQ(mat[3][2], -1);
  EXPECT_EQ(mat[3][3], -1);
}

TYPED_TEST(Mat4x4_Tests, MulAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  mat *= 2;
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[0][2], 6);
  EXPECT_EQ(mat[0][3], 8);
  EXPECT_EQ(mat[1][0], 10);
  EXPECT_EQ(mat[1][1], 12);
  EXPECT_EQ(mat[1][2], 14);
  EXPECT_EQ(mat[1][3], 16);
  EXPECT_EQ(mat[2][0], 18);
  EXPECT_EQ(mat[2][1], 20);
  EXPECT_EQ(mat[2][2], 22);
  EXPECT_EQ(mat[2][3], 24);
  EXPECT_EQ(mat[3][0], 26);
  EXPECT_EQ(mat[3][1], 28);
  EXPECT_EQ(mat[3][2], 30);
  EXPECT_EQ(mat[3][3], 32);
}

TYPED_TEST(Mat4x4_Tests, DivAssign_Scalar)
{
  TypeParam mat(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);
  mat /= 2;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[0][3], 4);
  EXPECT_EQ(mat[1][0], 5);
  EXPECT_EQ(mat[1][1], 6);
  EXPECT_EQ(mat[1][2], 7);
  EXPECT_EQ(mat[1][3], 8);
  EXPECT_EQ(mat[2][0], 9);
  EXPECT_EQ(mat[2][1], 10);
  EXPECT_EQ(mat[2][2], 11);
  EXPECT_EQ(mat[2][3], 12);
  EXPECT_EQ(mat[3][0], 13);
  EXPECT_EQ(mat[3][1], 14);
  EXPECT_EQ(mat[3][2], 15);
  EXPECT_EQ(mat[3][3], 16);
}

TYPED_TEST(Mat4x4_Tests, PreIncrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = ++mat;
  EXPECT_EQ(mat2[0][0], 2);
  EXPECT_EQ(mat2[0][1], 3);
  EXPECT_EQ(mat2[0][2], 4);
  EXPECT_EQ(mat2[0][3], 5);
  EXPECT_EQ(mat2[1][0], 6);
  EXPECT_EQ(mat2[1][1], 7);
  EXPECT_EQ(mat2[1][2], 8);
  EXPECT_EQ(mat2[1][3], 9);
  EXPECT_EQ(mat2[2][0], 10);
  EXPECT_EQ(mat2[2][1], 11);
  EXPECT_EQ(mat2[2][2], 12);
  EXPECT_EQ(mat2[2][3], 13);
  EXPECT_EQ(mat2[3][0], 14);
  EXPECT_EQ(mat2[3][1], 15);
  EXPECT_EQ(mat2[3][2], 16);
  EXPECT_EQ(mat2[3][3], 17);
}

TYPED_TEST(Mat4x4_Tests, PostIncrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = mat++;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[0][2], 3);
  EXPECT_EQ(mat2[0][3], 4);
  EXPECT_EQ(mat2[1][0], 5);
  EXPECT_EQ(mat2[1][1], 6);
  EXPECT_EQ(mat2[1][2], 7);
  EXPECT_EQ(mat2[1][3], 8);
  EXPECT_EQ(mat2[2][0], 9);
  EXPECT_EQ(mat2[2][1], 10);
  EXPECT_EQ(mat2[2][2], 11);
  EXPECT_EQ(mat2[2][3], 12);
  EXPECT_EQ(mat2[3][0], 13);
  EXPECT_EQ(mat2[3][1], 14);
  EXPECT_EQ(mat2[3][2], 15);
  EXPECT_EQ(mat2[3][3], 16);
}

TYPED_TEST(Mat4x4_Tests, PreDecrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = --mat;
  EXPECT_EQ(mat2[0][0], 0);
  EXPECT_EQ(mat2[0][1], 1);
  EXPECT_EQ(mat2[0][2], 2);
  EXPECT_EQ(mat2[0][3], 3);
  EXPECT_EQ(mat2[1][0], 4);
  EXPECT_EQ(mat2[1][1], 5);
  EXPECT_EQ(mat2[1][2], 6);
  EXPECT_EQ(mat2[1][3], 7);
  EXPECT_EQ(mat2[2][0], 8);
  EXPECT_EQ(mat2[2][1], 9);
  EXPECT_EQ(mat2[2][2], 10);
  EXPECT_EQ(mat2[2][3], 11);
  EXPECT_EQ(mat2[3][0], 12);
  EXPECT_EQ(mat2[3][1], 13);
  EXPECT_EQ(mat2[3][2], 14);
  EXPECT_EQ(mat2[3][3], 15);
}

TYPED_TEST(Mat4x4_Tests, PostDecrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = mat--;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[0][2], 3);
  EXPECT_EQ(mat2[0][3], 4);
  EXPECT_EQ(mat2[1][0], 5);
  EXPECT_EQ(mat2[1][1], 6);
  EXPECT_EQ(mat2[1][2], 7);
  EXPECT_EQ(mat2[1][3], 8);
  EXPECT_EQ(mat2[2][0], 9);
  EXPECT_EQ(mat2[2][1], 10);
  EXPECT_EQ(mat2[2][2], 11);
  EXPECT_EQ(mat2[2][3], 12);
  EXPECT_EQ(mat2[3][0], 13);
  EXPECT_EQ(mat2[3][1], 14);
  EXPECT_EQ(mat2[3][2], 15);
  EXPECT_EQ(mat2[3][3], 16);
}

TYPED_TEST(Mat4x4_Tests, Equals_True)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  EXPECT_TRUE(mat1 == mat2);
}

TYPED_TEST(Mat4x4_Tests, Equals_False)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17);
  EXPECT_FALSE(mat1 == mat2);
}

TYPED_TEST(Mat4x4_Tests, NotEquals_True)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17);
  EXPECT_TRUE(mat1 != mat2);
}

TYPED_TEST(Mat4x4_Tests, NotEquals_False)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  EXPECT_FALSE(mat1 != mat2);
}

TYPED_TEST(Mat4x4_Tests, Add_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = mat + 2;
  EXPECT_EQ(mat2[0][0], 3);
  EXPECT_EQ(mat2[0][1], 4);
  EXPECT_EQ(mat2[0][2], 5);
  EXPECT_EQ(mat2[0][3], 6);
  EXPECT_EQ(mat2[1][0], 7);
  EXPECT_EQ(mat2[1][1], 8);
  EXPECT_EQ(mat2[1][2], 9);
  EXPECT_EQ(mat2[1][3], 10);
  EXPECT_EQ(mat2[2][0], 11);
  EXPECT_EQ(mat2[2][1], 12);
  EXPECT_EQ(mat2[2][2], 13);
  EXPECT_EQ(mat2[2][3], 14);
  EXPECT_EQ(mat2[3][0], 15);
  EXPECT_EQ(mat2[3][1], 16);
  EXPECT_EQ(mat2[3][2], 17);
  EXPECT_EQ(mat2[3][3], 18);
}

TYPED_TEST(Mat4x4_Tests, Add_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  TypeParam mat3 = mat + mat2;
  EXPECT_EQ(mat3[0][0], 6);
  EXPECT_EQ(mat3[0][1], 8);
  EXPECT_EQ(mat3[0][2], 10);
  EXPECT_EQ(mat3[0][3], 12);
  EXPECT_EQ(mat3[1][0], 14);
  EXPECT_EQ(mat3[1][1], 16);
  EXPECT_EQ(mat3[1][2], 18);
  EXPECT_EQ(mat3[1][3], 20);
  EXPECT_EQ(mat3[2][0], 22);
  EXPECT_EQ(mat3[2][1], 24);
  EXPECT_EQ(mat3[2][2], 26);
  EXPECT_EQ(mat3[2][3], 28);
  EXPECT_EQ(mat3[3][0], 30);
  EXPECT_EQ(mat3[3][1], 32);
  EXPECT_EQ(mat3[3][2], 34);
  EXPECT_EQ(mat3[3][3], 36);
}

TYPED_TEST(Mat4x4_Tests, Sub_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2 = mat - 2;
  EXPECT_EQ(mat2[0][0], -1);
  EXPECT_EQ(mat2[0][1], 0);
  EXPECT_EQ(mat2[0][2], 1);
  EXPECT_EQ(mat2[0][3], 2);
  EXPECT_EQ(mat2[1][0], 3);
  EXPECT_EQ(mat2[1][1], 4);
  EXPECT_EQ(mat2[1][2], 5);
  EXPECT_EQ(mat2[1][3], 6);
  EXPECT_EQ(mat2[2][0], 7);
  EXPECT_EQ(mat2[2][1], 8);
  EXPECT_EQ(mat2[2][2], 9);
  EXPECT_EQ(mat2[2][3], 10);
  EXPECT_EQ(mat2[3][0], 11);
  EXPECT_EQ(mat2[3][1], 12);
  EXPECT_EQ(mat2[3][2], 13);
  EXPECT_EQ(mat2[3][3], 14);
}

TYPED_TEST(Mat4x4_Tests, Sub_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  TypeParam mat3 = mat - mat2;
  EXPECT_EQ(mat3[0][0], -4);
  EXPECT_EQ(mat3[0][1], -4);
  EXPECT_EQ(mat3[0][2], -4);
  EXPECT_EQ(mat3[0][3], -4);
  EXPECT_EQ(mat3[1][0], -4);
  EXPECT_EQ(mat3[1][1], -4);
  EXPECT_EQ(mat3[1][2], -4);
  EXPECT_EQ(mat3[1][3], -4);
  EXPECT_EQ(mat3[2][0], -4);
  EXPECT_EQ(mat3[2][1], -4);
  EXPECT_EQ(mat3[2][2], -4);
  EXPECT_EQ(mat3[2][3], -4);
  EXPECT_EQ(mat3[3][0], -4);
  EXPECT_EQ(mat3[3][1], -4);
  EXPECT_EQ(mat3[3][2], -4);
  EXPECT_EQ(mat3[3][3], -4);
}

TYPED_TEST(Mat4x4_Tests, Mult_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam result = mat * 2;
  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
  EXPECT_EQ(result[0][3], 8);
  EXPECT_EQ(result[1][0], 10);
  EXPECT_EQ(result[1][1], 12);
  EXPECT_EQ(result[1][2], 14);
  EXPECT_EQ(result[1][3], 16);
  EXPECT_EQ(result[2][0], 18);
  EXPECT_EQ(result[2][1], 20);
  EXPECT_EQ(result[2][2], 22);
  EXPECT_EQ(result[2][3], 24);
  EXPECT_EQ(result[3][0], 26);
  EXPECT_EQ(result[3][1], 28);
  EXPECT_EQ(result[3][2], 30);
  EXPECT_EQ(result[3][3], 32);
}

TYPED_TEST(Mat4x4_Tests, Mult_RowVector)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  typename TypeParam::row_type vec(1, 2, 3, 4);
  typename TypeParam::col_type result = mat * vec;
  EXPECT_EQ(result[0], 90);
  EXPECT_EQ(result[1], 100);
  EXPECT_EQ(result[2], 110);
  EXPECT_EQ(result[3], 120);
}

TYPED_TEST(Mat4x4_Tests, Mult_ColumnVector)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  typename TypeParam::col_type vec(1, 2, 3, 4);
  typename TypeParam::row_type result = vec * mat;
  EXPECT_EQ(result[0], 30);
  EXPECT_EQ(result[1], 70);
  EXPECT_EQ(result[2], 110);
  EXPECT_EQ(result[3], 150);
}

TYPED_TEST(Mat4x4_Tests, Mult_Mat)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  TypeParam result = mat1 * mat2;
  EXPECT_EQ(result[0][0], 202);
  EXPECT_EQ(result[0][1], 228);
  EXPECT_EQ(result[0][2], 254);
  EXPECT_EQ(result[0][3], 280);
  EXPECT_EQ(result[1][0], 314);
  EXPECT_EQ(result[1][1], 356);
  EXPECT_EQ(result[1][2], 398);
  EXPECT_EQ(result[1][3], 440);
  EXPECT_EQ(result[2][0], 426);
  EXPECT_EQ(result[2][1], 484);
  EXPECT_EQ(result[2][2], 542);
  EXPECT_EQ(result[2][3], 600);
  EXPECT_EQ(result[3][0], 538);
  EXPECT_EQ(result[3][1], 612);
  EXPECT_EQ(result[3][2], 686);
  EXPECT_EQ(result[3][3], 760);
}

TYPED_TEST(Mat4x4_Tests, Mult_Mat_Reversed)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  TypeParam result = mat2 * mat1;
  EXPECT_EQ(result[0][0], 130);
  EXPECT_EQ(result[0][1], 140);
  EXPECT_EQ(result[0][2], 150);
  EXPECT_EQ(result[0][3], 160);
  EXPECT_EQ(result[1][0], 306);
  EXPECT_EQ(result[1][1], 332);
  EXPECT_EQ(result[1][2], 358);
  EXPECT_EQ(result[1][3], 384);
  EXPECT_EQ(result[2][0], 482);
  EXPECT_EQ(result[2][1], 524);
  EXPECT_EQ(result[2][2], 566);
  EXPECT_EQ(result[2][3], 608);
  EXPECT_EQ(result[3][0], 658);
  EXPECT_EQ(result[3][1], 716);
  EXPECT_EQ(result[3][2], 774);
  EXPECT_EQ(result[3][3], 832);
}
} // namespace plex::tests