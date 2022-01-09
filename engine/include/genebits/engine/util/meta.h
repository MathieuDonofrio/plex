#ifndef GENEBITS_ENGINE_UTIL_META_H
#define GENEBITS_ENGINE_UTIL_META_H

#include <string_view>
#include <type_traits>

#include "genebits/engine/config/compiler.h"

namespace fsig // Function signature namespace. Keep small name, for faster compile time
{
///
/// Returns the function name with a templated type. Can be used to probe type name.
///
/// @tparam Type The type to include in the function name
///
/// @return The function name with the templated type.
///
template<typename Type>
consteval const char* /* Keep char* for faster compile-time */ F()
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
///
/// Returns a unique id for the type name and sequence at runtime.
///
/// UniqueId's are packed, therefore they are ideal for lookup tables. Id's are incrementally
/// distributed in a first come first serve fashion.
///
/// UniqueId's are distributed from sequences. You can specify the sequence index (Usually a hash).
///
/// @param[in] full_name The name of the type.
/// @param[in] sequence_key Identifier to the index sequence.
///
/// @return size_t The unique id for the type name and sequence.
///
size_t UniqueId(std::string_view full_name, std::string_view sequence_key);

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
  /// @warning
  ///     Does not return the same result on every compiler. And the name is not
  ///     guaranteed to be human readable.
  ///
  /// @note Const, volatile and references are stripped from the type.
  ///
  /// @return Full name of templated type.
  ///
  [[nodiscard]] static consteval std::string_view Name() noexcept
  {
    const std::string_view function_name = fsig::F<std::remove_cvref_t<Type>>();

    return function_name.substr(fsig::cStart, function_name.length() - fsig::cOffset);
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

    for (const auto c : Name())
    {
      hash = hash ^ static_cast<uint8_t>(c);
      hash *= prime;
    }

    return hash;
  }

  ///
  /// Returns a unique id for the type at runtime.
  ///
  /// UniqueId's are packed, therefore they are ideal for lookup tables. Id's are incrementally
  /// distributed in a first come first serve fashion.
  ///
  /// UniqueId's are distributed from sequences. You can specify the sequence index (Usually a hash).
  ///
  /// @note Implies one branch of overhead that will correctly be branch predicted.
  ///
  /// @tparam Sequence Type to use name as identifier for the sequence to use.
  ///
  /// @return size_t The unique id for the type and sequence.
  ///
  template<typename Sequence = void>
  static size_t UniqueId()
  {
    static const size_t value = engine::UniqueId(Name(), Meta<Sequence>::Name());

    return value;
  }
};

} // namespace genebits::engine

#endif