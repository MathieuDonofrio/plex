#ifndef GENEBITS_ENGINE_UTIL_CONCEPTS_H
#define GENEBITS_ENGINE_UTIL_CONCEPTS_H

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

} // namespace genebits::engine

#endif
