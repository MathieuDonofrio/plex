#ifndef PLEX_CONTAINERS_C_ARRAY_H
#define PLEX_CONTAINERS_C_ARRAY_H

namespace plex
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
} // namespace plex

#endif
