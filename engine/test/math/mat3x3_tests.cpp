#include "genebits/engine/math/mat3x3.h"

#include <gtest/gtest.h>

namespace plex::tests
{

static_assert(std::is_trivially_copy_constructible_v<float3x3>, "Mat3x3 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float3x3>, "Mat3x3 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float3x3>, "Mat3x3 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float3x3>, "Mat3x3 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float3x3>, "Mat3x3 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float3x3>, "Mat3x3 Must be trivially copyable");

template<typename T>
class Mat3x3_Tests : public testing::Test
{};

using Mat3x3_Test_Types = testing::Types<int3x3, float3x3, double3x3>;

TYPED_TEST_SUITE(Mat3x3_Tests, Mat3x3_Test_Types);

TYPED_TEST(Mat3x3_Tests, Constructor_Default)
{
  TypeParam mat;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 1);
  EXPECT_EQ(mat[1][2], 0);
  EXPECT_EQ(mat[2][0], 0);
  EXPECT_EQ(mat[2][1], 0);
  EXPECT_EQ(mat[2][2], 1);
}

TYPED_TEST(Mat3x3_Tests, Constructor_Scalar)
{
  TypeParam mat(2);
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 2);
  EXPECT_EQ(mat[1][2], 0);
  EXPECT_EQ(mat[2][0], 0);
  EXPECT_EQ(mat[2][1], 0);
  EXPECT_EQ(mat[2][2], 2);
}

TYPED_TEST(Mat3x3_Tests, Constructor_Values)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);
  EXPECT_EQ(mat[2][0], 7);
  EXPECT_EQ(mat[2][1], 8);
  EXPECT_EQ(mat[2][2], 9);
}

TYPED_TEST(Mat3x3_Tests, Constructor_Columns)
{
  TypeParam mat({ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 });
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);
  EXPECT_EQ(mat[2][0], 7);
  EXPECT_EQ(mat[2][1], 8);
  EXPECT_EQ(mat[2][2], 9);
}

TYPED_TEST(Mat3x3_Tests, Constructor_CopyFromOtherType)
{
  TypeParam mat(5);

  if constexpr (std::is_same_v<TypeParam, float3x3>)
  {
    int3x3 mat2(mat);
    EXPECT_EQ(mat[0][0], 5);
    EXPECT_EQ(mat[0][1], 0);
    EXPECT_EQ(mat[0][2], 0);
    EXPECT_EQ(mat[1][0], 0);
    EXPECT_EQ(mat[1][1], 5);
    EXPECT_EQ(mat[1][2], 0);
    EXPECT_EQ(mat[2][0], 0);
    EXPECT_EQ(mat[2][1], 0);
    EXPECT_EQ(mat[2][2], 5);
  }
  else
  {
    float3x3 mat2(mat);
    EXPECT_EQ(mat[0][0], 5);
    EXPECT_EQ(mat[0][1], 0);
    EXPECT_EQ(mat[0][2], 0);
    EXPECT_EQ(mat[1][0], 0);
    EXPECT_EQ(mat[1][1], 5);
    EXPECT_EQ(mat[1][2], 0);
    EXPECT_EQ(mat[2][0], 0);
    EXPECT_EQ(mat[2][1], 0);
    EXPECT_EQ(mat[2][2], 5);
  }
}

TYPED_TEST(Mat3x3_Tests, AddAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  mat += 2;
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[0][2], 5);
  EXPECT_EQ(mat[1][0], 6);
  EXPECT_EQ(mat[1][1], 7);
  EXPECT_EQ(mat[1][2], 8);
  EXPECT_EQ(mat[2][0], 9);
  EXPECT_EQ(mat[2][1], 10);
  EXPECT_EQ(mat[2][2], 11);
}

TYPED_TEST(Mat3x3_Tests, AddAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  mat += TypeParam(2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 5);
  EXPECT_EQ(mat[0][2], 7);
  EXPECT_EQ(mat[1][0], 9);
  EXPECT_EQ(mat[1][1], 11);
  EXPECT_EQ(mat[1][2], 13);
  EXPECT_EQ(mat[2][0], 15);
  EXPECT_EQ(mat[2][1], 17);
  EXPECT_EQ(mat[2][2], 19);
}

TYPED_TEST(Mat3x3_Tests, SubAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  mat -= 2;
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 1);
  EXPECT_EQ(mat[1][0], 2);
  EXPECT_EQ(mat[1][1], 3);
  EXPECT_EQ(mat[1][2], 4);
  EXPECT_EQ(mat[2][0], 5);
  EXPECT_EQ(mat[2][1], 6);
  EXPECT_EQ(mat[2][2], 7);
}

