#ifndef GENEBITS_ENGINE_MATH_SWIZZLE_H
#define GENEBITS_ENGINE_MATH_SWIZZLE_H

#include <cstdlib>
#include <type_traits>

namespace genebits::engine
{

///
/// A class that provides a way to access a vector's components in a different order.
///
/// @tparam Type The type of the vector's components.
/// @tparam Args The new order of the vector's components.
///
template<typename Type, size_t... Args>
struct Swizzle;

#define SWIZZLE_APPLY_VEC_OP(vec, op)     \
  size_t index = 0;                       \
  ((data()[Args] op(vec)[index++]), ...); \
  (void)index;

///
/// A class that provides a way to access a vector's components in a different order.
///
/// @tparam Vec The type of the vector.
/// @tparam T The type of the vector's components.
/// @tparam Size The size of the vector.
/// @tparam Args The new order of the vector's components.
///
template<template<typename, size_t> class Vec, typename T, size_t Size, size_t... Args>
struct Swizzle<Vec<T, Size>, Args...>
{
private:
  static constexpr size_t SwizzleSize = sizeof...(Args);
  static constexpr size_t SwizzleMap[SwizzleSize] = { Args... };

  ///
  /// Checks if the swizzle contains a duplicate value.
  ///
  /// @returns true if the swizzle contains a duplicate value, false otherwise.
  ///
  static constexpr bool HasDuplicateElements() noexcept
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

  ///
  /// Returns a pointer to the vector's data.
  ///
  /// Swizzle must be declared as the first non-addressable element of the vector.
  ///
  /// @return Pointer to the vector's data.
  ///
  const T* data() const noexcept
  {
    return reinterpret_cast<const T*>(this);
  }

  ///
  /// Returns a pointer to the vector's data.
  ///
  /// Swizzle must be declared as the first non-addressable element of the vector.
  ///
  /// @return Pointer to the vector's data.
  ///
  T* data() noexcept
  {
    return reinterpret_cast<T*>(this);
  }

public:
  ///
  /// Accesses the vector's components in the new order.
  ///
  /// @returns Reference to the component at the specified index.
  ///
  [[nodiscard]] T& operator[](size_t index) noexcept
  {
    return data()[SwizzleMap[index]];
  }

  ///
  /// Accesses the vector's components in the new order.
  ///
  /// @returns Reference to the component at the specified index.
  ///
  [[nodiscard]] T operator[](size_t index) const noexcept
  {
    return data()[SwizzleMap[index]];
  }

  ///
  /// Convert the swizzle to a vector.
  ///
  /// @returns The vector.
  ///
  operator Vec<T, SwizzleSize>() const noexcept
  {
    return Vec<T, SwizzleSize>(data()[Args]...);
  }

public:
  ///
  /// Assigns all components of the vector to the specified value and returns a copy of the vector.
  ///
  /// @param[in] value The value to assign to all components.
  ///
  /// @returns A copy of the vector with all components assigned to the specified value.
  ///
  Vec<T, SwizzleSize> operator=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((data()[Args] = scalar)...);
  }

  ///
  /// Assigns the vector's components to the specified vector's components and returns a copy of the vector.
  ///
  /// @param[in] vec The vector to assign to the vector's components.
  ///
  /// @returns A copy of the vector with its components assigned to the specified vector's components.
  ///
  Vec<T, SwizzleSize> operator=(Vec<T, SwizzleSize> vec) noexcept requires SwizzleNoDups
  {
    SWIZZLE_APPLY_VEC_OP(vec, =);
    return vec;
  }

