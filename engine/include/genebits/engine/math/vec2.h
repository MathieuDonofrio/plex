#ifndef GENEBITS_ENGINE_MATH_VEC2_H
#define GENEBITS_ENGINE_MATH_VEC2_H

#include <numeric>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/math/swizzle.h"

namespace genebits::engine
{
template<typename T, size_t Size>
struct Vec;

///
/// 2-dimensional mathematical vector.
///
/// The memory layout of the vector is: [x, y].
///
/// @tparam T The type of the components
///
template<typename T>
struct Vec<T, 2>
{
public:
  using value_type = T;

  static constexpr size_t length = 2;

  union
  {
    T data[length];

    struct
    {
      T x, y;
    };

    struct
    {
      T r, g;
    };

    struct
    {
      T s, t;
    };

    SWIZZLE_VEC2_TO_VEC2(Vec, x, y);
    SWIZZLE_VEC2_TO_VEC2(Vec, r, g);
    SWIZZLE_VEC2_TO_VEC2(Vec, s, t);

    SWIZZLE_VEC2_TO_VEC3(Vec, x, y);
    SWIZZLE_VEC2_TO_VEC3(Vec, r, g);
    SWIZZLE_VEC2_TO_VEC3(Vec, s, t);

    SWIZZLE_VEC2_TO_VEC4(Vec, x, y);
    SWIZZLE_VEC2_TO_VEC4(Vec, r, g);
    SWIZZLE_VEC2_TO_VEC4(Vec, s, t);
  };

public:
  ///
  /// Default constructor.
  ///
  /// Initializes all components to zero.
  ///
  constexpr Vec() noexcept : x(0), y(0) {}

  ///
  /// Scalar constructor.
  ///
  /// Creates a vector with all components equal to the given scalar.
  ///
  /// @param[in] scalar The scalar to use for all components.
  ///
  constexpr Vec(T scalar) noexcept : x(scalar), y(scalar) {}

  ///
  /// Parametric constructor.
  ///
  /// Creates a vector with the given components.
  ///
  /// @param[in] x_ The x component.
  /// @param[in] y_ The y component.
  ///
  constexpr Vec(T x_, T y_) noexcept : x(x_), y(y_) {}

  ///
  /// Copy constructor.
  ///
  /// @param[in] other The vector to copy.
  ///
  constexpr Vec(const Vec& other) noexcept = default;

  ///
  /// Copy constructor.
  ///
  /// @tparam U The type of the other vector.
  ///
  /// @param[in] other The vector to copy.
  ///
  template<typename U>
  constexpr explicit Vec(const Vec<U, 2>& other) noexcept : Vec(static_cast<T>(other.x), static_cast<T>(other.y))
  {}

  ///
  /// Add assignment operator.
  ///
  /// @param[in] scalar The scalar to add.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator+=(T scalar) noexcept
  {
    x += scalar;
    y += scalar;
    return *this;
  }

  ///
  /// Add assignment operator.
  ///
  /// @param[in] other The vector to add.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator+=(const Vec& other) noexcept
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  ///
  /// Subtract assignment operator.
  ///
  /// @param[in] scalar The scalar to subtract.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator-=(T scalar) noexcept
  {
    x -= scalar;
    y -= scalar;
    return *this;
  }

