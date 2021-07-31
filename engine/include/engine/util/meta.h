#ifndef GENEBITS_ENGINE_UTIL_META_H
#define GENEBITS_ENGINE_UTIL_META_H

#include <string_view>
#include <type_traits>

#include "config.h"

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
  constexpr std::string_view TemplatedFunctionName()
  {
#if COMPILER_MSVC
    return __FUNCSIG__;
#else
    return __PRETTY_FUNCTION__;
#endif
  }

  constexpr std::string_view cProbe = TemplatedFunctionName<void>();

  constexpr size_t cStart = cProbe.find("void");
  constexpr size_t cOffset = cProbe.length() - 4;

  ///
  /// Returns a unique integer identifier at runtime.
  ///
  /// An internal counter is incremented each and every time this method is called.
  ///
  /// @tparam Tag for creating multiple sequences.
  ///
  /// @return size_t
  ///
  template<typename Tag>
  static size_t NextUniqueId()
  {
    static size_t sequence = 0;

    return sequence++;
  }
} // namespace

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
  static constexpr std::string_view FullName()
  {
    std::string_view full_func_name = TemplatedFunctionName<Type>();

    full_func_name = full_func_name.substr(cStart, full_func_name.length() - cOffset);

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
  static constexpr std::string_view Name()
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
  static constexpr size_t Hash()
  {
    constexpr uint64_t prime = 0x100000001b3;

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
  /// @warning Different libraries will result in different UniqueIds.
  /// Do not uses this as global identifier.
  ///
  /// @tparam Tag A type tag used to identify the sequence to use.
  ///
  /// @return size_t The unique id for the type and sequence.
  ///
  template<typename Tag = void>
  static size_t UniqueId()
  {
    static const size_t value = NextUniqueId<Tag>();

    return value;
  }
};

} // namespace genebits::engine

#endif