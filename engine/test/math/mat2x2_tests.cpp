#include "genebits/engine/math/mat2x2.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{

static_assert(std::is_trivially_copy_constructible_v<float2x2>, "Mat2x2 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float2x2>, "Mat2x2 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float2x2>, "Mat2x2 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float2x2>, "Mat2x2 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float2x2>, "Mat2x2 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float2x2>, "Mat2x2 Must be trivially copyable");

template<typename T>
class Mat2x2_Tests : public testing::Test
{};

using Mat2x2_Test_Types = testing::Types<int2x2, float2x2, double2x2>;

TYPED_TEST_SUITE(Mat2x2_Tests, Mat2x2_Test_Types);

TYPED_TEST(Mat2x2_Tests, Constructor_Default)
{
  TypeParam mat;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 1);
}

TYPED_TEST(Mat2x2_Tests, Constructor_Scalar)
{
  TypeParam mat(2);
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 2);
}

TYPED_TEST(Mat2x2_Tests, Constructor_Values)
{
  TypeParam mat(1, 2, 3, 4);
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[1][0], 3);
  EXPECT_EQ(mat[1][1], 4);
}

TYPED_TEST(Mat2x2_Tests, Constructor_Columns)
{
  TypeParam mat({ 1, 2 }, { 3, 4 });
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[1][0], 3);
  EXPECT_EQ(mat[1][1], 4);
}

TYPED_TEST(Mat2x2_Tests, Constructor_CopyFromOtherType)
{
  TypeParam mat(5);

  if constexpr (std::is_same_v<TypeParam, float2x2>)
  {
    int2x2 mat2(mat);
    EXPECT_EQ(mat2[0][0], 5);
    EXPECT_EQ(mat2[0][1], 0);
    EXPECT_EQ(mat2[1][0], 0);
    EXPECT_EQ(mat2[1][1], 5);
  }
  else
  {
    float2x2 mat2(mat);
    EXPECT_EQ(mat2[0][0], 5);
    EXPECT_EQ(mat2[0][1], 0);
    EXPECT_EQ(mat2[1][0], 0);
    EXPECT_EQ(mat2[1][1], 5);
  }
}

TYPED_TEST(Mat2x2_Tests, AddAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  mat += 2;
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[1][0], 5);
  EXPECT_EQ(mat[1][1], 6);
}

TYPED_TEST(Mat2x2_Tests, AddAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4);
  mat += TypeParam(2, 3, 4, 5);
  EXPECT_EQ(mat[0][0], 3);
  EXPECT_EQ(mat[0][1], 5);
  EXPECT_EQ(mat[1][0], 7);
  EXPECT_EQ(mat[1][1], 9);
}

TYPED_TEST(Mat2x2_Tests, SubAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  mat -= 2;
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[1][0], 1);
  EXPECT_EQ(mat[1][1], 2);
}

TYPED_TEST(Mat2x2_Tests, SubAssign_Mat)
{
  TypeParam mat(1, 2, 3, 4);
  mat -= TypeParam(2, 3, 4, 5);
  EXPECT_EQ(mat[0][0], -1);
  EXPECT_EQ(mat[0][1], -1);
  EXPECT_EQ(mat[1][0], -1);
  EXPECT_EQ(mat[1][1], -1);
}

TYPED_TEST(Mat2x2_Tests, MulAssign_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  mat *= 2;
  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[1][0], 6);
  EXPECT_EQ(mat[1][1], 8);
}

TYPED_TEST(Mat2x2_Tests, DivAssign_Scalar)
{
  TypeParam mat(2, 4, 6, 8);
  mat /= 2;
  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[1][0], 3);
  EXPECT_EQ(mat[1][1], 4);
}

TYPED_TEST(Mat2x2_Tests, PreIncrement)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = ++mat;
  EXPECT_EQ(mat2[0][0], 2);
  EXPECT_EQ(mat2[0][1], 3);
  EXPECT_EQ(mat2[1][0], 4);
  EXPECT_EQ(mat2[1][1], 5);
}

TYPED_TEST(Mat2x2_Tests, PostIncrement)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = mat++;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[1][0], 3);
  EXPECT_EQ(mat2[1][1], 4);
}

