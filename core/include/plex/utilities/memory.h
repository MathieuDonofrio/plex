#ifndef PLEX_UTILITIES_MEMORY_H
#define PLEX_UTILITIES_MEMORY_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>
#include <memory>

#include "plex/config/compiler.h"
#include "plex/debug/assertion.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
///
/// Relocates an element into uninitialized memory. Equivalent to moving source to destination then destroying source.
///
/// Optimizes destructive move with bitwise memory copy when possible.
///
/// @note Inspired by proposal: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1144r5.html
///
/// @tparam Type Type to relocate.
///
/// @param[in] src Source.
/// @param[in] dst Destionation.
///
template<class Type>
constexpr Type* RelocateAt(Type* src, Type* dst)
{
  if constexpr (IsTriviallyRelocatable<Type>::value)
  {
    if (!std::is_constant_evaluated())
    {
      std::memmove(dst, src, sizeof(Type));
      return std::launder(dst);
    }
  }

  Type* result = ::new (static_cast<void*>(dst)) Type(std::move(*src));
  src->~Type();
  return result;
}

///
/// Relocates the range of elements into uninitialized memory. Equivalent to moving all source elements to the
/// destination then destroying the source elements.
///
/// Optimizes destructive move with bitwise memory copy when possible.
///
/// @note Inspired by proposal: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1144r5.html
///
/// @tparam Type Type to relocate.
///
/// @param[in] first First iterator to source.
/// @param[in] last Last iterator to source.
/// @param[in] dest Destination iterator.
///
/// @return The iterator pointing to the last element relocated.
///
template<std::forward_iterator Iterator1, std::forward_iterator Iterator2>
constexpr Iterator2 UninitializedRelocate(Iterator1 first, Iterator1 last, Iterator2 dest)
{
  using Type = typename std::iterator_traits<Iterator2>::value_type;
  using RelocateType = decltype(std::move(*first));

  constexpr bool relocatable = std::is_same_v<Type, std::remove_reference_t<RelocateType>>
                               && IsTriviallyRelocatable<Type>::value
                               && std::is_pointer_v<Iterator1> && std::is_pointer_v<Iterator2>;

  if constexpr (relocatable)
  {
    if (!std::is_constant_evaluated())
    {
      const size_t bytes = reinterpret_cast<char*>(last) - reinterpret_cast<char*>(first);

      if (bytes != 0)
      {
        std::memmove(std::addressof(*dest), std::addressof(*first), bytes);
        dest += (last - first);
      }

      return dest;
    }
  }

  for (; first != last; ++dest, ++first)
  {
    ::new (static_cast<void*>(std::addressof(*dest))) Type(std::move(*first));
    std::destroy_at(std::addressof(*first));
  }

  return dest;
}

} // namespace plex
#endif
