#ifndef PLEX_MATH_MAT4X4_H
#define PLEX_MATH_MAT4X4_H

#include "plex/math/vec4.h"

namespace plex
{
template<typename T, size_t Rows, size_t Cols>
struct Mat;

///
/// 4x4 mathematical matrix.
///
/// The matrix is stored in column major order.
///
/// @tparam T The type of the matrix elements.
///
template<typename T>
struct Mat<T, 4, 4>
{
public:
  using value_type = T;

  static constexpr size_t rows = 4;
  static constexpr size_t cols = 4;

  using col_type = Vec<T, rows>;
  using row_type = Vec<T, cols>;
  using transpose_type = Mat<T, rows, cols>;

private:
  col_type data[4];

public:
  ///
  /// Default constructor.
  ///
  /// Initializes the matrix to the identity matrix.
  ///
  constexpr Mat() noexcept
    : data { col_type(1, 0, 0, 0), col_type(0, 1, 0, 0), col_type(0, 0, 1, 0), col_type(0, 0, 0, 1) }
  {} // Identity

  ///
  /// Scalar constructor.
  ///
  /// Initializes the matrix to the scaled identity matrix.
  ///
  /// @param[in] scalar The scalar to scale the identity matrix by.
  ///
  constexpr Mat(T scalar) noexcept
    : data {
        col_type(scalar, 0, 0, 0), col_type(0, scalar, 0, 0), col_type(0, 0, scalar, 0), col_type(0, 0, 0, scalar)
      }
  {}

  ///
  /// Parametric constructor.
  ///
  /// @param[in] x0 The x component of the first column.
  /// @param[in] y0 The y component of the first column.
  /// @param[in] z0 The z component of the first column.
  /// @param[in] w0 The w component of the first column.
  /// @param[in] x1 The x component of the second column.
  /// @param[in] y1 The y component of the second column.
  /// @param[in] z1 The z component of the second column.
  /// @param[in] w1 The w component of the second column.
  /// @param[in] x2 The x component of the third column.
  /// @param[in] y2 The y component of the third column.
  /// @param[in] z2 The z component of the third column.
  /// @param[in] w2 The w component of the third column.
  /// @param[in] x3 The x component of the fourth column.
  /// @param[in] y3 The y component of the fourth column.
  /// @param[in] z3 The z component of the fourth column.
  /// @param[in] w3 The w component of the fourth column.
  ///
  constexpr Mat(T x0, T y0, T z0, T w0, T x1, T y1, T z1, T w1, T x2, T y2, T z2, T w2, T x3, T y3, T z3, T w3) noexcept
    : data { col_type(x0, y0, z0, w0), col_type(x1, y1, z1, w1), col_type(x2, y2, z2, w2), col_type(x3, y3, z3, w3) }
  {}

  ///
  /// Column constructor.
  ///
  /// @param a The first column.
  /// @param b The second column.
  /// @param c The third column.
  /// @param d The fourth column.
  ///
  constexpr Mat(const col_type& a, const col_type& b, const col_type& c, const col_type& d) noexcept
    : data { a, b, c, d }
  {}

  ///
  /// Copy constructor.
  ///
  /// @param[in] other The matrix to copy.
  ///
  constexpr Mat(const Mat& other) noexcept = default;

  ///
  /// Copy constructor.
  ///
  /// @tparam U The type of the other matrix.
  ///
  /// @param[in] other The matrix to copy.
  ///
  template<typename U>
  constexpr explicit Mat(const Mat<U, rows, cols>& other) noexcept
    : data { col_type(other[0]), col_type(other[1]), col_type(other[1]), col_type(other[2]) }
  {}

  ///
  /// Add assignment operator.
  ///
  /// @param[in] scalar The scalar to add.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator+=(T scalar) noexcept
  {
    data[0] += scalar;
    data[1] += scalar;
    data[2] += scalar;
    data[3] += scalar;
    return *this;
  }

  ///
  /// Add assignment operator.
  ///
  /// @param[in] other The matrix to add.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator+=(const Mat& other) noexcept
  {
    data[0] += other[0];
    data[1] += other[1];
    data[2] += other[2];
    data[3] += other[3];
    return *this;
  }