  ///
  /// Adds the specified value to the vector's components and returns a copy of the vector.
  ///
  /// @param[in] value The value to add to the vector's components.
  ///
  /// @returns A copy of the vector with its components incremented by the specified value.
  ///
  Vec<T, SwizzleSize> operator+=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((data()[Args] += scalar)...);
  }

  ///
  /// Adds the specified vector's components to the vector's components and returns a copy of the vector.
  ///
  /// @param[in] vec The vector to add to the vector's components.
  ///
  /// @returns A copy of the vector with its components incremented by the specified vector's components.
  ///
  Vec<T, SwizzleSize> operator+=(Vec<T, SwizzleSize> vec) noexcept requires SwizzleNoDups
  {
    SWIZZLE_APPLY_VEC_OP(vec, +=);
    return vec;
  }

  ///
  /// Subtracts the specified value from the vector's components and returns a copy of the vector.
  ///
  /// @param[in] value The value to subtract from the vector's components.
  ///
  /// @returns A copy of the vector with its components decremented by the specified value.
  ///
  Vec<T, SwizzleSize> operator-=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((data()[Args] -= scalar)...);
  }

  ///
  /// Subtracts the specified vector's components from the vector's components and returns a copy of the vector.
  ///
  /// @param[in] vec The vector to subtract from the vector's components.
  ///
  /// @returns A copy of the vector with its components decremented by the specified vector's components.
  ///
  Vec<T, SwizzleSize> operator-=(Vec<T, SwizzleSize> vec) noexcept requires SwizzleNoDups
  {
    SWIZZLE_APPLY_VEC_OP(vec, -=);
    return vec;
  }

  ///
  /// Multiplies the vector's components by the specified value and returns a copy of the vector.
  ///
  /// @param[in] value The value to multiply the vector's components by.
  ///
  /// @returns A copy of the vector with its components multiplied by the specified value.
  ///
  Vec<T, SwizzleSize> operator*=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((data()[Args] *= scalar)...);
  }

  ///
  /// Multiplies the vector's components by the specified vector's components and returns a copy of the vector.
  ///
  /// @param[in] vec The vector to multiply the vector's components by.
  ///
  /// @returns A copy of the vector with its components multiplied by the specified vector's components.
  ///
  Vec<T, SwizzleSize> operator*=(Vec<T, SwizzleSize> vec) noexcept requires SwizzleNoDups
  {
    SWIZZLE_APPLY_VEC_OP(vec, *=);
    return vec;
  }

  ///
  /// Divides the vector's components by the specified value and returns a copy of the vector.
  ///
  /// @param[in] value The value to divide the vector's components by.
  ///
  /// @returns A copy of the vector with its components divided by the specified value.
  ///
  Vec<T, SwizzleSize> operator/=(T scalar) noexcept requires SwizzleNoDups
  {
    return Vec<T, SwizzleSize>((data()[Args] /= scalar)...);
  }

  ///
  /// Divides the vector's components by the specified vector's components and returns a copy of the vector.
  ///
  /// @param[in] vec The vector to divide the vector's components by.
  ///
  /// @returns A copy of the vector with its components divided by the specified vector's components.
  ///
  Vec<T, SwizzleSize> operator/=(Vec<T, SwizzleSize> vec) noexcept requires SwizzleNoDups
  {
    SWIZZLE_APPLY_VEC_OP(vec, /=);
    return vec;
  }
};

#undef SWIZZLE_APPLY_VEC_OP

#define SWIZZLE_VEC2_TO_VEC2(Vec, A0, A1)          \
  [[no_unique_address]] Swizzle<Vec, 0, 0> A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1> A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0> A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1> A1##A1;

#define SWIZZLE_VEC2_TO_VEC3(Vec, A0, A1)                 \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0> A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1> A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0> A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1> A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0> A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1> A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0> A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1> A1##A1##A1;

#define SWIZZLE_VEC2_TO_VEC4(Vec, A0, A1)                        \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 0> A0##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 1> A0##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 0> A0##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 1> A0##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 0> A0##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 1> A0##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 0> A0##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 1> A0##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 0> A1##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 1> A1##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 0> A1##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 1> A1##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 0> A1##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 1> A1##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 0> A1##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 1> A1##A1##A1##A1;

#define SWIZZLE_VEC3_TO_VEC2(Vec, A0, A1, A2)      \
  [[no_unique_address]] Swizzle<Vec, 0, 0> A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1> A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2> A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0> A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1> A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2> A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0> A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1> A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2> A2##A2;

#define SWIZZLE_VEC3_TO_VEC3(Vec, A0, A1, A2)             \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0> A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1> A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2> A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0> A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1> A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2> A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0> A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1> A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2> A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0> A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1> A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2> A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0> A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1> A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2> A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0> A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1> A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2> A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0> A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1> A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2> A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0> A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1> A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2> A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0> A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1> A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2> A2##A2##A2;

#define SWIZZLE_VEC3_TO_VEC4(Vec, A0, A1, A2)                    \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 0> A0##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 1> A0##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 2> A0##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 0> A0##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 1> A0##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 2> A0##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 0> A0##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 1> A0##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 2> A0##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 0> A0##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 1> A0##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 2> A0##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 0> A0##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 1> A0##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 2> A0##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 0> A0##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 1> A0##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 2> A0##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 0> A0##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 1> A0##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 2> A0##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 0> A0##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 1> A0##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 2> A0##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 0> A0##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 1> A0##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 2> A0##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 0> A1##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 1> A1##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 2> A1##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 0> A1##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 1> A1##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 2> A1##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 0> A1##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 1> A1##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 2> A1##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 0> A1##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 1> A1##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 2> A1##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 0> A1##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 1> A1##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 2> A1##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 0> A1##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 1> A1##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 2> A1##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 0> A1##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 1> A1##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 2> A1##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 0> A1##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 1> A1##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 2> A1##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 0> A1##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 1> A1##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 2> A1##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 0> A2##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 1> A2##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 2> A2##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 0> A2##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 1> A2##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 2> A2##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 0> A2##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 1> A2##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 2> A2##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 0> A2##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 1> A2##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 2> A2##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 0> A2##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 1> A2##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 2> A2##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 0> A2##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 1> A2##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 2> A2##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 0> A2##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 1> A2##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 2> A2##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 0> A2##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 1> A2##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 2> A2##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 0> A2##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 1> A2##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 2> A2##A2##A2##A2;

