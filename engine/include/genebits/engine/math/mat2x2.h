#ifndef GENEBITS_ENGINE_MATH_MAT2X2_H
#define GENEBITS_ENGINE_MATH_MAT2X2_H

#include "genebits/engine/math/matrix_compute.h"
#include "genebits/engine/math/vec2.h"

namespace genebits::engine
{
template<typename T, size_t Rows, size_t Cols>
struct Mat;

template<typename T>
struct Mat<T, 2, 2>
{
public:
  static constexpr const size_t rows = 2;
  static constexpr const size_t cols = 2;

  using ColType = Vec<T, rows>;
  using RowType = Vec<T, cols>;
  using TransposeType = Mat<T, rows, cols>;

private:
  ColType data[2];

public:
  constexpr Mat() noexcept : data { ColType(1, 0), ColType(0, 1) } {} // Identity

  constexpr Mat(T scalar) noexcept : data { ColType(scalar, 0), ColType(0, scalar) } {} // Identity

  constexpr Mat(T x0, T y0, T x1, T y1) noexcept : data { ColType(x0, y0), ColType(x1, y1) } {}

  constexpr Mat(const ColType& a, const ColType& b) noexcept : data { a, b } {}

  constexpr Mat(const Mat& other) noexcept = default;

  template<typename U>
  constexpr explicit Mat(const Mat<U, rows, cols>& other) noexcept : data(ColType(other[0]), ColType(other[1]))
  {}

  constexpr Mat& operator+=(T scalar) noexcept
  {
    data[0] += scalar;
    data[1] += scalar;
    return *this;
  }

  constexpr Mat& operator+=(const Mat& other) noexcept
  {
    data[0] += other[0];
    data[1] += other[1];
    return *this;
  }

  constexpr Mat& operator-=(T scalar) noexcept
  {
    data[0] -= scalar;
    data[1] -= scalar;
    return *this;
  }

  constexpr Mat& operator-=(const Mat& other) noexcept
  {
    data[0] -= other[0];
    data[1] -= other[1];
    return *this;
  }

  constexpr Mat& operator*=(T scalar) noexcept
  {
    data[0] *= scalar;
    data[1] *= scalar;
    return *this;
  }

  constexpr Mat& operator*=(const Mat& other) noexcept
  {
    return (*this = *this * other);
  }

  constexpr Mat& operator/=(T scalar) noexcept
  {
    data[0] /= scalar;
    data[1] /= scalar;
    return *this;
  }

  constexpr Mat& operator/=(const Mat& other) noexcept
  {
    return (*this = *this * Inverse(other));
  }

  constexpr Mat& operator++() noexcept
  {
    ++data[0];
    ++data[1];
    return *this;
  }

  constexpr const Mat operator++(int) noexcept
  {
    Mat tmp(*this);
    return ++(*this), tmp;
  }

  constexpr Mat& operator--() noexcept
  {
    --data[0];
    --data[1];
    return *this;
  }

  constexpr const Mat operator--(int) noexcept
  {
    Mat tmp(*this);
    return --(*this), tmp;
  }

  [[nodiscard]] constexpr const ColType& operator[](size_t index) const noexcept
  {
    ASSERT(index < cols, "Matrix column out of range");
    return data[index];
  }

  [[nodiscard]] constexpr ColType& operator[](size_t index) noexcept
  {
    ASSERT(index < cols, "Matrix column out of range");
    return data[index];
  }

  [[nodiscard]] friend constexpr Mat operator*(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] * scalar, mat[1] * scalar };
  }

  [[nodiscard]] friend constexpr Mat operator*(const Mat& lhs, const Mat& rhs) noexcept
  {
    return Mat { //
      lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1], //
      lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1], //
      lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1], //
      lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1]
    };
  }

  // TODO Other multiplications

  [[nodiscard]] friend constexpr Mat operator/(const Mat& mat, T scalar) noexcept
  {
    return Mat { mat[0] / scalar, mat[1] / scalar };
  }

  [[nodiscard]] friend constexpr Mat operator/(const Mat& lhs, const Mat& rhs) noexcept
  {
    return lhs * Inverse(rhs);
  }

  // TODO Other divisions

  [[nodiscard]] friend constexpr bool operator==(const Mat& lhs, const Mat&& rhs) noexcept
  {
    return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]);
  }

  [[nodiscard]] friend constexpr bool operator!=(const Mat&& lhs, const Mat&& rhs) noexcept
  {
    return !(lhs == rhs);
  }
};

using bool2x2 = Mat<bool, 2, 2>;
using char2x2 = Mat<char, 2, 2>;
using uchar2x2 = Mat<unsigned char, 2, 2>;
using short2x2 = Mat<short, 2, 2>;
using ushort2x2 = Mat<unsigned short, 2, 2>;
using int2x2 = Mat<int, 2, 2>;
using uint2x2 = Mat<unsigned int, 2, 2>;
using float2x2 = Mat<float, 2, 2>;
using double2x2 = Mat<double, 2, 2>;
} // namespace genebits::engine

#endif // GENEBITS_VEC_H
