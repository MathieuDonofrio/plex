#ifndef GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H
#define GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H

#include "genebits/engine/debug/assertion.h"

namespace plex
{
template<typename T, size_t Rows, size_t Cols>
struct Mat;

// Trace

///
/// Computes the trace of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the trace of.
///
/// @return The trace of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Trace(const Mat<T, 2, 2>& mat) noexcept
{
  return mat[0][0] + mat[1][1];
}

///
/// Computes the trace of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the trace of.
///
/// @return The trace of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Trace(const Mat<T, 3, 3>& mat) noexcept
{
  return mat[0][0] + mat[1][1] + mat[2][2];
}

///
/// Computes the trace of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the trace of.
///
/// @return The trace of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Trace(const Mat<T, 4, 4>& mat) noexcept
{
  return mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3];
}

///
/// Computes the transpose of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to transpose.
///
/// @return The transpose of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Transpose(const Mat<T, 2, 2>& mat) noexcept
{
  return Mat<T, 2, 2>(mat[0][0], mat[1][0], mat[0][1], mat[1][1]);
}

///
/// Computes the transpose of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to transpose.
///
/// @return The transpose of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 3, 3> Transpose(const Mat<T, 3, 3>& mat) noexcept
{
  return Mat<T, 3, 3>(
    mat[0][0], mat[1][0], mat[2][0], mat[0][1], mat[1][1], mat[2][1], mat[0][2], mat[1][2], mat[2][2]);
}

///
/// Computes the transpose of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to transpose.
///
/// @return The transpose of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 4, 4> Transpose(const Mat<T, 4, 4>& mat) noexcept
{
  return Mat<T, 4, 4>(mat[0][0],
    mat[1][0],
    mat[2][0],
    mat[3][0],
    mat[0][1],
    mat[1][1],
    mat[2][1],
    mat[3][1],
    mat[0][2],
    mat[1][2],
    mat[2][2],
    mat[3][2],
    mat[0][3],
    mat[1][3],
    mat[2][3],
    mat[3][3]);
}

// Determinant

///
/// Computes the determinant of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the determinant of.
///
/// @return Determinant of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Determinant(const Mat<T, 2, 2>& mat) noexcept
{
  return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

///
/// Computes the determinant of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the determinant of.
///
/// @return Determinant of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Determinant(const Mat<T, 3, 3>& mat) noexcept
{
  const T minor_00 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
  const T minor_10 = mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2];
  const T minor_20 = mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2];

  return mat[0][0] * minor_00 - mat[1][0] * minor_10 + mat[2][0] * minor_20;
}

///
/// Computes the determinant of a matrix.
///
/// @tparam T The type of the matrix.
///
/// @param[in] mat The matrix to compute the determinant of.
///
/// @return Determinant of the matrix.
///
template<typename T>
[[nodiscard]] constexpr T Determinant(const Mat<T, 4, 4>& mat) noexcept
{
  const T sub_factor_01 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
  const T sub_factor_02 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
  const T sub_factor_03 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
  const T sub_factor_12 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
  const T sub_factor_13 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
  const T sub_factor_23 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];

  const T minor_00 = mat[1][1] * sub_factor_23 - mat[1][2] * sub_factor_13 + mat[1][3] * sub_factor_12;
  const T minor_01 = mat[1][0] * sub_factor_23 - mat[1][2] * sub_factor_03 + mat[1][3] * sub_factor_02;
  const T minor_02 = mat[1][0] * sub_factor_13 - mat[1][1] * sub_factor_03 + mat[1][3] * sub_factor_01;
  const T minor_03 = mat[1][0] * sub_factor_12 - mat[1][1] * sub_factor_02 + mat[1][2] * sub_factor_01;

  return mat[0][0] * minor_00 - mat[0][1] * minor_01 + mat[0][2] * minor_02 - mat[0][3] * minor_03;
}

// Inverse

///
/// Computes the inverse of a matrix.
///
/// @warning If the matrix is not invertible, then the behavior of this function is undefined.
///
/// @tparam T The type of the elements of the matrix.
///
/// @param[in] mat The matrix to compute the inverse of.
///
/// @return The inverse of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Inverse(const Mat<T, 2, 2>& mat) noexcept
{
  // Determinant and adjoint method

  const T det = Determinant(mat);

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 2, 2>( //
    +mat[1][1] * inv_det,
    -mat[0][1] * inv_det,
    -mat[1][0] * inv_det,
    +mat[0][0] * inv_det);
}

///
/// Computes the inverse of a matrix.
///
/// @warning If the matrix is not invertible, then the behavior of this function is undefined.
///
/// @tparam T The type of the elements of the matrix.
///
/// @param[in] mat The matrix to compute the inverse of.
///
/// @return The inverse of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 3, 3> Inverse(const Mat<T, 3, 3>& mat) noexcept
{
  // Determinant and adjoint method

  const T minor_00 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
  const T minor_01 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];
  const T minor_02 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];
  const T minor_10 = mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2];
  const T minor_11 = mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2];
  const T minor_12 = mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1];
  const T minor_20 = mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2];
  const T minor_21 = mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2];
  const T minor_22 = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

  const T det = mat[0][0] * minor_00 - mat[1][0] * minor_10 + mat[2][0] * minor_20;

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 3, 3>( //
    +minor_00 * inv_det,
    -minor_10 * inv_det,
    +minor_20 * inv_det,
    -minor_01 * inv_det,
    +minor_11 * inv_det,
    -minor_21 * inv_det,
    +minor_02 * inv_det,
    -minor_12 * inv_det,
    +minor_22 * inv_det);
}