  ///
  /// Subtract assignment operator.
  ///
  /// @param[in] other The vector to subtract.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator-=(const Vec& other) noexcept
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  ///
  /// Multiply assignment operator.
  ///
  /// @param[in] scalar The scalar to multiply.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator*=(T scalar) noexcept
  {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  ///
  /// Multiply assignment operator.
  ///
  /// @param[in] other The vector to multiply.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator*=(const Vec& other) noexcept
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  ///
  /// Divide assignment operator.
  ///
  /// @param[in] scalar The scalar to divide.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator/=(T scalar) noexcept
  {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  ///
  /// Divide assignment operator.
  ///
  /// @param[in] other The vector to divide.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator/=(const Vec& other) noexcept
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  ///
  /// Modulo assignment operator.
  ///
  /// @param[in] scalar The scalar to modulo.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator%=(T scalar) noexcept
  {
    x %= scalar;
    y %= scalar;
    return *this;
  }

  ///
  /// Modulo assignment operator.
  ///
  /// @param[in] other The vector to modulo.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator%=(const Vec& other) noexcept
  {
    x %= other.x;
    y %= other.y;
    return *this;
  }

  ///
  /// Bitwise and assignment operator.
  ///
  /// @param[in] scalar The scalar to bitwise and.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator&=(T scalar) noexcept
  {
    x &= scalar;
    y &= scalar;
    return *this;
  }

  ///
  /// Bitwise and assignment operator.
  ///
  /// @param[in] other The vector to bitwise and.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator&=(const Vec& other) noexcept
  {
    x &= other.x;
    y &= other.y;
    return *this;
  }

  ///
  /// Bitwise or assignment operator.
  ///
  /// @param[in] scalar The scalar to bitwise or.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator|=(T scalar) noexcept
  {
    x |= scalar;
    y |= scalar;
    return *this;
  }

  ///
  /// Bitwise or assignment operator.
  ///
  /// @param[in] other The vector to bitwise or.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator|=(const Vec& other) noexcept
  {
    x |= other.x;
    y |= other.y;
    return *this;
  }

  ///
  /// Bitwise xor assignment operator.
  ///
  /// @param[in] scalar The scalar to bitwise xor.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator^=(T scalar) noexcept
  {
    x ^= scalar;
    y ^= scalar;
    return *this;
  }

  ///
  /// Bitwise xor assignment operator.
  ///
  /// @param[in] other The vector to bitwise xor.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator^=(const Vec& other) noexcept
  {
    x ^= other.x;
    y ^= other.y;
    return *this;
  }

  ///
  /// Bitwise left shift assignment operator.
  ///
  /// @param[in] scalar The scalar to bitwise left shift.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator<<=(T scalar) noexcept
  {
    x <<= scalar;
    y <<= scalar;
    return *this;
  }

  ///
  /// Bitwise left shift assignment operator.
  ///
  /// @param[in] other The vector to bitwise left shift.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator<<=(const Vec& other) noexcept
  {
    x <<= other.x;
    y <<= other.y;
    return *this;
  }

  ///
  /// Bitwise right shift assignment operator
  ///
  /// @param[in] scalar The scalar to bitwise right shift.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator>>=(T scalar) noexcept
  {
    x >>= scalar;
    y >>= scalar;
    return *this;
  }

  ///
  /// Bitwise right shift assignment operator
  ///
  /// @param[in] other The vector to bitwise right shift.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator>>=(const Vec& other) noexcept
  {
    x >>= other.x;
    y >>= other.y;
    return *this;
  }

  ///
  /// Pre increment.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator++() noexcept
  {
    ++x;
    ++y;
    return *this;
  }

  ///
  /// Post increment.
  ///
  /// @returns A copy of this vector.
  ///
  constexpr const Vec operator++(int) noexcept
  {
    Vec tmp(*this);
    return ++(*this), tmp;
  }

  ///
  /// Pre decrement.
  ///
  /// @returns A reference to this vector.
  ///
  constexpr Vec& operator--() noexcept
  {
    --x;
    --y;
    return *this;
  }

  ///
  /// Post decrement.
  ///
  /// @returns A copy of this vector.
  ///
  constexpr const Vec operator--(int) noexcept
  {
    Vec tmp(*this);
    return --(*this), tmp;
  }

  ///
  /// Subscript operator.
  ///
  /// @param[in] index The index of the component to access.
  ///
  [[nodiscard]] constexpr T operator[](size_t index) const noexcept
  {
    ASSERT(index < length, "Vector component out of range");
    if (std::is_constant_evaluated())
    {
      // Avoids non-active union member access during constant evaluation.
      switch (index)
      {
      default:
      case 0: return x;
      case 1: return y;
      }
    }
    else
    {
      return data[index];
    }
  }

  ///
  /// Subscript operator.
  ///
  /// @param[in] index The index of the component to access.
  ///
  [[nodiscard]] constexpr T& operator[](size_t index) noexcept
  {
    ASSERT(index < length, "Vector component out of range");
    if (std::is_constant_evaluated())
    {
      // Avoids non-active union member access during constant evaluation.
      switch (index)
      {
      default:
      case 0: return x;
      case 1: return y;
      }
    }
    else
    {
      return data[index];
    }
  }

  ///
  /// Boolean conversion operator.
  ///
  /// @returns True if the vector is not zero.
  ///
  [[nodiscard]] explicit constexpr operator bool() const noexcept
  {
    return x || y;
  }

  ///
  /// Addition operator.
  ///
  /// @param[in] vec The vector to add.
  /// @param[in] scalar The scalar to add.
  ///
  /// @returns The result of the addition.
  ///
  [[nodiscard]] friend constexpr Vec operator+(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x + scalar, vec.y + scalar };
  }

  ///
  /// Addition operator.
  ///
  /// @param[in] lhs The left hand side of the addition.
  /// @param[in] rhs The right hand side of the addition.
  ///
  /// @returns The result of the addition.
  ///
  [[nodiscard]] friend constexpr Vec operator+(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x + rhs.x, lhs.y + rhs.y };
  }

