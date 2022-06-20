#ifndef GENEBITS_ENGINE_CONTAINERS_TYPE_MAP_H
#define GENEBITS_ENGINE_CONTAINERS_TYPE_MAP_H

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/type_info.h"

namespace genebits::engine
{
///
/// Concept that determines if the type can be used as the value of a type map.
///
/// @tparam Type The type to check
///
template<typename Type>
concept TypeMapValueType = VectorType<Type> && std::is_default_constructible_v<Type>;

///
/// Maps types to values where the type is used efficiently generate a runtime key based off the type name.
///
/// The key is obtained from type using TypeInfo unique identifiers.
///
/// Much faster than using a normal map when a type is required as a key.
///
/// @tparam Value Value to map types with.
///
template<TypeMapValueType Value>
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
  {
    const size_t index = Key<Type>();

    if (values_.size() <= index) [[unlikely]]
    {
      ASSERT(index < 100000, "To many types, probably a bug"); // Highly unlikely the map exceeds 100k types

      values_.Resize(index + 1);
    }

    return values_[index];
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
  void SetDefault() noexcept(noexcept(Value()))
  {
    new (std::addressof(Get<Type>())) Value();
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
  [[nodiscard]] bool ContainsNonDefault() const noexcept
  requires std::equality_comparable<Value>
  {
    const auto key = Key<Type>();

    return values_.size() > key && values_[key] != cDefaultValue;
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
    ASSERT(values_.size() > Key<Type>(), "Type maps out of bounds");

    return values_[Key<Type>()];
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
  using DefaultValueType = std::conditional_t<std::equality_comparable<Value>, Value, void*>;

  static inline const DefaultValueType cDefaultValue {};

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
  [[nodiscard]] static size_t Key() noexcept
  {
    return TypeIndex<Type, TypeMap>();
  }

private:
  Vector<Value> values_;
};

template<typename Type>
struct IsTriviallyRelocatable<TypeMap<Type>> : public IsTriviallyRelocatable<Vector<Type>>
{};
} // namespace genebits::engine

#endif
