#ifndef GENEBITS_ENGINE_UTILITIES_META_H
#define GENEBITS_ENGINE_UTILITIES_META_H

#include <string_view>
#include <type_traits>

#include "genebits/engine/config/compiler.h"

///
/// Returns the function name with a templated type. Can be used to probe type name.
///
/// @tparam T The type to include in the function name
///
/// @return The function name with the templated type.
///
template<typename T>
consteval auto _F() // Keep small to avoid overhead
{
#if COMPILER_MSVC
  return __FUNCSIG__;
#else
  return __PRETTY_FUNCTION__;
#endif
}

namespace genebits::engine
{
namespace void_probe
{
  // Probing technique:
  // Use a known type to obtain information about how to extract the type from the signature.

  constexpr std::string_view cFuncSig = _F<void>();

  constexpr size_t cStart = cFuncSig.find("void");
  constexpr size_t cOffset = cFuncSig.length() - 4;
} // namespace void_probe

///
/// Returns the full name of the type, this includes namespaces.
///
/// @warning
///     Does not return the same result on every compiler. And the name is not
///     guaranteed to be human readable.
///
/// @note Const, volatile and references are stripped from the type.
///
/// @tparam Type The type to obtain type name from.
///
/// @return Full name for type.
///
template<typename Type>
[[nodiscard]] static consteval std::string_view TypeName() noexcept
{
  const std::string_view function_name = _F<std::remove_cvref_t<Type>>();

  return function_name.substr(void_probe::cStart, function_name.length() - void_probe::cOffset);
}

///
/// Returns a compile-time hash code for the type derived from type name.
///
/// @tparam Type The type to obtain type hash for.
///
/// @return Hash code for type.
///
template<typename Type>
[[nodiscard]] static consteval size_t TypeHash() noexcept
{
  // FNV1a

  const uint64_t prime = 0x100000001b3;

  uint64_t hash = 14695981039346656037ul;

  for (const auto c : TypeName<Type>())
  {
    hash = hash ^ static_cast<uint8_t>(c);
    hash *= prime;
  }

  return static_cast<size_t>(hash);
}

namespace details
{
  ///
  /// Holds the type index for a type & tag in global storage initialized during runtime.
  ///
  /// @tparam Type The type to get index storage for
  /// @tparam Tag The tag used to identify the index sequence with.
  ///
  template<typename Type, typename Tag>
  struct TypeIndexGlobalStorage
  {
    static const size_t value;
  };

  ///
  /// Returns a unique index for the type name and a tag. Indexes obtained from the same tag use the same index
  /// sequence.
  ///
  /// Indexes are provided at runtime in a first come first serve order.
  ///
  /// @param[in] type_name The name of the type.
  /// @param[in] tag_name Tag used to identify the index sequence.
  ///
  /// @return size_t The unique id for the type name and sequence.
  ///
  COLD_SECTION NO_INLINE size_t TypeIndex(std::string_view type_name, std::string_view tag_name);

  ///
  /// Returns a unique index for the type and a tag. Indexes obtained from the same tag use the same index
  /// sequence.
  ///
  /// @tparam Type The type to get index for.
  /// @tparam Tag The tag to obtain the index sequence from.
  ///
  /// @return size_t The unique id for the type and sequence.
  ///
  template<typename Type, typename Tag>
  size_t TypeIndex()
  {
    return TypeIndex(TypeName<Type>(), TypeName<Tag>());
  }

  template<typename Type, typename Tag>
  const size_t TypeIndexGlobalStorage<Type, Tag>::value = details::TypeIndex<Type, Tag>();
} // namespace details

///
/// Returns an index for the type initialized once at runtime.
///
/// Indexes start at zero and are incremented each time a new type requests an index. This makes the index ideal for
/// lookup tables.
///
/// An optional tag can be provided to use a difference index sequence. This helps to create more packed index
/// sequences.
///
/// @tparam Type The type to obtain index for.
/// @tparam Tag Optional tag used to identify the index sequence to use.
///
/// @return Index for type.
///
template<typename Type, typename Tag = void>
ALWAYS_INLINE static size_t TypeIndex() noexcept
{
  return details::TypeIndexGlobalStorage<Type, Tag>::value;
}

} // namespace genebits::engine

#endif