TYPED_TEST(Mat2x2_Tests, PreDecrement)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = --mat;
  EXPECT_EQ(mat2[0][0], 0);
  EXPECT_EQ(mat2[0][1], 1);
  EXPECT_EQ(mat2[1][0], 2);
  EXPECT_EQ(mat2[1][1], 3);
}

TYPED_TEST(Mat2x2_Tests, PostDecrement)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = mat--;
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[0][1], 2);
  EXPECT_EQ(mat2[1][0], 3);
  EXPECT_EQ(mat2[1][1], 4);
}

TYPED_TEST(Mat2x2_Tests, Equals_True)
{
  TypeParam mat1(1, 2, 3, 4);
  TypeParam mat2(1, 2, 3, 4);
  EXPECT_TRUE(mat1 == mat2);
}

TYPED_TEST(Mat2x2_Tests, Equals_False)
{
  TypeParam mat1(1, 2, 3, 4);
  TypeParam mat2(1, 2, 3, 5);
  EXPECT_FALSE(mat1 == mat2);
}

TYPED_TEST(Mat2x2_Tests, NotEquals_True)
{
  TypeParam mat1(1, 2, 3, 4);
  TypeParam mat2(1, 3, 3, 4);
  EXPECT_TRUE(mat1 != mat2);
}

TYPED_TEST(Mat2x2_Tests, NotEquals_False)
{
  TypeParam mat1(1, 2, 3, 4);
  TypeParam mat2(1, 2, 3, 4);
  EXPECT_FALSE(mat1 != mat2);
}

TYPED_TEST(Mat2x2_Tests, Add_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = mat + 2;
  EXPECT_EQ(mat2[0][0], 3);
  EXPECT_EQ(mat2[0][1], 4);
  EXPECT_EQ(mat2[1][0], 5);
  EXPECT_EQ(mat2[1][1], 6);
}

TYPED_TEST(Mat2x2_Tests, Add_Mat)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2(5, 6, 7, 8);
  TypeParam mat3 = mat + mat2;
  EXPECT_EQ(mat3[0][0], 6);
  EXPECT_EQ(mat3[0][1], 8);
  EXPECT_EQ(mat3[1][0], 10);
  EXPECT_EQ(mat3[1][1], 12);
}

TYPED_TEST(Mat2x2_Tests, Sub_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2 = mat - 2;
  EXPECT_EQ(mat2[0][0], -1);
  EXPECT_EQ(mat2[0][1], 0);
  EXPECT_EQ(mat2[1][0], 1);
  EXPECT_EQ(mat2[1][1], 2);
}

TYPED_TEST(Mat2x2_Tests, Sub_Mat)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam mat2(5, 6, 7, 8);
  TypeParam mat3 = mat - mat2;
  EXPECT_EQ(mat3[0][0], -4);
  EXPECT_EQ(mat3[0][1], -4);
  EXPECT_EQ(mat3[1][0], -4);
  EXPECT_EQ(mat3[1][1], -4);
}

TYPED_TEST(Mat2x2_Tests, Mult_Scalar)
{
  TypeParam mat(1, 2, 3, 4);
  TypeParam result = mat * 2;
  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[1][1], 8);
}

TYPED_TEST(Mat2x2_Tests, Mult_RowVector)
{
  TypeParam mat(1, 2, 3, 4);
  typename TypeParam::row_type vec(1, 2);
  typename TypeParam::col_type result = mat * vec;
  EXPECT_EQ(result[0], 7);
  EXPECT_EQ(result[1], 10);
}

TYPED_TEST(Mat2x2_Tests, Mult_ColumnVector)
{
  TypeParam mat(1, 2, 3, 4);
  typename TypeParam::col_type vec(1, 2);
  typename TypeParam::row_type result = vec * mat;
  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 11);
}

TYPED_TEST(Mat2x2_Tests, Mult_Mat)
{
  TypeParam mat1(1, 2, 3, 4);
  TypeParam mat2(5, 6, 7, 8);
  TypeParam result = mat1 * mat2;
  EXPECT_EQ(result[0][0], 23);
  EXPECT_EQ(result[0][1], 34);
  EXPECT_EQ(result[1][0], 31);
  EXPECT_EQ(result[1][1], 46);
}
} // namespace genebits::engine::tests