#ifndef GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H
#define GENEBITS_ENGINE_MATH_MATRIX_COMPUTE_H

namespace genebits::engine
{
template<typename T, size_t Rows, size_t Cols>
struct Mat;

template<typename T>
constexpr Mat<T, 2, 2> Transpose(const Mat<T, 2, 2>& mat)
{
  return Mat<T, 2, 2>(mat[0][0], mat[1][0], mat[0][1], mat[1][1]);
}

template<typename T>
constexpr T Determinant(const Mat<T, 2, 2>& mat)
{
  return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

template<typename T>
constexpr Mat<T, 2, 2> Inverse(const Mat<T, 2, 2>& mat)
{
  const T inv_det = static_cast<T>(1) / Determinant(mat);

  return Mat<T, 2, 2>(mat[1][1] * inv_det, -mat[0][1] * inv_det, -mat[1][0] * inv_det, mat[0][0] * inv_det);
}

} // namespace genebits::engine

#endif
