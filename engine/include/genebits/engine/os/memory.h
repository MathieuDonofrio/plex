#ifndef GENEBITS_ENGINE_OS_MEMORY_H
#define GENEBITS_ENGINE_OS_MEMORY_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>
#include <memory>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/type_traits.h"

namespace genebits::engine
{
///
/// Tries to put memory in cache.
///
/// @param[in] source Memory address.
///
constexpr void Prefetch(void* source)
{
  if (!std::is_constant_evaluated())
  {
    // We should always use NTA
    _mm_prefetch(static_cast<const char*>(source), _MM_HINT_NTA);
  }
}

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
constexpr void RelocateAt(Type* src, Type* dst)
{
  if (std::is_constant_evaluated() || !IsTriviallyRelocatable<Type>::value)
  {
    ::new (static_cast<void*>(dst)) Type(std::move(*src));
    src->~Type();
  }
  else if constexpr (IsTriviallyRelocatable<Type>::value)
  {
    std::memmove(dst, src, sizeof(Type));
  }
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
template<typename Type>
constexpr void UninitializedRelocate(Type* first, Type* last, Type* dest)
{
  if (std::is_constant_evaluated() || !IsTriviallyRelocatable<Type>::value)
  {
    for (; first != last; (void)++dest, ++first)
    {
      ::new (static_cast<void*>(dest)) Type(std::move(*first));
      first->~Type();
    }
  }
  else if constexpr (IsTriviallyRelocatable<Type>::value)
  {
    const size_t bytes = reinterpret_cast<char*>(last) - reinterpret_cast<char*>(first);

    std::memmove(dest, first, bytes);
  }
}

///
/// Relocates the range of elements backwards into uninitialized memory. Equivalent to moving all source elements to the
/// destination then destroying the source elements.
///
/// Optimizes destructive move with bitwise memory copy when possible.
///
/// @tparam Type Type to relocate.
///
/// @param[in] first First iterator to source.
/// @param[in] last Last iterator to source.
/// @param[in] dest Destination iterator (points to end of destination).
///
template<typename Type>
constexpr void UninitializedRelocateBackwards(Type* first, Type* last, Type* dest)
{
  if (std::is_constant_evaluated() || !IsTriviallyRelocatable<Type>::value)
  {
    --dest;
    --last;
    --first;

    for (; first != last; (void)--dest, --last)
    {
      ::new (static_cast<void*>(dest)) Type(std::move(*last));
      last->~Type();
    }
  }
  else if constexpr (IsTriviallyRelocatable<Type>::value)
  {
    const size_t bytes = reinterpret_cast<char*>(last) - reinterpret_cast<char*>(first);

    std::memmove(reinterpret_cast<char*>(dest) - bytes, first, bytes);
  }
}

} // namespace genebits::engine
#endif