  ///
  /// Subtraction operator.
  ///
  /// @param[in] vec The vector to subtract.
  /// @param[in] scalar The scalar to subtract.
  ///
  /// @returns The result of the subtraction.
  ///
  [[nodiscard]] friend constexpr Vec operator-(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x - scalar, vec.y - scalar };
  }

  ///
  /// Subtraction operator.
  ///
  /// @param[in] lhs The left hand side of the subtraction.
  /// @param[in] rhs The right hand side of the subtraction.
  ///
  /// @returns The result of the subtraction.
  ///
  [[nodiscard]] friend constexpr Vec operator-(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x - rhs.x, lhs.y - rhs.y };
  }

  ///
  /// Multiplication operator.
  ///
  /// @param[in] vec The vector to multiply.
  /// @param[in] scalar The scalar to multiply.
  ///
  /// @returns The result of the multiplication.
  ///
  [[nodiscard]] friend constexpr Vec operator*(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x * scalar, vec.y * scalar };
  }

  ///
  /// Multiplication operator.
  ///
  /// @param[in] lhs The left hand side of the multiplication.
  /// @param[in] rhs The right hand side of the multiplication.
  ///
  /// @returns The result of the multiplication.
  ///
  [[nodiscard]] friend constexpr Vec operator*(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x * rhs.x, lhs.y * rhs.y };
  }

  ///
  /// Division operator.
  ///
  /// @param[in] vec The vector to divide.
  /// @param[in] scalar The scalar to divide.
  ///
  /// @returns The result of the division.
  ///
  [[nodiscard]] friend constexpr Vec operator/(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x / scalar, vec.y / scalar };
  }

  ///
  /// Division operator.
  ///
  /// @param[in] lhs The left hand side of the division.
  /// @param[in] rhs The right hand side of the division.
  ///
  /// @returns The result of the division.
  ///
  [[nodiscard]] friend constexpr Vec operator/(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x / rhs.x, lhs.y / rhs.y };
  }

  ///
  /// Modulo operator.
  ///
  /// @param[in] vec The vector to modulo.
  /// @param[in] scalar The scalar to modulo.
  ///
  /// @returns The result of the modulo.
  ///
  [[nodiscard]] friend constexpr Vec operator%(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x % scalar, vec.y % scalar };
  }

  ///
  /// Modulo operator.
  ///
  /// @param[in] lhs The left hand side of the modulo.
  /// @param[in] rhs The right hand side of the modulo.
  ///
  /// @returns The result of the modulo.
  ///
  [[nodiscard]] friend constexpr Vec operator%(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x % rhs.x, lhs.y % rhs.y };
  }

  ///
  /// Bitwise and operator.
  ///
  /// @param[in] vec The vector to bitwise and.
  /// @param[in] scalar The scalar to bitwise and.
  ///
  /// @returns The result of the bitwise and.
  ///
  [[nodiscard]] friend constexpr Vec operator&(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x & scalar, vec.y & scalar };
  }

  ///
  /// Bitwise and operator.
  ///
  /// @param[in] lhs The left hand side of the bitwise and.
  /// @param[in] rhs The right hand side of the bitwise and.
  ///
  /// @returns The result of the bitwise and.
  ///
  [[nodiscard]] friend constexpr Vec operator&(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x & rhs.x, lhs.y & rhs.y };
  }

  ///
  /// Bitwise or operator.
  ///
  /// @param[in] vec The vector to bitwise or.
  /// @param[in] scalar The scalar to bitwise or.
  ///
  /// @returns The result of the bitwise or.
  ///
  [[nodiscard]] friend constexpr Vec operator|(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x | scalar, vec.y | scalar };
  }

  ///
  /// Bitwise or operator.
  ///
  /// @param[in] lhs The left hand side of the bitwise or.
  /// @param[in] rhs The right hand side of the bitwise or.
  ///
  /// @returns The result of the bitwise or.
  ///
  [[nodiscard]] friend constexpr Vec operator|(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x | rhs.x, lhs.y | rhs.y };
  }

  ///
  /// Bitwise xor operator.
  ///
  /// @param[in] vec The vector to bitwise xor.
  /// @param[in] scalar The scalar to bitwise xor.
  ///
  /// @returns The result of the bitwise xor.
  ///
  [[nodiscard]] friend constexpr Vec operator^(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x ^ scalar, vec.y ^ scalar };
  }

  ///
  /// Bitwise xor operator.
  ///
  /// @param[in] lhs The left hand side of the bitwise xor.
  /// @param[in] rhs The right hand side of the bitwise xor.
  ///
  /// @returns The result of the bitwise xor.
  ///
  [[nodiscard]] friend constexpr Vec operator^(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x ^ rhs.x, lhs.y ^ rhs.y };
  }

  ///
  /// Bitwise left shift operator.
  ///
  /// @param[in] vec The vector to bitwise left shift.
  /// @param[in] scalar The scalar to bitwise left shift.
  ///
  /// @returns The result of the bitwise left shift.
  ///
  [[nodiscard]] friend constexpr Vec operator<<(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x << scalar, vec.y << scalar };
  }

  ///
  /// Bitwise left shift operator.
  ///
  /// @param[in] lhs The left hand side of the bitwise left shift.
  /// @param[in] rhs The right hand side of the bitwise left shift.
  ///
  /// @returns The result of the bitwise left shift.
  ///
  [[nodiscard]] friend constexpr Vec operator<<(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x << rhs.x, lhs.y << rhs.y };
  }

  ///
  /// Bitwise right shift operator.
  ///
  /// @param[in] vec The vector to bitwise right shift.
  /// @param[in] scalar The scalar to bitwise right shift.
  ///
  /// @returns The result of the bitwise right shift.
  ///
  [[nodiscard]] friend constexpr Vec operator>>(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x >> scalar, vec.y >> scalar };
  }

  ///
  /// Bitwise right shift operator.
  ///
  /// @param[in] lhs The left hand side of the bitwise right shift.
  /// @param[in] rhs The right hand side of the bitwise right shift.
  ///
  /// @returns The result of the bitwise right shift.
  ///
  [[nodiscard]] friend constexpr Vec operator>>(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x >> rhs.x, lhs.y >> rhs.y };
  }

  ///
  /// Not operator.
  ///
  /// @param[in] vec The vector to not.
  ///
  /// @returns The result of the not.
  ///
  [[nodiscard]] friend constexpr Vec operator~(const Vec& vec) noexcept
  {
    return Vec { ~vec.x, ~vec.y };
  }

  ///
  /// Equality operator.
  ///
  /// @param[in] lhs The left hand side of the equality.
  /// @param[in] rhs The right hand side of the equality.
  ///
  /// @returns True if the two vectors are equal, false otherwise.
  ///
  [[nodiscard]] friend constexpr bool operator==(const Vec& lhs, const Vec& rhs) noexcept
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  ///
  /// Inequality operator.
  ///
  /// @param[in] lhs The left hand side of the inequality.
  /// @param[in] rhs The right hand side of the inequality.
  ///
  /// @returns True if the two vectors are not equal, false otherwise.
  ///
  [[nodiscard]] friend constexpr bool operator!=(const Vec& lhs, const Vec& rhs) noexcept
  {
    return !(lhs == rhs);
  }
};