  ///
  /// Subtract assignment operator.
  ///
  /// @param[in] scalar The scalar to subtract.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator-=(T scalar) noexcept
  {
    data[0] -= scalar;
    data[1] -= scalar;
    data[2] -= scalar;
    data[3] -= scalar;
    return *this;
  }

  ///
  /// Subtract assignment operator.
  ///
  /// @param[in] other The matrix to subtract.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator-=(const Mat& other) noexcept
  {
    data[0] -= other[0];
    data[1] -= other[1];
    data[2] -= other[2];
    data[3] -= other[3];
    return *this;
  }

  ///
  /// Multiply assignment operator.
  ///
  /// @param[in] scalar The scalar to multiply.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator*=(T scalar) noexcept
  {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;
    return *this;
  }

  ///
  /// Multiply assignment operator.
  ///
  /// @param[in] other The matrix to multiply.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator*=(const Mat& other) noexcept
  {
    return (*this = *this * other);
  }

  ///
  /// Divide assignment operator.
  ///
  /// @param[in] scalar The scalar to divide.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator/=(T scalar) noexcept
  {
    data[0] /= scalar;
    data[1] /= scalar;
    data[2] /= scalar;
    data[3] /= scalar;
    return *this;
  }

  ///
  /// Pre-increment operator.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator++() noexcept
  {
    ++data[0];
    ++data[1];
    ++data[2];
    ++data[3];
    return *this;
  }

  ///
  /// Post-increment operator.
  ///
  /// @return A copy of the matrix.
  ///
  constexpr const Mat operator++(int) noexcept
  {
    Mat tmp(*this);
    return ++(*this), tmp;
  }

  ///
  /// Pre-decrement operator.
  ///
  /// @return A reference to the matrix.
  ///
  constexpr Mat& operator--() noexcept
  {
    --data[0];
    --data[1];
    --data[2];
    --data[3];
    return *this;
  }

  ///
  /// Post-decrement operator.
  ///
  /// @return A copy of the matrix.
  ///
  constexpr const Mat operator--(int) noexcept
  {
    Mat tmp(*this);
    return --(*this), tmp;
  }

  ///
  /// Subscript operator.
  ///
  /// Accesses the column at the specified index.
  ///
  /// @param[in] index The index corresponding to the column.
  ///
  /// @return A reference to the column.
  ///
  [[nodiscard]] constexpr const col_type& operator[](size_t index) const noexcept
  {
    ASSERT(index < cols, "Matrix column out of range");
    return data[index];
  }

  ///
  /// Subscript operator.
  ///
  /// Accesses the column at the specified index.
  ///
  /// @param[in] index The index corresponding to the column.
  ///
  /// @return A reference to the column.
  ///
  [[nodiscard]] constexpr col_type& operator[](size_t index) noexcept
  {
    ASSERT(index < cols, "Matrix column out of range");
    return data[index];
  }

