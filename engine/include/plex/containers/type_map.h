#ifndef PLEX_CONTAINERS_TYPE_MAP_H
#define PLEX_CONTAINERS_TYPE_MAP_H

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/type_info.h"

namespace plex
{
// clang-format off

///
/// Concept used to determine if a type can be used as an underlying container for a type map.
///
/// The container must be contiguous and resizable.
///
/// @tparam Type The type to check.
/// @tparam Value The value type of the type map.
///
template<typename Type, typename Value>
concept TypeMapContainer = requires(Type& container, size_t size)
{
  { *container.begin() } -> std::same_as<Value&>;
  { container.begin() } -> std::contiguous_iterator;

  { container.size() } -> std::convertible_to<size_t>;
  container.resize(size);
};

// clang-format on

///
/// Container adapter for mapping types to values.
///
/// Uses an efficiently generated dense type index to do lookups into a contiguous array.
///
/// Much faster than using a normal map, the price of a obtaining a value for a type is essentially the cost of an array
/// lookup.
///
/// @see TypeIndex
///
/// @tparam Value Value to map types with.
///
template<typename Value, TypeMapContainer<Value> Container = Vector<Value>>
class TypeMap
{
public:
  ///
  /// Safely returns the value reference for the type key.
  ///
  /// If the mapping never existed, the value will be default constructed.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type>
  Value& Assure()
  requires std::default_initializable<Value>
  {
    const size_t index = Key<Type>();

    if (values_.size() <= index) [[unlikely]]
    {
      ResizeFor(index);
    }

    return std::to_address(values_.begin())[index];
  }

  ///
  /// Sets the value for the type key to the default value.
  ///
  /// This is essentially the way to remove a value from the map. Can be used in combination
  /// with ContainsNonDefault().
  ///
  /// @tparam[in] Type The type to use as key.
  ///
  template<typename Type>
  void SetDefault() noexcept(std::is_nothrow_default_constructible_v<Value>)
  requires std::default_initializable<Value>
  {
    Value* value = std::addressof(Get<Type>());
    value->~Value();
    new (value) Value();
  }

  ///
  /// Returns whether or not a mapping exists and the value is not the default constructed value.
  ///
  /// There is no other way to check if the map contains a type or not. If your values can contain the default
  /// constructed value, you can always wrap the value using an indirection like a pointer, or something like
  /// std::optional.
  ///
  /// @note The value type must be equality comparable to use this method.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return True if the type exists in the map, false otherwise.
  ///
  template<typename Type>
  [[nodiscard]] bool ContainsNonDefault() const noexcept(std::is_nothrow_default_constructible_v<Value>)
  requires std::equality_comparable<Value>
  {
    const auto key = Key<Type>();

    return values_.size() > key && values_.data()[key] != Value {};
  }

  ///
  /// Returns the value reference for the type key.
  ///
  /// @warning
  ///     Make sure Assure was called at least once for this type. Or else
  ///     this method's behaviour is undefined.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type>
  [[nodiscard]] const Value& Get() const noexcept
  {
    const auto key = Key<Type>();

    ASSERT(values_.size() > key, "Type maps out of bounds");

    return std::to_address(values_.begin())[key];
  }

  ///
  /// Returns the value reference for the type key.
  ///
  /// @warning
  ///     Make sure Assure was called at least once for this type. Or else
  ///     this method's behaviour is undefined.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type>
  [[nodiscard]] Value& Get() noexcept
  {
    return const_cast<Value&>(static_cast<const TypeMap*>(this)->template Get<Type>());
  }

private:
  ///
  /// Resizes the internal array to make sure the index is valid.
  ///
  /// @param[in] index The index to resize for.
  ///
  COLD_SECTION NO_INLINE void ResizeFor(const size_t index)
  {
    ASSERT(index < 100000, "To many types, probably a bug"); // Highly unlikely the map exceeds 100k types

    values_.resize(index + 1);
  }

  ///
  /// Obtains the key for a type.
  ///
  /// Uses TypeInfo to generate an identifier for the type from the TypeMap<Value> sequence. Identifiers for the type
  /// are not the same as the ones from the global sequence. This means that the sequence used for the map is less
  /// "polluted" than using a naive approach, resulting in less memory overhead and cache misses.
  ///
  /// @tparam Type The type to obtain key for.
  ///
  /// @return size_t key for the templated type.
  ///
  template<typename Type>
  [[nodiscard]] ALWAYS_INLINE static size_t Key() noexcept
  {
    return TypeIndex<Type, Container>();
  }

private:
  Container values_;
};

template<typename Type, typename Container>
struct IsTriviallyRelocatable<TypeMap<Type, Container>> : public IsTriviallyRelocatable<Container>
{};
} // namespace plex

#endif