#define SWIZZLE_VEC4_TO_VEC2(Vec, A0, A1, A2, A3)  \
  [[no_unique_address]] Swizzle<Vec, 0, 0> A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1> A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2> A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3> A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0> A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1> A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2> A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3> A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0> A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1> A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2> A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3> A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0> A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1> A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2> A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3> A3##A3;

#define SWIZZLE_VEC4_TO_VEC3(Vec, A0, A1, A2, A3)         \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0> A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1> A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2> A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 3> A0##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0> A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1> A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2> A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 3> A0##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0> A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1> A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2> A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 3> A0##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 0> A0##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 1> A0##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 2> A0##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 3> A0##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0> A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1> A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2> A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 3> A1##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0> A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1> A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2> A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 3> A1##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0> A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1> A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2> A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 3> A1##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 0> A1##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 1> A1##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 2> A1##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 3> A1##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0> A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1> A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2> A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 3> A2##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0> A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1> A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2> A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 3> A2##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0> A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1> A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2> A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 3> A2##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 0> A2##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 1> A2##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 2> A2##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 3> A2##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 0> A3##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 1> A3##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 2> A3##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 3> A3##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 0> A3##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 1> A3##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 2> A3##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 3> A3##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 0> A3##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 1> A3##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 2> A3##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 3> A3##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 0> A3##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 1> A3##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 2> A3##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 3> A3##A3##A3;