TYPED_TEST(Mat3x3_Tests, SubAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  mat -= TypeParam(2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], -1);
  EXPECT_EQ(mat[0][2], -1);
  EXPECT_EQ(mat[1][0], -1);
  EXPECT_EQ(mat[1][1], -1);
  EXPECT_EQ(mat[1][2], -1);
  EXPECT_EQ(mat[2][0], -1);
  EXPECT_EQ(mat[2][1], -1);
  EXPECT_EQ(mat[2][2], -1);
}

TYPED_TEST(Mat3x3_Tests, MulAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  mat *= 2;
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[0][2], 6);
  EXPECT_EQ(mat[1][0], 8);
  EXPECT_EQ(mat[1][1], 10);
  EXPECT_EQ(mat[1][2], 12);
  EXPECT_EQ(mat[2][0], 14);
  EXPECT_EQ(mat[2][1], 16);
  EXPECT_EQ(mat[2][2], 18);
}

TYPED_TEST(Mat3x3_Tests, DivAssign_Scalar)
{
  TypeParam mat(2, 4, 6, 8, 10, 12, 14, 16, 18);
  mat /= 2;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);
  EXPECT_EQ(mat[2][0], 7);
  EXPECT_EQ(mat[2][1], 8);
  EXPECT_EQ(mat[2][2], 9);
}

TYPED_TEST(Mat3x3_Tests, PreIncrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = ++mat;
  EXPECT_EQ(mat2[0][0], 2);
  EXPECT_EQ(mat2[0][1], 3);
  EXPECT_EQ(mat2[0][2], 4);
  EXPECT_EQ(mat2[1][0], 5);
  EXPECT_EQ(mat2[1][1], 6);
  EXPECT_EQ(mat2[1][2], 7);
  EXPECT_EQ(mat2[2][0], 8);
  EXPECT_EQ(mat2[2][1], 9);
  EXPECT_EQ(mat2[2][2], 10);
}

TYPED_TEST(Mat3x3_Tests, PostIncrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = mat++;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[0][2], 3);
  EXPECT_EQ(mat2[1][0], 4);
  EXPECT_EQ(mat2[1][1], 5);
  EXPECT_EQ(mat2[1][2], 6);
  EXPECT_EQ(mat2[2][0], 7);
  EXPECT_EQ(mat2[2][1], 8);
  EXPECT_EQ(mat2[2][2], 9);
}

TYPED_TEST(Mat3x3_Tests, PreDecrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = --mat;
  EXPECT_EQ(mat2[0][0], 0);
  EXPECT_EQ(mat2[0][1], 1);
  EXPECT_EQ(mat2[0][2], 2);
  EXPECT_EQ(mat2[1][0], 3);
  EXPECT_EQ(mat2[1][1], 4);
  EXPECT_EQ(mat2[1][2], 5);
  EXPECT_EQ(mat2[2][0], 6);
  EXPECT_EQ(mat2[2][1], 7);
  EXPECT_EQ(mat2[2][2], 8);
}

TYPED_TEST(Mat3x3_Tests, PostDecrement)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = mat--;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[0][2], 3);
  EXPECT_EQ(mat2[1][0], 4);
  EXPECT_EQ(mat2[1][1], 5);
  EXPECT_EQ(mat2[1][2], 6);
  EXPECT_EQ(mat2[2][0], 7);
  EXPECT_EQ(mat2[2][1], 8);
  EXPECT_EQ(mat2[2][2], 9);
}

TYPED_TEST(Mat3x3_Tests, Equals_True)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_TRUE(mat1 == mat2);
}

TYPED_TEST(Mat3x3_Tests, Equals_False)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 10);
  EXPECT_FALSE(mat1 == mat2);
}

TYPED_TEST(Mat3x3_Tests, NotEquals_True)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 9, 9);
  EXPECT_TRUE(mat1 != mat2);
}

TYPED_TEST(Mat3x3_Tests, NotEquals_False)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_FALSE(mat1 != mat2);
}

TYPED_TEST(Mat3x3_Tests, Add_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = mat + 2;
  EXPECT_EQ(mat2[0][0], 3);
  EXPECT_EQ(mat2[0][1], 4);
  EXPECT_EQ(mat2[0][2], 5);
  EXPECT_EQ(mat2[1][0], 6);
  EXPECT_EQ(mat2[1][1], 7);
  EXPECT_EQ(mat2[1][2], 8);
  EXPECT_EQ(mat2[2][0], 9);
  EXPECT_EQ(mat2[2][1], 10);
  EXPECT_EQ(mat2[2][2], 11);
}

