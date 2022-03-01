#ifndef GENEBITS_ENGINE_OS_MEMORY_H
#define GENEBITS_ENGINE_OS_MEMORY_H

#include <cstdint>
#include <cstdlib>
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
inline void Prefetch(void* source)
{
  _mm_prefetch(static_cast<const char*>(source), _MM_HINT_NTA);
}

///
/// Relocates elements into uninitialized memory. Equivalent to moving all elements to new array and calling destructor
/// on old array elements.
///
/// If the type is trivially relocatable, optimizes the relocation by using memcpy/memmove.
///
/// @note Inspired by proposal: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1144r5.html
///
/// @tparam Overlap Whether or not source and destination overlap.
/// @tparam SourceIt Source iterator.
/// @tparam DestIt Destination iterator.
///
/// @param[in] first First iterator to source.
/// @param[in] last Last iterator to source.
/// @param[in] dest Destination iterator.
///
template<bool Overlap = true, typename SourceIt, typename DestIt>
inline void UninitializedRelocate(SourceIt first, SourceIt last, DestIt dest)
{
  using DestType = typename std::iterator_traits<DestIt>::value_type;
  using SourceType = decltype(std::move(*first));

  constexpr bool memcpyable =
    (std::is_same_v<DestType, std::remove_reference_t<SourceType>> && IsTriviallyRelocatable<DestType>::value);
  constexpr bool both_contiguous = (std::is_pointer_v<SourceIt> && std::is_pointer_v<DestIt>);

  if constexpr (memcpyable && both_contiguous)
  {
    const size_t bytes = (char*)last - (char*)first;

    if constexpr (Overlap) std::memmove(std::addressof(*dest), std::addressof(*first), bytes);
    else
    {
      std::memcpy(std::addressof(*dest), std::addressof(*first), bytes);
    }
  }
  else
  {
    for (; first != last; (void)++dest, ++first)
    {
      ::new (static_cast<void*>(std::addressof(*dest))) DestType(std::move(*first));
      std::destroy_at(std::addressof(*first));
    }
  }
}

} // namespace genebits::engine
#endif
