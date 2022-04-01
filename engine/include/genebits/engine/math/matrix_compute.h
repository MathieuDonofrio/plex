#ifndef GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H
#define GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H

#include "genebits/engine/debug/assertion.h"

namespace genebits::engine
{
template<typename T, size_t Size>
struct Vec;

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
  return mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
         - mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2])
         + mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]);
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
  const T factor0 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
  const T factor1 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
  const T factor2 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
  const T factor3 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
  const T factor4 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
  const T factor5 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];

  const T factor10 = mat[1][1] * factor0 - mat[1][2] * factor1 + mat[1][3] * factor2;
  const T factor11 = mat[1][0] * factor0 - mat[1][2] * factor3 + mat[1][3] * factor4;
  const T factor12 = mat[1][0] * factor1 - mat[1][1] * factor3 + mat[1][3] * factor5;
  const T factor13 = mat[1][0] * factor2 - mat[1][1] * factor4 + mat[1][2] * factor5;

  return mat[0][0] * factor10 - mat[0][1] * factor11 + mat[0][2] * factor12 - mat[0][3] * factor13;
}

// Adjoint

// TODO

// Inverse

///
/// Computes the inverse of a matrix with a provided determinant.
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
  const T det = Determinant(mat);

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 2, 2>(mat[1][1] * inv_det, -mat[0][1] * inv_det, -mat[1][0] * inv_det, mat[0][0] * inv_det);
}

///
/// Computes the inverse of a matrix with a provided determinant.
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
  const T det = Determinant(mat);

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 3, 3>( //
    (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * inv_det,
    (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) * inv_det,
    (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * inv_det,
    (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) * inv_det,
    (mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2]) * inv_det,
    (mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1]) * inv_det,
    (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]) * inv_det,
    (mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2]) * inv_det,
    (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * inv_det);
}

///
/// Computes the inverse of a matrix with a provided determinant.
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
  const T det = Determinant(mat);

  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  // TODO

  return {};
}
} // namespace genebits::engine

#endif
