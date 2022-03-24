#ifndef GENEBITS_ENGINE_MATH_VEC2_H
#define GENEBITS_ENGINE_MATH_VEC2_H

#include <numeric>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/math/swizzle.h"

namespace genebits::engine
{
template<typename T, size_t Size>
struct Vec;

template<typename T>
struct Vec<T, 2>
{
  static constexpr const size_t length = 2;

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

    // TODO VEC2 to VEC3
    // TODO VEC2 to VEC4
  };

  constexpr Vec() noexcept : x(0), y(0) {}

  constexpr Vec(T scalar) noexcept : x(scalar), y(scalar) {}

  constexpr Vec(T x, T y) noexcept : r(x), g(y) {}

  constexpr Vec(const Vec& other) noexcept = default;

  template<typename U>
  constexpr explicit Vec(const Vec<U, 2>& other) noexcept : x(static_cast<T>(other.x)), y(static_cast<T>(other.y))
  {}

  constexpr Vec& operator+=(T scalar) noexcept
  {
    x += scalar;
    y += scalar;
    return *this;
  }

  constexpr Vec& operator+=(const Vec& other) noexcept
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vec& operator-=(T scalar) noexcept
  {
    x -= scalar;
    y -= scalar;
    return *this;
  }

  constexpr Vec& operator-=(const Vec& other) noexcept
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vec& operator*=(T scalar) noexcept
  {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Vec& operator*=(const Vec& other) noexcept
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  constexpr Vec& operator/=(T scalar) noexcept
  {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  constexpr Vec& operator/=(const Vec& other) noexcept
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  constexpr Vec& operator%=(T scalar) noexcept
  {
    x %= scalar;
    y %= scalar;
    return *this;
  }

  constexpr Vec& operator%=(const Vec& other) noexcept
  {
    x %= other.x;
    y %= other.y;
    return *this;
  }

  constexpr Vec& operator&=(T scalar) noexcept
  {
    x &= scalar;
    y &= scalar;
    return *this;
  }

  constexpr Vec& operator&=(const Vec& other) noexcept
  {
    x &= other.x;
    y &= other.y;
    return *this;
  }

  constexpr Vec& operator|=(T scalar) noexcept
  {
    x |= scalar;
    y |= scalar;
    return *this;
  }

  constexpr Vec& operator|=(const Vec& other) noexcept
  {
    x |= other.x;
    y |= other.y;
    return *this;
  }

  constexpr Vec& operator^=(T scalar) noexcept
  {
    x ^= scalar;
    y ^= scalar;
    return *this;
  }

  constexpr Vec& operator^=(const Vec& other) noexcept
  {
    x ^= other.x;
    y ^= other.y;
    return *this;
  }

  constexpr Vec& operator<<=(T scalar) noexcept
  {
    x <<= scalar;
    y <<= scalar;
    return *this;
  }

  constexpr Vec& operator<<=(const Vec& other) noexcept
  {
    x <<= other.x;
    y <<= other.y;
    return *this;
  }

  constexpr Vec& operator>>=(T scalar) noexcept
  {
    x >>= scalar;
    y >>= scalar;
    return *this;
  }

  constexpr Vec& operator>>=(const Vec& other) noexcept
  {
    x >>= other.x;
    y >>= other.y;
    return *this;
  }

  constexpr Vec& operator++() noexcept
  {
    ++x;
    ++y;
    return *this;
  }

  constexpr const Vec operator++(int) noexcept
  {
    Vec<T, 2> tmp(*this);
    return ++(*this), tmp;
  }

  constexpr Vec& operator--() noexcept
  {
    --x;
    --y;
    return *this;
  }

  constexpr const Vec operator--(int) noexcept
  {
    Vec tmp(*this);
    return --(*this), tmp;
  }

  [[nodiscard]] constexpr T operator[](size_t index) const noexcept
  {
    ASSERT(index < length, "Vector element out of range");
    return data[index];
  }

  [[nodiscard]] constexpr T& operator[](size_t index) noexcept
  {
    ASSERT(index < length, "Vector element out of range");
    return data[index];
  }

  [[nodiscard]] explicit constexpr operator bool() const noexcept
  {
    return !(x == 0 && y == 0);
  }

  [[nodiscard]] friend constexpr Vec operator+(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x + scalar, vec.y + scalar };
  }

  [[nodiscard]] friend constexpr Vec operator+(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x + rhs.x, lhs.y + rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator-(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x - scalar, vec.y - scalar };
  }

  [[nodiscard]] friend constexpr Vec operator-(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x - rhs.x, lhs.y - rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator*(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x * scalar, vec.y * scalar };
  }

  [[nodiscard]] friend constexpr Vec operator*(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x * rhs.x, lhs.y * rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator/(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x / scalar, vec.y / scalar };
  }

  [[nodiscard]] friend constexpr Vec operator/(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x / rhs.x, lhs.y / rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator%(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x % scalar, vec.y % scalar };
  }

  [[nodiscard]] friend constexpr Vec operator%(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x % rhs.x, lhs.y % rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator&(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x & scalar, vec.y & scalar };
  }

  [[nodiscard]] friend constexpr Vec operator&(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x & rhs.x, lhs.y & rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator|(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x | scalar, vec.y | scalar };
  }

  [[nodiscard]] friend constexpr Vec operator|(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x | rhs.x, lhs.y | rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator^(const Vec& vec, T scalar) noexcept
  {
    return Vec { vec.x | scalar, vec.y | scalar };
  }

  [[nodiscard]] friend constexpr Vec operator^(const Vec& lhs, const Vec& rhs) noexcept
  {
    return Vec { lhs.x ^ rhs.x, lhs.y ^ rhs.y };
  }

  [[nodiscard]] friend constexpr Vec operator~(const Vec& vec) noexcept
  {
    return Vec { ~vec.x, ~vec.y };
  }

  [[nodiscard]] friend constexpr bool operator==(const Vec& lhs, const Vec& rhs) noexcept
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  [[nodiscard]] friend constexpr bool operator!=(const Vec& lhs, const Vec& rhs) noexcept
  {
    return !(lhs == rhs);
  }
};

[[nodiscard]] constexpr Vec<bool, 2> operator&&(const Vec<bool, 2>& lhs, const Vec<bool, 2>& rhs) noexcept
{
  return Vec<bool, 2> { lhs.x && rhs.y, lhs.y && rhs.y };
}

[[nodiscard]] constexpr Vec<bool, 2> operator||(const Vec<bool, 2>& lhs, const Vec<bool, 2>& rhs) noexcept
{
  return Vec<bool, 2> { lhs.x || rhs.y, lhs.y || rhs.y };
}

using bool2 = Vec<bool, 2>;
using char2 = Vec<char, 2>;
using uchar2 = Vec<unsigned char, 2>;
using short2 = Vec<short, 2>;
using ushort2 = Vec<unsigned short, 2>;
using int2 = Vec<int, 2>;
using uint2 = Vec<unsigned int, 2>;
using float2 = Vec<float, 2>;
using double2 = Vec<double, 2>;
} // namespace genebits::engine

#endif // GENEBITS_VEC_H
