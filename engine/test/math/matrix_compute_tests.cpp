#include "genebits/engine/math/matrix_compute.h"

#include <gtest/gtest.h>

#include "genebits/engine/math/mat2x2.h"
#include "genebits/engine/math/mat3x3.h"
#include "genebits/engine/math/mat4x4.h"

namespace plex::tests
{
template<typename T>
class MatrixCompute_Tests : public testing::Test
{};

template<typename T>
class MatrixComputeF_Tests : public testing::Test
{};

using MatrixCompute_Tests_Types = testing::Types<int, float, double>;
using MatrixComputeF_Tests_Types = testing::Types<float, double>;

TYPED_TEST_SUITE(MatrixCompute_Tests, MatrixCompute_Tests_Types);
TYPED_TEST_SUITE(MatrixComputeF_Tests, MatrixComputeF_Tests_Types);

TYPED_TEST(MatrixCompute_Tests, Trace_Mat2x2)
{
  using T = TypeParam;

  Mat<T, 2, 2> mat { { 1, 2 }, { 3, 4 } };
  T trace = Trace(mat);

  EXPECT_EQ(trace, T(5));
}

TYPED_TEST(MatrixCompute_Tests, Trace_Mat3x3)
{
  using T = TypeParam;

  Mat<T, 3, 3> mat { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  T trace = Trace(mat);

  EXPECT_EQ(trace, T(15));
}

TYPED_TEST(MatrixCompute_Tests, Trace_Mat4x4)
{
  using T = TypeParam;

  Mat<T, 4, 4> mat { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };
  T trace = Trace(mat);

  EXPECT_EQ(trace, T(34));
}

TYPED_TEST(MatrixCompute_Tests, Transpose_Mat2x2)
{
  using T = TypeParam;

  Mat<T, 2, 2> mat { { 1, 2 }, { 3, 4 } };
  Mat<T, 2, 2> expected { { 1, 3 }, { 2, 4 } };

  Mat<T, 2, 2> transpose = Transpose(mat);

  EXPECT_EQ(transpose, expected);
}

TYPED_TEST(MatrixCompute_Tests, Transpose_Mat3x3)
{
  using T = TypeParam;

  Mat<T, 3, 3> mat { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  Mat<T, 3, 3> expected { { 1, 4, 7 }, { 2, 5, 8 }, { 3, 6, 9 } };

  Mat<T, 3, 3> transpose = Transpose(mat);

  EXPECT_EQ(transpose, expected);
}

TYPED_TEST(MatrixCompute_Tests, Transpose_Mat4x4)
{
  using T = TypeParam;

  Mat<T, 4, 4> mat { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };
  Mat<T, 4, 4> expected { { 1, 5, 9, 13 }, { 2, 6, 10, 14 }, { 3, 7, 11, 15 }, { 4, 8, 12, 16 } };

  Mat<T, 4, 4> transpose = Transpose(mat);

  EXPECT_EQ(transpose, expected);
}

TYPED_TEST(MatrixCompute_Tests, Determinant_Mat2x2)
{
  using T = TypeParam;

  Mat<T, 2, 2> mat { { 1, 2 }, { 3, 4 } };
  T det = Determinant(mat);

  EXPECT_EQ(det, T(-2));
}

TYPED_TEST(MatrixCompute_Tests, Determinant_Mat3x3)
{
  using T = TypeParam;

  Mat<T, 3, 3> mat { { 2, 3, 5 }, { 7, 11, 13 }, { 17, 19, 23 } };
  T det = Determinant(mat);

  EXPECT_EQ(det, T(-78));
}

TYPED_TEST(MatrixCompute_Tests, Determinant_Mat4x4)
{
  using T = TypeParam;

  Mat<T, 4, 4> mat { { 2, 3, 5, 7 }, { 11, 13, 17, 19 }, { 23, 29, 31, 37 }, { 41, 43, 47, 53 } };
  T det = Determinant(mat);

  EXPECT_EQ(det, T(880));
}

TYPED_TEST(MatrixComputeF_Tests, Inverse_Mat2x2)
{
  using T = TypeParam;

  Mat<T, 2, 2> mat { { 1, 2 }, { 3, 4 } };
  Mat<T, 2, 2> expected { { -2, 1 }, { 1.5, -0.5 } };

  Mat<T, 2, 2> inv = Inverse(mat);

  EXPECT_EQ(inv, expected);
}

TYPED_TEST(MatrixComputeF_Tests, Inverse_Mat3x3)
{
  using T = TypeParam;

  Mat<T, 3, 3> mat { { 2, 3, 5 }, { 7, 11, 13 }, { 17, 19, 23 } };

  Mat<T, 3, 3> inv = Inverse(mat);

  EXPECT_NEAR(inv[0][0], -1.0 / 13.0, 0.00001);
  EXPECT_NEAR(inv[0][1], -1.0 / 3.0, 0.00001);
  EXPECT_NEAR(inv[0][2], 8.0 / 39.0, 0.00001);
  EXPECT_NEAR(inv[1][0], -10.0 / 13.0, 0.00001);
  EXPECT_NEAR(inv[1][1], 1.0 / 2.0, 0.00001);
  EXPECT_NEAR(inv[1][2], -3.0 / 26.0, 0.00001);
  EXPECT_NEAR(inv[2][0], 9.0 / 13.0, 0.00001);
  EXPECT_NEAR(inv[2][1], -1.0 / 6.0, 0.00001);
  EXPECT_NEAR(inv[2][2], -1.0 / 78.0, 0.00001);
}

TYPED_TEST(MatrixComputeF_Tests, Inverse_Mat4x4)
{
  using T = TypeParam;

  Mat<T, 4, 4> mat { { 2, 3, 5, 7 }, { 11, 13, 17, 19 }, { 23, 29, 31, 37 }, { 41, 43, 47, 53 } };

  Mat<T, 4, 4> inv = Inverse(mat);

  EXPECT_NEAR(inv[0][0], 3.0 / 11.0, 0.00001);
  EXPECT_NEAR(inv[0][1], -12.0 / 55.0, 0.00001);
  EXPECT_NEAR(inv[0][2], -1.0 / 5.0, 0.00001);
  EXPECT_NEAR(inv[0][3], 2.0 / 11.0, 0.00001);
  EXPECT_NEAR(inv[1][0], -5.0 / 11.0, 0.00001);
  EXPECT_NEAR(inv[1][1], -2.0 / 55.0, 0.00001);
  EXPECT_NEAR(inv[1][2], 3.0 / 10.0, 0.00001);
  EXPECT_NEAR(inv[1][3], -3.0 / 22.0, 0.00001);
  EXPECT_NEAR(inv[2][0], -13.0 / 22.0, 0.00001);
  EXPECT_NEAR(inv[2][1], 307.0 / 440.0, 0.00001);
  EXPECT_NEAR(inv[2][2], -1.0 / 10.0, 0.00001);
  EXPECT_NEAR(inv[2][3], -9.0 / 88.0, 0.00001);
  EXPECT_NEAR(inv[3][0], 15.0 / 22.0, 0.00001);
  EXPECT_NEAR(inv[3][1], -37.0 / 88.0, 0.00001);
  EXPECT_NEAR(inv[3][2], 0, 0.00001);
  EXPECT_NEAR(inv[3][3], 7.0 / 88.0, 0.00001);
}

} // namespace plex::tests