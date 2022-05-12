#ifndef GENEBITS_ENGINE_CONTAINERS_C_ARRAY_H
#define GENEBITS_ENGINE_CONTAINERS_C_ARRAY_H

#include <cstdint>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/type_traits.h"

namespace genebits::engine
{
///
/// Simple alias for readability when working with c arrays.
///
/// @tparam T The type of the array.
/// @tparam N The size of the array.
///
template<typename T, size_t N>
using CArray = T[N];

///
/// Empty type that can be default constructed using braces. Used to specialize for empty c arrays.
///
struct EmptyCArray
{};

namespace details
{
  ///
  /// Array base to get array type.
  ///
  /// @tparam T The type of the array.
  ///
  /// @tparam N The size of the array.
  ///
  template<typename T, size_t N>
  struct ArrayBase
  {
    using CArrayType = CArray<T, N>;
  };

  ///
  /// Array base to get array type.
  ///
  /// Specialization for empty c arrays.
  ///
  /// @tparam T The type of the array.
  ///
  template<typename T>
  struct ArrayBase<T, 0>
  {
    using CArrayType = EmptyCArray;
  };
} // namespace details

///
/// Array built on top of CArray. Similar to std::array, but more compile-time lightweight and allows for empty arrays.
///
/// @tparam T The type of the array.
/// @tparam N The size of the array.
///
template<typename T, size_t N>
class Array : public details::ArrayBase<T, N>
{
public:
  using CArrayType = typename details::ArrayBase<T, N>::CArrayType;
  static constexpr size_t Size = N;

public:
  // Style Exception: STL
  // clang-format off

  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = T;

  using iterator = T*;
  using const_iterator = const T*;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;

  // Forward iterator creation methods.
  [[nodiscard]] constexpr iterator begin() { return reinterpret_cast<iterator>(this); }
  [[nodiscard]] constexpr const_iterator begin() const { return reinterpret_cast<iterator>(this); }
  [[nodiscard]] constexpr iterator end() { return begin() + Size; }
  [[nodiscard]] constexpr const_iterator end() const { return begin() + Size; }

  // Explicit const forward iterator creation methods.
  [[nodiscard]] constexpr const_iterator cbegin() const { return begin(); }
  [[nodiscard]] constexpr const_iterator cend() const { return end(); }

  // Reverse iterator creation methods.
  [[nodiscard]] constexpr reverse_iterator rbegin() { return reverse_iterator(end()); }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  [[nodiscard]] constexpr reverse_iterator rend() { return reverse_iterator(begin()); }
  [[nodiscard]] constexpr const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Internal array accessors
  [[nodiscard]] constexpr pointer data() { return pointer(begin()); }
  [[nodiscard]] constexpr const_pointer data() const { return const_pointer(begin()); }
  [[nodiscard]] constexpr reference front() { return begin()[0]; }
  [[nodiscard]] constexpr const_reference front() const { return begin()[0]; }
  [[nodiscard]] constexpr reference back() { return end()[-1]; }
  [[nodiscard]] constexpr const_reference back() const { return end()[-1]; }

  // clang-format on

  ///
  /// Const array access operator.
  ///
  /// @param[in] index Index to access
  ///
  /// @return Const reference to element at the index.
  ///
  [[nodiscard]] constexpr const T& operator[](const size_type index) const noexcept
  {
    ASSERT(index < Size, "Index out of bounds");
    return data_[index];
  }

  ///
  /// Array access operator.
  ///
  /// @param[in] index Index to access
  ///
  /// @return Reference to element at the index.
  ///
  [[nodiscard]] constexpr T& operator[](const size_type index) noexcept
  {
    ASSERT(index < Size, "Index out of bounds");
    return data_[index];
  }

  ///
  /// Returns the amount of elements currently stored in the vector.
  ///
  /// @return Size of the vector.
  ///
  [[nodiscard]] constexpr size_t size() const noexcept
  {
    return Size;
  }

public:
  ///
  /// Constructor
  ///
  /// Same as CArray construction.
  ///
  /// @tparam Types Variadic template for Type.
  ///
  /// @param[in] args Arguments to construct the array with.
  ///
  template<std::same_as<T>... Types>
  constexpr Array(Types&&... args) noexcept : data_ { std::forward<Types>(args)... }
  {}

private:
  CArrayType data_;
};

///
/// Concatenates two arrays.
///
/// @tparam T The type of the array.
/// @tparam N Size of first array.
/// @tparam M Size of second array.
///
/// @param[in] lhs First array.
/// @param[in] rhs Second array.
///
/// @return Concatenated array.
///
template<typename T, size_t N, size_t M>
constexpr Array<T, N + M> operator+(const Array<T, N>& lhs, const Array<T, M>& rhs) noexcept
{
  Array<T, N + M> result;

  if constexpr (N > 0) std::copy(lhs.begin(), lhs.end(), result.begin());
  if constexpr (M > 0) std::copy(rhs.begin(), rhs.end(), result.begin() + N);

  return result;
}

template<typename Type, size_t N>
struct IsTriviallyRelocatable<Array<Type, N>> : std::bool_constant<IsTriviallyRelocatable<Type>::value>
{};
} // namespace genebits::engine

#endif // GENEBITS_C_ARRAY_H
