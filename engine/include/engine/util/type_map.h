#ifndef GENEBITS_ENGINE_UTIL_TYPE_MAP_H
#define GENEBITS_ENGINE_UTIL_TYPE_MAP_H

#include <vector>

#include "engine/util/meta.h"

namespace genebits::engine
{
///
/// Map used to map types to values where the type is the key.
///
/// The key is generated using the Meta util UniqueId method.
///
/// This map had extremely low overhead and performance oriented.
///
/// @tparam Value The value to map types to.
///
template<typename Value>
class TypeMap
{
public:
  ///
  /// Safely returns the value reference for the type key.
  ///
  /// If the mapping never existed, this method will make sure that it is created.
  ///
  /// Usually O(1) with very little overhead, but sometimes during creating of the
  /// mapping (only happends once per key) and internal resize must be called.
  ///
  /// @tparam Type The type to use as key.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type>
  Value& Assure() noexcept
  {
    const size_t index = Key<Type>();

    if (values_.size() <= index)
    {
      values_.resize(index + 1);
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
    return Meta<Type>::template UniqueId<TypeMap<Value>>();
  }

private:
  std::vector<Value> values_;
};

} // namespace genebits::engine

#endif
