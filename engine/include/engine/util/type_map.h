#ifndef GENEBITS_ENGINE_UTIL_TYPE_MAP_H
#define GENEBITS_ENGINE_UTIL_TYPE_MAP_H

#include "engine/config/assertion.h"
#include "engine/util/fast_vector.h"
#include "engine/util/meta.h"

namespace genebits::engine
{
///
/// Concept that determines if the type can be used as the value of a type map.
///
/// @tparam Type The type to check
///
template<typename Type>
concept TypeMapValueType = std::is_default_constructible_v<Type> && FastVectorType<Type>;

///
/// Map used to map types to values where the type is the key.
///
/// The key is generated using the Meta util UniqueId method.
///
/// This map is extremely low overhead and performance oriented.
///
/// @tparam Value Value to map types with.
/// @tparam AllocatorImpl Allocator to use to allocate memory.
///
template<TypeMapValueType Value, Allocator AllocatorImpl = Mallocator>
class TypeMap
{
public:
  ///
  /// Safely returns the value reference for the type key.
  ///
  /// If the mapping never existed, this method will make sure that it is created.
  ///
  /// Usually O(1) with very little overhead, but sometimes during creating of the
  /// mapping (only happens once per key) and internal resize must be called.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type>
  Value& Assure() noexcept
  {
    const size_t index = Key<Type>();

    if (values_.Size() <= index) [[unlikely]]
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
    ASSERT(values_.Size() > Key<Type>(), "Type maps out of bounds");

    return values_[Key<Type>()];
  }

private:
  ///
  /// Obtains the key for a type.
  ///
  /// Correctly implements the UniqueId fetching so that there is a new sequence generated
  /// for every value type. This reduces the memory usage.
  ///
  /// @tparam Type The type to obtain key for.
  ///
  /// @return size_t key for the templated type.
  ///
  template<typename Type>
  [[nodiscard]] static size_t Key() noexcept
  {
    return Meta<Type>::template UniqueId<TypeMap<Value, AllocatorImpl>>();
  }

private:
  FastVector<Value, AllocatorImpl> values_;
};

} // namespace genebits::engine

#endif
