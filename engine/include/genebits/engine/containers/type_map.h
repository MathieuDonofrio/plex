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
concept TypeMapValueType = VectorType<Type>;

///
/// Map used to map types to values where the type is the key.
///
/// The key is generated using the Meta util Index method.
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
  /// Usually O(1). If a value was never initialized for the type this method is expensive
  /// but this happens at most only once per type.
  ///
  /// @tparam Type The type to use as key.
  /// @tparam Args The arguments used to initialize new values.
  ///
  /// @return Reference to the value mapped by the type.
  ///
  template<typename Type, typename... Args>
  requires std::is_constructible_v<Value, Args...> Value& Assure(Args&&... args) noexcept
  {
    const size_t index = Key<Type>();

    if (values_.size() <= index) [[unlikely]]
    {
      ASSERT(index < 10000, "To many types"); // Highly unlikely the map exceeds 10k types, probably a bug.

      values_.Resize(index + 1, std::forward<Args>(args)...);
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
  [[nodiscard]] Value& Get() noexcept
  {
    return const_cast<Value&>(static_cast<const TypeMap*>(this)->template Get<Type>());
  }

private:
  ///
  /// Obtains the key for a type.
  ///
  /// Correctly implements the Index fetching so that there is a new sequence generated
  /// for every value type. This reduces the memory usage.
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
  Vector<Value, AllocatorImpl> values_;
};

} // namespace genebits::engine

#endif
