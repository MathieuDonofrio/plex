#ifndef GENEBITS_ENGINE_UTIL_META_H
#define GENEBITS_ENGINE_UTIL_META_H

#include <string_view>
#include <type_traits>

#include "genebits/engine/config/compiler.h"

namespace genebits::engine
{
namespace
{
  ///
  /// Returns the function name with a templated type. Can be used to probe type name.
  ///
  /// @tparam Type The type to include in the function name
  ///
  /// @return The function name with the templated type.
  ///
  template<typename Type>
  consteval std::string_view TemplatedFunctionName()
  {
#if COMPILER_MSVC
    return __FUNCSIG__;
#else
    return __PRETTY_FUNCTION__;
#endif
  }

  // Probing technique:
  // Get the templated function name of void type and use that to find the start and offset of the type.

  namespace probing
  {
    constexpr std::string_view cProbe = TemplatedFunctionName<void>();

    constexpr size_t cStart = cProbe.find("void");
    constexpr size_t cOffset = cProbe.length() - 4;
  } // namespace probing

} // namespace

///
/// Returns a unique id for the type name and sequence at runtime.
///
/// UniqueId's are packed, therefore they are ideal for lookup tables. Id's are incrementally
/// distributed in a first come first serve fashion.
///
/// UniqueId's are distributed from sequences. You can specify the sequence index (Usually a hash).
///
/// @param[in] full_name The name of the type.
/// @param[in] sequence_index An index used as an identifier to a sequence.
///
/// @return size_t The unique id for the type name and sequence.
///
size_t UniqueId(std::string_view full_name, size_t sequence_index);

///
/// Templated structure that contains meta information about the templated type.
///
/// @tparam Type The type to access meta information for.
///
template<typename Type>
struct Meta
{
public:
  ///
  /// Returns the full name of the type, this includes namespaces.
  ///
  /// @example MyNamespace::MyType
  ///
  /// @return Full name of templated type.
  ///
  [[nodiscard]] static consteval std::string_view FullName() noexcept
  {
    std::string_view full_func_name = TemplatedFunctionName<Type>();
    full_func_name = full_func_name.substr(probing::cStart, full_func_name.length() - probing::cOffset);

    const size_t off = full_func_name.find_last_of(' '); // MSVC adds some extra stuff separated by a space

    return off == std::string_view::npos ? full_func_name : full_func_name.substr(off + 1);
  }

  ///
  /// Returns the name of the type.
  ///
  /// @example MyType
  ///
  /// @return Name of templated type.
  ///
  [[nodiscard]] static consteval std::string_view Name() noexcept
  {
    const std::string_view full_name = FullName();

    const size_t off = full_name.find_last_of(':');

    return off == std::string_view::npos ? full_name : full_name.substr(off + 1);
  }

  ///
  /// Returns the hash of the type based on its full name.
  ///
  /// Hashing is done at compile-time so it can be used in a constexpr context.
  ///
  /// @note FNV1a algorithm.
  ///
  /// @warning There may be collisions.
  ///
  /// @return Type Hash of type.
  ///
  [[nodiscard]] static consteval size_t Hash() noexcept
  {
    const uint64_t prime = 0x100000001b3;

    uint64_t hash = 14695981039346656037ul;

    for (const auto c : FullName())
    {
      hash = hash ^ static_cast<uint8_t>(c);
      hash *= prime;
    }

    return static_cast<size_t>(hash);
  }

  ///
  /// Returns a unique id for the type at runtime.
  ///
  /// UniqueId's are packed, therefore they are ideal for lookup tables. Id's are incrementally
  /// distributed in a first come first serve fashion.
  ///
  /// UniqueId's are distributed from sequences. You can specify the sequence index (Usually a hash).
  ///
  /// @param[in] sequence_index An index used as an identifier to a sequence.
  ///
  /// @return size_t The unique id for the type and sequence.
  ///
  template<size_t SequenceIndex = 0>
  static size_t UniqueId()
  {
    static const size_t value = engine::UniqueId(FullName(), SequenceIndex);

    return value;
  }
};

// TODO Add meta specialization for variadic templates
// Move sorting from archetype.h
// Also add check for unique list

} // namespace genebits::engine

#endif