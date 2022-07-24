#ifndef PLEX_MATH_VEC4_H
#define PLEX_MATH_VEC4_H

#include <numeric>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/math/swizzle.h"

namespace plex
{
template<typename T, size_t Size>
struct Vec;

///
/// 4-dimensional mathematical vector.
///
/// The memory layout of the vector is: [x, y, z, w].
///
/// @tparam T The type of the components
///
template<typename T>
struct Vec<T, 4>
{
public:
  using value_type = T;

  static constexpr size_t length = 4;

  union
  {
    T x, r, s;

    SWIZZLE_VEC4_TO_VEC2(Vec, x, y, z, w);
    SWIZZLE_VEC4_TO_VEC2(Vec, r, g, b, a);
    SWIZZLE_VEC4_TO_VEC2(Vec, s, t, p, q);

    SWIZZLE_VEC4_TO_VEC3(Vec, x, y, z, w);
    SWIZZLE_VEC4_TO_VEC3(Vec, r, g, b, a);
    SWIZZLE_VEC4_TO_VEC3(Vec, s, t, p, q);

    SWIZZLE_VEC4_TO_VEC4(Vec, x, y, z, w);
    SWIZZLE_VEC4_TO_VEC4(Vec, r, g, b, a);
    SWIZZLE_VEC4_TO_VEC4(Vec, s, t, p, q);
  };

  union
  {
    T y, g, t;
  };

  union
  {
    T z, b, p;
  };

  union
  {
    T w, a, q;
  };

  ///
  /// Returns a pointer to the first element of the vector.
  ///
  /// @return A pointer to the first element of the vector.
  ///
  [[nodiscard]] const T* data() const noexcept
  {
    return std::bit_cast<const T*>(this);
  }

  ///
  /// Returns a pointer to the first element of the vector.
  ///
  /// @return A pointer to the first element of the vector.
  ///
  [[nodiscard]] T* data() noexcept
  {
    return std::bit_cast<T*>(this);
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
      case 2: return z;
      case 3: return w;
      }
    }
    else
    {
      return data()[index];
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
      case 2: return z;
      case 3: return w;
      }
    }
    else
    {
      return data()[index];
    }
  }

