#ifndef GENEBITS_ENGINE_UTILITIES_META_H
#define GENEBITS_ENGINE_UTILITIES_META_H

#include <string_view>
#include <type_traits>

#include "genebits/engine/config/compiler.h"

namespace fsig // Keep small namespace for faster compile-time.
{
///
/// Returns the function name with a templated type. Can be used to probe type name.
///
/// @tparam Type The type to include in the function name
///
/// @return The function name with the templated type.
///
template<typename Type>
consteval const char* F() // Keep char* return and small name for faster compile-time.
{
#if COMPILER_MSVC
  return __FUNCSIG__;
#else
  return __PRETTY_FUNCTION__;
#endif
}

// Probing technique:
// Get the templated function name of void type and use that to find the start and offset of the type.

constexpr std::string_view cProbe = F<void>();

constexpr size_t cStart = cProbe.find("void");
constexpr size_t cOffset = cProbe.length() - 4;
} // namespace fsig

namespace genebits::engine
{
namespace details
{
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
  size_t TypeIndex(std::string_view type_name, std::string_view tag_name);
} // namespace details

///
/// Templated structure that contains meta information about the templated type.
///
/// @tparam Type The type to access meta information for.
///
template<typename Type>
struct TypeInfo
{
public:
  ///
  /// Returns the full name of the type, this includes namespaces.
  ///
  /// @warning
  ///     Does not return the same result on every compiler. And the name is not
  ///     guaranteed to be human readable.
  ///
  /// @note Const, volatile and references are stripped from the type.
  ///
  /// @return Full name for type.
  ///
  [[nodiscard]] static consteval std::string_view Name() noexcept
  {
    const std::string_view function_name = fsig::F<std::remove_cvref_t<Type>>();

    return function_name.substr(fsig::cStart, function_name.length() - fsig::cOffset);
  }

  ///
  /// Returns a compile-time hash code for the type derived from type name.
  ///
  /// @return Hash code for type.
  ///
  [[nodiscard]] static consteval size_t HashCode() noexcept
  {
    // FNV1a

    const uint64_t prime = 0x100000001b3;

    uint64_t hash = 14695981039346656037ul;

    for (const auto c : Name())
    {
      hash = hash ^ static_cast<uint8_t>(c);
      hash *= prime;
    }

    return static_cast<size_t>(hash);
  }

  ///
  /// Returns an index for the type initialized once at runtime.
  ///
  /// Indexes start at zero and are incremented each time a new type requests an index. This makes the index ideal for
  /// lookup tables.
  ///
  /// An optional tag can be provided to use a difference index sequence. This helps to create more packed index
  /// sequences.
  ///
  /// @tparam Tag Optional tag used to identify the index sequence to use.
  ///
  /// @return Index for type.
  ///
  template<typename Tag = void>
  static size_t Index()
  {
    static const size_t value = engine::details::TypeIndex(Name(), TypeInfo<Tag>::Name());

    return value;
  }
};

} // namespace genebits::engine

#endif