  ///
  /// Add operator.
  ///
  /// @param[in] scalar The scalar to add.
  ///
  /// @return The result of the addition.
  ///
  [[nodiscard]] friend constexpr Mat operator+(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] + scalar, mat[1] + scalar, mat[2] + scalar, mat[3] + scalar };
  }

  ///
  /// Add operator.
  ///
  /// @param[in] lhs Left-hand side matrix.
  /// @param[in] rhs Right-hand side matrix.
  ///
  /// @return The result of the addition.
  ///
  [[nodiscard]] friend constexpr Mat operator+(const Mat& lhs, const Mat& rhs) noexcept
  {
    return Mat { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] };
  }

  ///
  /// Subtract operator.
  ///
  /// @param[in] scalar The scalar to subtract.
  ///
  /// @return The result of the subtraction.
  ///
  [[nodiscard]] friend constexpr Mat operator-(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] - scalar, mat[1] - scalar, mat[2] - scalar, mat[3] - scalar };
  }

  ///
  /// Subtract operator.
  ///
  /// @param[in] lhs Left-hand side matrix.
  /// @param[in] rhs Right-hand side matrix.
  ///
  /// @return The result of the subtraction.
  ///
  [[nodiscard]] friend constexpr Mat operator-(const Mat& lhs, const Mat& rhs) noexcept
  {
    return Mat { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] };
  }

  ///
  /// Multiplication operator.
  ///
  /// @param[in] scalar The scalar to multiply.
  ///
  /// @return The product of the matrix and the scalar.
  ///
  [[nodiscard]] friend constexpr Mat operator*(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] * scalar, mat[1] * scalar, mat[2] * scalar, mat[3] * scalar };
  }

  ///
  /// Multiplication operator.
  ///
  /// Performs matrix-vector multiplication.
  ///
  /// @param[in] mat Matrix to multiply vector by.
  /// @param[in] vec Vector to transform.
  ///
  /// @return The transformed vector.
  ///
  [[nodiscard]] friend constexpr Mat::col_type operator*(const Mat& mat, const Mat::row_type& vec) noexcept
  {
    return Mat::col_type { //
      mat[0][0] * vec.x + mat[1][0] * vec.y + mat[2][0] * vec.z + mat[3][0] * vec.w,
      mat[0][1] * vec.x + mat[1][1] * vec.y + mat[2][1] * vec.z + mat[3][1] * vec.w,
      mat[0][2] * vec.x + mat[1][2] * vec.y + mat[2][2] * vec.z + mat[3][2] * vec.w,
      mat[0][3] * vec.x + mat[1][3] * vec.y + mat[2][3] * vec.z + mat[3][3] * vec.w
    };
  }

  ///
  /// Multiplication operator.
  ///
  /// Performs vector-matrix multiplication.
  ///
  /// @param[in] mat Matrix to multiply.
  /// @param[in] vec Vector to multiply vector with.
  ///
  /// @return The transformed vector.
  ///
  [[nodiscard]] friend constexpr Mat::row_type operator*(const Mat::col_type& vec, const Mat& mat) noexcept
  {
    return Mat::row_type { //
      mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w,
      mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w,
      mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w,
      mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w
    };
  }

  ///
  /// Multiplication operator.
  ///
  /// Performs matrix-matrix multiplication.
  ///
  /// @param[in] lhs Left-hand side matrix.
  /// @param[in] rhs Right-hand side matrix.
  ///
  /// @return The product of the matrices.
  ///
  [[nodiscard]] friend constexpr Mat operator*(const Mat& lhs, const Mat& rhs) noexcept
  {
    // No point in manually unrolling these loops, some compilers will even optimize it slightly better with the loops.

    Mat result(0);

    for (size_t i = 0; i != 4; ++i)
    {
      for (size_t j = 0; j != 4; ++j)
      {
        result[i][j] = lhs[0][j] * rhs[i][0] + lhs[1][j] * rhs[i][1] + lhs[2][j] * rhs[i][2] + lhs[3][j] * rhs[i][3];
      }
    }

    return result;
  }

  ///
  /// Division operator.
  ///
  /// @param[in] scalar The scalar to divide.
  ///
  /// @return The quotient of the matrix and the scalar.
  ///
  [[nodiscard]] friend constexpr Mat operator/(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] / scalar, mat[1] / scalar, mat[2] / scalar, mat[3] / scalar };
  }

  ///
  /// Equality operator.
  ///
  /// @param[in] other The matrix to compare.
  ///
  /// @return True if the matrices are equal, false otherwise.
  ///
  [[nodiscard]] friend constexpr bool operator==(const Mat& lhs, const Mat& rhs) noexcept
  {
    return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) && (lhs[2] == rhs[2]) && (lhs[3] == rhs[3]);
  }

  ///
  /// Inequality operator.
  ///
  /// @param[in] other The matrix to compare.
  ///
  /// @return True if the matrices are not equal, false otherwise.
  ///
  [[nodiscard]] friend constexpr bool operator!=(const Mat& lhs, const Mat& rhs) noexcept
  {
    return !(lhs == rhs);
  }
};

using bool4x4 = Mat<bool, 4, 4>;
using int4x4 = Mat<int, 4, 4>;
using uint4x4 = Mat<unsigned int, 4, 4>;
using float4x4 = Mat<float, 4, 4>;
using double4x4 = Mat<double, 4, 4>;
} // namespace plex

namespace std
{
template<typename T>
struct hash<plex::Mat<T, 4, 4>>
{
  constexpr size_t operator()(const plex::Mat<T, 4, 4>& mat) const noexcept
  {
    return std::hash<T>()(mat[0]) ^ std::hash<T>(mat[1]) ^ std::hash<T>(mat[2]) ^ std::hash<T>(mat[3]);
  }
};
} // namespace std

#endif // GENEBITS_VEC_H
