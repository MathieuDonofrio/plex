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
  struct ArrayTypeSelector
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
  struct ArrayTypeSelector<T, 0>
  {
    using CArrayType = CArray<T, 1>;
  };
} // namespace details

///
/// Array built on top of CArray. Similar to std::array, but more compile-time lightweight and allows for empty arrays.
///
/// @tparam T The type of the array.
/// @tparam N The size of the array.
///
template<typename T, size_t N>
class Array
{
public:
  using CArrayType = typename details::ArrayTypeSelector<T, N>::CArrayType;
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
  [[nodiscard]] constexpr iterator begin() { return data_; }
  [[nodiscard]] constexpr const_iterator begin() const { return data_; }
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

  constexpr auto operator<=>(const Array&) const = default;

private:
  CArrayType data_;
};

///
/// Concatenates arrays in given order.
///
/// @tparam Type The type of array to concatenate
///
/// @tparam Lengths The lengths of every array to concatenate.
///
/// @param[in] arrays The arrays to concatenate.
///
/// @return Result of concatenation.
///
template<typename Type, size_t... Lengths>
constexpr auto ConcatArrays(const Array<Type, Lengths>&... arrays)
{
  if constexpr (sizeof...(Lengths) != 0)
  {
    constexpr size_t total_length = (... + Lengths);

    Array<Type, total_length> result;

    auto it = result.begin();
    ((it = std::copy(arrays.begin(), arrays.end(), it)), ...);
    (void)it; // maybe unused

    return result;
  }
  else
  {
    return Array<Type, 0> {};
  }
}

template<typename Type, size_t N>
struct IsTriviallyRelocatable<Array<Type, N>> : public std::bool_constant<IsTriviallyRelocatable<Type>::value>
{};
} // namespace genebits::engine

#endif // GENEBITS_C_ARRAY_H
