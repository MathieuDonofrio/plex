#ifndef GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H
#define GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H

#include "genebits/engine/debug/assertion.h"

namespace genebits::engine
{
template<typename T, size_t Rows, size_t Cols>
struct Mat;

// Transpose

template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Transpose(const Mat<T, 2, 2>& mat) noexcept
{
  return Mat<T, 2, 2>(mat[0][0], mat[1][0], mat[0][1], mat[1][1]);
}

// Determinant

template<typename T>
[[nodiscard]] constexpr T Determinant(const Mat<T, 2, 2>& mat) noexcept
{
  return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

// Product

template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Product(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs) noexcept
{
  return Mat { //
    lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1], //
    lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1], //
    lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1], //
    lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1]
  };
}

// Inverse

template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Inverse(const Mat<T, 2, 2>& mat, T det) noexcept
{
  ASSERT(det == Determinant(mat), "Incorrect determinant");
  ASSERT(det != 0, "Matrix not invertible. (Zero determinant)");

  const T inv_det = static_cast<T>(1) / det;

  return Mat<T, 2, 2>(mat[1][1] * inv_det, -mat[0][1] * inv_det, -mat[1][0] * inv_det, mat[0][0] * inv_det);
}

template<typename T>
[[nodiscard]] constexpr Mat<T, 2, 2> Inverse(const Mat<T, 2, 2>& mat) noexcept
{
  return Inverse(mat, Determinant(mat));
}

} // namespace genebits::engine

#endif