///
/// Computes the inverse of a matrix.
///
/// @warning If the matrix is not invertible, then the behavior of this function is undefined.
///
/// @tparam T The type of the elements of the matrix.
///
/// @param[in] mat The matrix to compute the inverse of.
///
/// @return The inverse of the matrix.
///
template<typename T>
[[nodiscard]] constexpr Mat<T, 4, 4> Inverse(const Mat<T, 4, 4>& mat) noexcept
{
  const T sub_factor_22_33 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
  const T sub_factor_12_33 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
  const T sub_factor_12_23 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];
  const T sub_factor_21_33 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
  const T sub_factor_11_33 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
  const T sub_factor_11_23 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];
  const T sub_factor_21_32 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
  const T sub_factor_11_32 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
  const T sub_factor_11_22 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
  const T sub_factor_20_33 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
  const T sub_factor_10_33 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
  const T sub_factor_10_23 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];
  const T sub_factor_20_32 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
  const T sub_factor_10_32 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
  const T sub_factor_10_22 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];
  const T sub_factor_20_31 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
  const T sub_factor_10_31 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
  const T sub_factor_10_21 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

  const T minor_00 = mat[1][1] * sub_factor_22_33 - mat[2][1] * sub_factor_12_33 + mat[3][1] * sub_factor_12_23;
  const T minor_01 = mat[1][0] * sub_factor_22_33 - mat[2][0] * sub_factor_12_33 + mat[3][0] * sub_factor_12_23;
  const T minor_02 = mat[1][0] * sub_factor_21_33 - mat[2][0] * sub_factor_11_33 + mat[3][0] * sub_factor_11_23;
  const T minor_03 = mat[1][0] * sub_factor_21_32 - mat[2][0] * sub_factor_11_32 + mat[3][0] * sub_factor_11_22;
  const T minor_10 = mat[0][1] * sub_factor_22_33 - mat[0][2] * sub_factor_21_33 + mat[0][3] * sub_factor_21_32;
  const T minor_11 = mat[0][0] * sub_factor_22_33 - mat[0][2] * sub_factor_20_33 + mat[0][3] * sub_factor_20_32;
  const T minor_12 = mat[0][0] * sub_factor_21_33 - mat[0][1] * sub_factor_20_33 + mat[0][3] * sub_factor_20_31;
  const T minor_13 = mat[0][0] * sub_factor_21_32 - mat[0][1] * sub_factor_20_32 + mat[0][2] * sub_factor_20_31;
  const T minor_20 = mat[0][1] * sub_factor_12_33 - mat[0][2] * sub_factor_11_33 + mat[0][3] * sub_factor_11_32;
  const T minor_21 = mat[0][0] * sub_factor_12_33 - mat[0][2] * sub_factor_10_33 + mat[0][3] * sub_factor_10_32;
  const T minor_22 = mat[0][0] * sub_factor_11_33 - mat[0][1] * sub_factor_10_33 + mat[0][3] * sub_factor_10_31;
  const T minor_23 = mat[0][0] * sub_factor_11_32 - mat[0][1] * sub_factor_10_32 + mat[0][2] * sub_factor_10_31;
  const T minor_30 = mat[0][1] * sub_factor_12_23 - mat[0][2] * sub_factor_11_23 + mat[0][3] * sub_factor_11_22;
  const T minor_31 = mat[0][0] * sub_factor_12_23 - mat[0][2] * sub_factor_10_23 + mat[0][3] * sub_factor_10_22;
  const T minor_32 = mat[0][0] * sub_factor_11_23 - mat[0][1] * sub_factor_10_23 + mat[0][3] * sub_factor_10_21;
  const T minor_33 = mat[0][0] * sub_factor_11_22 - mat[0][1] * sub_factor_10_22 + mat[0][2] * sub_factor_10_21;

  const T det = mat[0][0] * minor_00 - mat[0][1] * minor_01 + mat[0][2] * minor_02 - mat[0][3] * minor_03;

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 4, 4>( //
    +minor_00 * inv_det,
    -minor_10 * inv_det,
    +minor_20 * inv_det,
    -minor_30 * inv_det,
    -minor_01 * inv_det,
    +minor_11 * inv_det,
    -minor_21 * inv_det,
    +minor_31 * inv_det,
    +minor_02 * inv_det,
    -minor_12 * inv_det,
    +minor_22 * inv_det,
    -minor_32 * inv_det,
    -minor_03 * inv_det,
    +minor_13 * inv_det,
    -minor_23 * inv_det,
    +minor_33 * inv_det);
}
} // namespace plex

#endif
