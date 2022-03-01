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
/// Map used to map types to values where the type is the key.
///
/// The key is generated using the Meta util Index method.
///
/// This map is extremely low overhead and performance oriented.
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
  Value& Assure() noexcept
  {
    const size_t index = Key<Type>();

    if (values_.size() <= index) [[unlikely]]
    {
      ASSERT(index < 10000, "To many types"); // Highly unlikely the map exceeds 10k types, probably a bug.

      values_.Resize(index + 1);
    }

    return values_[index];
  }

  ///
  /// Returns the value reference for the type key.
  ///
  /// Always O(1) with extremely low overhead. Essentially an array lookup.
  ///
  /// @warning
  ///     Make sure Assure was called at least once for this type. Or else
  ///     this method is undefined behaviour.
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
  ///     Make sure Assure was called at least once for this type. Otherwise, behaviour is undefined.
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
  /// Obtains the key for a type.
  ///
  /// @tparam Type The type to obtain key for.
  ///
  /// @return size_t key for the templated type.
  ///
  template<typename Type>
  [[nodiscard]] static size_t Key() noexcept
  {
    return TypeInfo<Type>::template Index<TypeMap>();
  }

private:
  Vector<Value> values_;
};

} // namespace genebits::engine

#endif