///
/// And operator.
///
/// @param[in] lhs The left hand side of the and.
/// @param[in] rhs The right hand side of the and.
///
/// @returns The result of the and.
///
[[nodiscard]] constexpr Vec<bool, 2> operator&&(const Vec<bool, 2>& lhs, const Vec<bool, 2>& rhs) noexcept
{
  return Vec<bool, 2> { lhs.x && rhs.y, lhs.y && rhs.y };
}

///
/// Or operator.
///
/// @param[in] lhs The left hand side of the or.
/// @param[in] rhs The right hand side of the or.
///
/// @returns The result of the or.
///
[[nodiscard]] constexpr Vec<bool, 2> operator||(const Vec<bool, 2>& lhs, const Vec<bool, 2>& rhs) noexcept
{
  return Vec<bool, 2> { lhs.x || rhs.y, lhs.y || rhs.y };
}

using bool2 = Vec<bool, 2>;
using int2 = Vec<int, 2>;
using uint2 = Vec<unsigned int, 2>;
using float2 = Vec<float, 2>;
using double2 = Vec<double, 2>;
} // namespace genebits::engine

namespace std
{
template<typename T>
struct hash<genebits::engine::Vec<T, 2>>
{
  constexpr size_t operator()(const genebits::engine::Vec<T, 2>& vec) const noexcept
  {
    return hash<T>()(vec.x) ^ (hash<T>()(vec.y) << 1);
  }
};
} // namespace std

#endif
