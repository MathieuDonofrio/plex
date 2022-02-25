#ifndef GENEBITS_ENGINE_UTILITIES_CONCEPTS_H
#define GENEBITS_ENGINE_UTILITIES_CONCEPTS_H

#include <concepts>
#include <type_traits>

namespace genebits::engine
{
///
/// Integer concept that checks if the size is a power of two.
///
/// @tparam Size The size to test
///
template<size_t Size>
concept POT = Size != 0 && (Size & (Size - 1)) == 0;

///
/// Concept that check if the type is plain old data.
///
/// @tparam Type Type to test.
///
template<typename Type>
concept POD = std::is_pod_v<Type>;

namespace details
{
  template<typename... Types>
  struct UniqueTypesImpl : std::true_type
  {};

  template<typename Type, typename... Types>
  struct UniqueTypesImpl<Type, Types...>
    : std::conjunction<std::negation<std::disjunction<std::is_same<Type, Types>...>>, UniqueTypesImpl<Types...>>
  {};
} // namespace details

///
/// Checks if all types are unique
///
/// Types are compared using std::is_same.
///
/// @tparam Types The types to check
///
template<typename... Types>
concept UniqueTypes = details::UniqueTypesImpl<Types...>::value;

} // namespace genebits::engine

#endif
