#ifndef GENEBITS_ENGINE_MATH_SWIZZLE_H
#define GENEBITS_ENGINE_MATH_SWIZZLE_H

#include <type_traits>

namespace genebits::engine
{
template<typename Type, size_t... Args>
struct Swizzle;

template<template<typename, size_t> class Vec, typename T, size_t Size, size_t... Args>
struct Swizzle<Vec<T, Size>, Args...>
{
private:
  static constexpr size_t SwizzleSize = sizeof...(Args);
  static constexpr size_t SwizzleMap[SwizzleSize] = { Args... };

  static constexpr bool HasDuplicateElements()
  {
    for (size_t i = 0; i < SwizzleSize; i++)
    {
      for (size_t j = i + 1; j < SwizzleSize; j++)
      {
        if (SwizzleMap[i] == SwizzleMap[j]) return true;
      }
    }

    return false;
  }

  static constexpr bool SwizzleNoDups = !HasDuplicateElements();

  T data[Size];

public:
  [[nodiscard]] constexpr T& operator[](size_t index) noexcept
  {
    return data[SwizzleMap[index]];
  }

  [[nodiscard]] constexpr T operator[](size_t index) const noexcept
  {
    return data[SwizzleMap[index]];
  }

  constexpr operator Vec<T, SwizzleSize>() const noexcept
  {
    return Vec<T, SwizzleSize>(data[Args]...);
  }

public:
  constexpr Vec<T, SwizzleSize> operator=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((this->data[Args] = scalar)...);
  }

  constexpr Vec<T, SwizzleSize> operator=(const Vec<T, SwizzleSize>& vec) noexcept requires SwizzleNoDups
  {
    size_t index = 0;
    ((this->data[Args] = vec.data[index++]), ...);
    (void)index;
    return vec;
  }

  constexpr void operator+=(T scalar) noexcept requires SwizzleNoDups
  {
    ((this->data[Args] += scalar), ...);
  }

  constexpr void operator+=(const Vec<T, SwizzleSize>& vec) noexcept requires SwizzleNoDups
  {
    size_t index = 0;
    ((this->data[Args] += vec.data[index++]), ...);
    (void)index;
  }

  constexpr void operator-=(T scalar) noexcept requires SwizzleNoDups
  {
    ((this->data[Args] -= scalar), ...);
  }

  constexpr void operator-=(const Vec<T, SwizzleSize>& vec) noexcept requires SwizzleNoDups
  {
    size_t index = 0;
    ((this->data[Args] -= vec.data[index++]), ...);
    (void)index;
  }

  constexpr void operator*=(T scalar) noexcept requires SwizzleNoDups
  {
    ((this->data[Args] *= scalar), ...);
  }

  constexpr void operator*=(const Vec<T, SwizzleSize>& vec) noexcept requires SwizzleNoDups
  {
    size_t index = 0;
    ((this->data[Args] *= vec.data[index++]), ...);
    (void)index;
  }

  constexpr void operator/=(T scalar) noexcept requires SwizzleNoDups
  {
    ((this->data[Args] /= scalar), ...);
  }

  constexpr void operator/=(const Vec<T, SwizzleSize>& vec) noexcept requires SwizzleNoDups
  {
    size_t index = 0;
    ((this->data[Args] /= vec.data[index++]), ...);
    (void)index;
  }
};

#define SWIZZLE_VEC2_TO_VEC2(Vec, A0, A1) \
  Swizzle<Vec, 0, 0> A0##A0;              \
  Swizzle<Vec, 0, 1> A0##A1;              \
  Swizzle<Vec, 1, 0> A1##A0;              \
  Swizzle<Vec, 1, 1> A1##A1;

} // namespace genebits::engine

#endif