#define SWIZZLE_VEC4_TO_VEC4(Vec, A0, A1, A2, A3)                \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 0> A0##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 1> A0##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 2> A0##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 0, 3> A0##A0##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 0> A0##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 1> A0##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 2> A0##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 1, 3> A0##A0##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 0> A0##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 1> A0##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 2> A0##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 2, 3> A0##A0##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 3, 0> A0##A0##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 3, 1> A0##A0##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 3, 2> A0##A0##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 0, 3, 3> A0##A0##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 0> A0##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 1> A0##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 2> A0##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 0, 3> A0##A1##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 0> A0##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 1> A0##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 2> A0##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 1, 3> A0##A1##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 0> A0##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 1> A0##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 2> A0##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 2, 3> A0##A1##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 3, 0> A0##A1##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 3, 1> A0##A1##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 3, 2> A0##A1##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 1, 3, 3> A0##A1##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 0> A0##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 1> A0##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 2> A0##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 0, 3> A0##A2##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 0> A0##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 1> A0##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 2> A0##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 1, 3> A0##A2##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 0> A0##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 1> A0##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 2> A0##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 2, 3> A0##A2##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 3, 0> A0##A2##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 3, 1> A0##A2##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 3, 2> A0##A2##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 2, 3, 3> A0##A2##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 0, 0> A0##A3##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 0, 1> A0##A3##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 0, 2> A0##A3##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 0, 3> A0##A3##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 1, 0> A0##A3##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 1, 1> A0##A3##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 1, 2> A0##A3##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 1, 3> A0##A3##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 2, 0> A0##A3##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 2, 1> A0##A3##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 2, 2> A0##A3##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 2, 3> A0##A3##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 3, 0> A0##A3##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 3, 1> A0##A3##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 3, 2> A0##A3##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 0, 3, 3, 3> A0##A3##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 0> A1##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 1> A1##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 2> A1##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 0, 3> A1##A0##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 0> A1##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 1> A1##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 2> A1##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 1, 3> A1##A0##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 0> A1##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 1> A1##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 2> A1##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 2, 3> A1##A0##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 3, 0> A1##A0##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 3, 1> A1##A0##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 3, 2> A1##A0##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 0, 3, 3> A1##A0##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 0> A1##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 1> A1##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 2> A1##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 0, 3> A1##A1##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 0> A1##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 1> A1##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 2> A1##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 1, 3> A1##A1##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 0> A1##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 1> A1##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 2> A1##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 2, 3> A1##A1##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 3, 0> A1##A1##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 3, 1> A1##A1##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 3, 2> A1##A1##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 1, 3, 3> A1##A1##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 0> A1##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 1> A1##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 2> A1##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 0, 3> A1##A2##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 0> A1##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 1> A1##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 2> A1##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 1, 3> A1##A2##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 0> A1##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 1> A1##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 2> A1##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 2, 3> A1##A2##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 3, 0> A1##A2##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 3, 1> A1##A2##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 3, 2> A1##A2##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 2, 3, 3> A1##A2##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 0, 0> A1##A3##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 0, 1> A1##A3##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 0, 2> A1##A3##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 0, 3> A1##A3##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 1, 0> A1##A3##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 1, 1> A1##A3##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 1, 2> A1##A3##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 1, 3> A1##A3##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 2, 0> A1##A3##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 2, 1> A1##A3##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 2, 2> A1##A3##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 2, 3> A1##A3##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 3, 0> A1##A3##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 3, 1> A1##A3##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 3, 2> A1##A3##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 1, 3, 3, 3> A1##A3##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 0> A2##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 1> A2##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 2> A2##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 0, 3> A2##A0##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 0> A2##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 1> A2##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 2> A2##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 1, 3> A2##A0##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 0> A2##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 1> A2##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 2> A2##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 2, 3> A2##A0##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 3, 0> A2##A0##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 3, 1> A2##A0##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 3, 2> A2##A0##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 0, 3, 3> A2##A0##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 0> A2##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 1> A2##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 2> A2##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 0, 3> A2##A1##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 0> A2##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 1> A2##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 2> A2##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 1, 3> A2##A1##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 0> A2##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 1> A2##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 2> A2##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 2, 3> A2##A1##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 3, 0> A2##A1##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 3, 1> A2##A1##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 3, 2> A2##A1##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 1, 3, 3> A2##A1##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 0> A2##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 1> A2##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 2> A2##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 0, 3> A2##A2##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 0> A2##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 1> A2##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 2> A2##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 1, 3> A2##A2##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 0> A2##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 1> A2##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 2> A2##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 2, 3> A2##A2##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 3, 0> A2##A2##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 3, 1> A2##A2##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 3, 2> A2##A2##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 2, 3, 3> A2##A2##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 0, 0> A2##A3##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 0, 1> A2##A3##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 0, 2> A2##A3##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 0, 3> A2##A3##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 1, 0> A2##A3##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 1, 1> A2##A3##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 1, 2> A2##A3##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 1, 3> A2##A3##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 2, 0> A2##A3##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 2, 1> A2##A3##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 2, 2> A2##A3##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 2, 3> A2##A3##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 3, 0> A2##A3##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 3, 1> A2##A3##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 3, 2> A2##A3##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 2, 3, 3, 3> A2##A3##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 0, 0> A3##A0##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 0, 1> A3##A0##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 0, 2> A3##A0##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 0, 3> A3##A0##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 1, 0> A3##A0##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 1, 1> A3##A0##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 1, 2> A3##A0##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 1, 3> A3##A0##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 2, 0> A3##A0##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 2, 1> A3##A0##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 2, 2> A3##A0##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 2, 3> A3##A0##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 3, 0> A3##A0##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 3, 1> A3##A0##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 3, 2> A3##A0##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 0, 3, 3> A3##A0##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 0, 0> A3##A1##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 0, 1> A3##A1##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 0, 2> A3##A1##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 0, 3> A3##A1##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 1, 0> A3##A1##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 1, 1> A3##A1##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 1, 2> A3##A1##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 1, 3> A3##A1##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 2, 0> A3##A1##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 2, 1> A3##A1##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 2, 2> A3##A1##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 2, 3> A3##A1##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 3, 0> A3##A1##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 3, 1> A3##A1##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 3, 2> A3##A1##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 1, 3, 3> A3##A1##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 0, 0> A3##A2##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 0, 1> A3##A2##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 0, 2> A3##A2##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 0, 3> A3##A2##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 1, 0> A3##A2##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 1, 1> A3##A2##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 1, 2> A3##A2##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 1, 3> A3##A2##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 2, 0> A3##A2##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 2, 1> A3##A2##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 2, 2> A3##A2##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 2, 3> A3##A2##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 3, 0> A3##A2##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 3, 1> A3##A2##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 3, 2> A3##A2##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 2, 3, 3> A3##A2##A3##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 0, 0> A3##A3##A0##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 0, 1> A3##A3##A0##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 0, 2> A3##A3##A0##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 0, 3> A3##A3##A0##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 1, 0> A3##A3##A1##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 1, 1> A3##A3##A1##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 1, 2> A3##A3##A1##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 1, 3> A3##A3##A1##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 2, 0> A3##A3##A2##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 2, 1> A3##A3##A2##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 2, 2> A3##A3##A2##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 2, 3> A3##A3##A2##A3; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 3, 0> A3##A3##A3##A0; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 3, 1> A3##A3##A3##A1; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 3, 2> A3##A3##A3##A2; \
  [[no_unique_address]] Swizzle<Vec, 3, 3, 3, 3> A3##A3##A3##A3;
} // namespace genebits::engine

#endif
