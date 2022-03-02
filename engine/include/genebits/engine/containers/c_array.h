#ifndef GENEBITS_ENGINE_CONTAINERS_C_ARRAY_H
#define GENEBITS_ENGINE_CONTAINERS_C_ARRAY_H

#include <cstdint>

namespace genebits::engine
{
///
/// Simple alias for readability when working with c arrays.
///
template<typename Type, std::size_t Size>
using CArray = Type[Size];

///
/// Empty type that can be default constructed using braces. Used to specialize for empty c arrays.
///
struct EmptyCArray
{};
} // namespace genebits::engine

#endif // GENEBITS_C_ARRAY_H