public:
  ///
  /// Default constructor.
  ///
  /// Initializes all components to zero.
  ///
  constexpr Vec() noexcept : x(0), y(0), z(0), w(0) {}

  ///
  /// Scalar constructor.
  ///
  /// Creates a vector with all components equal to the given scalar.
  ///
  /// @param[in] scalar The scalar to use for all components.
  ///
  constexpr Vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}

  ///
  /// Parametric constructor.
  ///
  /// Creates a vector with the given components.
  ///
  /// @param[in] x_ The x component.
  /// @param[in] y_ The y component.
  /// @param[in] z_ The z component.
  /// @param[in] w_ The w component.
  ///
  constexpr Vec(T x_, T y_, T z_, T w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}

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
  constexpr explicit Vec(const Vec<U, 4>& other) noexcept
    : Vec(static_cast<T>(other.x), static_cast<T>(other.y), static_cast<T>(other.z), static_cast<T>(other.w))
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
    z += scalar;
    w += scalar;
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
    z += other.z;
    w += other.w;
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
    z -= scalar;
    w -= scalar;
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
    z -= other.z;
    w -= other.w;
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
    z *= scalar;
    w *= scalar;
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
    z *= other.z;
    w *= other.w;
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
    z /= scalar;
    w /= scalar;
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
    z /= other.z;
    w /= other.w;
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
    z %= scalar;
    w %= scalar;
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
    z %= other.z;
    w %= other.w;
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
    z &= scalar;
    w &= scalar;
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
    z &= other.z;
    w &= other.w;
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
    z |= scalar;
    w |= scalar;
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
    z |= other.z;
    w |= other.w;
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
    z ^= scalar;
    w ^= scalar;
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
    z ^= other.z;
    w ^= other.w;
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
    z <<= scalar;
    w <<= scalar;
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
    z <<= other.z;
    w <<= other.w;
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
    z >>= scalar;
    w >>= scalar;
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
    z >>= other.z;
    w >>= other.w;
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
    ++z;
    ++w;
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
    --z;
    --w;
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
  /// Boolean conversion operator.
  ///
  /// @returns True if the vector is not zero.
  ///
  [[nodiscard]] explicit constexpr operator bool() const noexcept
  {
    return x || y || z || w;
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
    return Vec { vec.x + scalar, vec.y + scalar, vec.z + scalar, vec.w + scalar };
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
    return Vec { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
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
    return Vec { vec.x - scalar, vec.y - scalar, vec.z - scalar, vec.w - scalar };
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
    return Vec { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
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
    return Vec { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar };
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
    return Vec { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
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
    return Vec { vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar };
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
    return Vec { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
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
    return Vec { vec.x % scalar, vec.y % scalar, vec.z % scalar, vec.w % scalar };
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
    return Vec { lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z, lhs.w % rhs.w };
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
    return Vec { vec.x & scalar, vec.y & scalar, vec.z & scalar, vec.w & scalar };
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
    return Vec { lhs.x & rhs.x, lhs.y & rhs.y, lhs.z & rhs.z, lhs.w & rhs.w };
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
    return Vec { vec.x | scalar, vec.y | scalar, vec.z | scalar, vec.w | scalar };
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
    return Vec { lhs.x | rhs.x, lhs.y | rhs.y, lhs.z | rhs.z, lhs.w | rhs.w };
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
    return Vec { vec.x ^ scalar, vec.y ^ scalar, vec.z ^ scalar, vec.w ^ scalar };
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
    return Vec { lhs.x ^ rhs.x, lhs.y ^ rhs.y, lhs.z ^ rhs.z, lhs.w ^ rhs.w };
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
    return Vec { vec.x << scalar, vec.y << scalar, vec.z << scalar, vec.w << scalar };
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
    return Vec { lhs.x << rhs.x, lhs.y << rhs.y, lhs.z << rhs.z, lhs.w << rhs.w };
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
    return Vec { vec.x >> scalar, vec.y >> scalar, vec.z >> scalar, vec.w >> scalar };
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
    return Vec { lhs.x >> rhs.x, lhs.y >> rhs.y, lhs.z >> rhs.z, lhs.w >> rhs.w };
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
    return Vec { ~vec.x, ~vec.y, ~vec.z, ~vec.w };
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
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
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
[[nodiscard]] constexpr Vec<bool, 4> operator&&(const Vec<bool, 4>& lhs, const Vec<bool, 4>& rhs) noexcept
{
  return Vec<bool, 4> { lhs.x && rhs.y, lhs.y && rhs.y, lhs.z && rhs.z, lhs.w && rhs.w };
}

///
/// Or operator.
///
/// @param[in] lhs The left hand side of the or.
/// @param[in] rhs The right hand side of the or.
///
/// @returns The result of the or.
///
[[nodiscard]] constexpr Vec<bool, 4> operator||(const Vec<bool, 4>& lhs, const Vec<bool, 4>& rhs) noexcept
{
  return Vec<bool, 4> { lhs.x || rhs.y, lhs.y || rhs.y, lhs.z || rhs.z, lhs.w || rhs.w };
}

using bool4 = Vec<bool, 4>;
using int4 = Vec<int, 4>;
using uint4 = Vec<unsigned int, 4>;
using float4 = Vec<float, 4>;
using double4 = Vec<double, 4>;
} // namespace plex

namespace std
{
template<typename T>
struct hash<plex::Vec<T, 4>>
{
  constexpr size_t operator()(const plex::Vec<T, 4>& vec) const noexcept
  {
    return hash<T>()(vec.x) ^ hash<T>()(vec.y) ^ hash<T>()(vec.z) ^ hash<T>()(vec.w);
  }
};
} // namespace std

#endif