TYPED_TEST(Mat3x3_Tests, Add_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13);
  TypeParam mat3 = mat + mat2;
  EXPECT_EQ(mat3[0][0], 6);
  EXPECT_EQ(mat3[0][1], 8);
  EXPECT_EQ(mat3[0][2], 10);
  EXPECT_EQ(mat3[1][0], 12);
  EXPECT_EQ(mat3[1][1], 14);
  EXPECT_EQ(mat3[1][2], 16);
  EXPECT_EQ(mat3[2][0], 18);
  EXPECT_EQ(mat3[2][1], 20);
  EXPECT_EQ(mat3[2][2], 22);
}

TYPED_TEST(Mat3x3_Tests, Sub_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2 = mat - 2;
  EXPECT_EQ(mat2[0][0], -1);
  EXPECT_EQ(mat2[0][1], 0);
  EXPECT_EQ(mat2[0][2], 1);
  EXPECT_EQ(mat2[1][0], 2);
  EXPECT_EQ(mat2[1][1], 3);
  EXPECT_EQ(mat2[1][2], 4);
  EXPECT_EQ(mat2[2][0], 5);
  EXPECT_EQ(mat2[2][1], 6);
  EXPECT_EQ(mat2[2][2], 7);
}

TYPED_TEST(Mat3x3_Tests, Sub_Mat)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13);
  TypeParam mat3 = mat - mat2;
  EXPECT_EQ(mat3[0][0], -4);
  EXPECT_EQ(mat3[0][1], -4);
  EXPECT_EQ(mat3[0][2], -4);
  EXPECT_EQ(mat3[1][0], -4);
  EXPECT_EQ(mat3[1][1], -4);
  EXPECT_EQ(mat3[1][2], -4);
  EXPECT_EQ(mat3[2][0], -4);
  EXPECT_EQ(mat3[2][1], -4);
  EXPECT_EQ(mat3[2][2], -4);
}

TYPED_TEST(Mat3x3_Tests, Mult_Scalar)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam result = mat * 2;
  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[1][2], 12);
  EXPECT_EQ(result[2][0], 14);
  EXPECT_EQ(result[2][1], 16);
}

TYPED_TEST(Mat3x3_Tests, Mult_RowVector)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  typename TypeParam::row_type vec(1, 2, 3);
  typename TypeParam::col_type result = mat * vec;
  EXPECT_EQ(result[0], 30);
  EXPECT_EQ(result[1], 36);
  EXPECT_EQ(result[2], 42);
}

TYPED_TEST(Mat3x3_Tests, Mult_ColumnVector)
{
  TypeParam mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
  typename TypeParam::col_type vec(1, 2, 3);
  typename TypeParam::row_type result = vec * mat;
  EXPECT_EQ(result[0], 14);
  EXPECT_EQ(result[1], 32);
  EXPECT_EQ(result[2], 50);
}

TYPED_TEST(Mat3x3_Tests, Mult_Mat)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13);
  TypeParam result = mat1 * mat2;
  EXPECT_EQ(result[0][0], 78);
  EXPECT_EQ(result[0][1], 96);
  EXPECT_EQ(result[0][2], 114);
  EXPECT_EQ(result[1][0], 114);
  EXPECT_EQ(result[1][1], 141);
  EXPECT_EQ(result[1][2], 168);
  EXPECT_EQ(result[2][0], 150);
  EXPECT_EQ(result[2][1], 186);
  EXPECT_EQ(result[2][2], 222);
}

TYPED_TEST(Mat3x3_Tests, Mult_Mat_Reversed)
{
  TypeParam mat1(1, 2, 3, 4, 5, 6, 7, 8, 9);
  TypeParam mat2(5, 6, 7, 8, 9, 10, 11, 12, 13);
  TypeParam result = mat2 * mat1;
  EXPECT_EQ(result[0][0], 54);
  EXPECT_EQ(result[0][1], 60);
  EXPECT_EQ(result[0][2], 66);
  EXPECT_EQ(result[1][0], 126);
  EXPECT_EQ(result[1][1], 141);
  EXPECT_EQ(result[1][2], 156);
  EXPECT_EQ(result[2][0], 198);
  EXPECT_EQ(result[2][1], 222);
  EXPECT_EQ(result[2][2], 246);
}

} // namespace plex::tests