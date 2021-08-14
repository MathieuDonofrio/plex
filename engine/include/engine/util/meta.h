#ifndef GENEBITS_ENGINE_UTIL_META_H
#define GENEBITS_ENGINE_UTIL_META_H

#include <atomic>
#include <string_view>
#include <type_traits>

#include "engine/config/compiler.h"

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

  namespace probing
  {
    // Probing technique:
    // Get the templated function name of void type and use that to find the start and offset of the type.

    constexpr std::string_view cProbe = TemplatedFunctionName<void>();

    constexpr size_t cStart = cProbe.find("void");
    constexpr size_t cOffset = cProbe.length() - 4;
  } // namespace probing

  ///
  /// Returns the next index for the list of sequences. Used to be able to have more than once
  /// sequence to optimize memory usage in certain cases.
  ///
  /// An internal counter is incremented every time this method is called.
  ///
  /// @note
  ///     The static variable in the function is a header-only solution for extern. This is
  ///     guaranteed as long as ODR is not violated (No templates).
  ///
  /// @return size_t next index in the sequence
  ///
  inline size_t NextSequenceIndex()
  {
    static std::atomic_size_t sequence {};

    return sequence.fetch_add(1, std::memory_order_relaxed);
  }

  ///
  /// Returns the sequence index for the tag type.
  ///
  /// @tparam Tag A type tag used to identify the sequence to use.
  ///
  /// @return size_t The sequence index for the tag.
  ///
  template<typename Tag>
  static size_t SequenceIndex()
  {
    static const size_t value = NextSequenceIndex();

    return value;
  }

  ///
  /// Returns a unique integer identifier at runtime.
  ///
  /// The internal counter for the specified sequence is incremented every time
  /// this method is called.
  ///
  /// @note
  ///     There is a max amount of sequences (256). If there are more than 256 sequences
  ///     they will be reused. Using multiple sequences allows for more packed arrays
  ///     when the ids are used for lookup. This means better cache locality and potential
  ///     for less memory usage.
  ///
  /// @return size_t next index in the specified sequence
  ///
  inline size_t NextUniqueId(size_t sequence_index)
  {
    constexpr size_t cMaxSequences = 1 << 8; // 256

    static std::atomic_uint32_t sequences[cMaxSequences]; // 32 bit is enough (4294967296 unique ids per sequence)

    const size_t actual_index = sequence_index & (cMaxSequences - 1);

    return sequences[actual_index].fetch_add(1, std::memory_order_relaxed);
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
  /// @tparam Tag A type tag used to identify the sequence to use.
  ///
  /// @return size_t The unique id for the type and sequence.
  ///
  template<typename Tag = void>
  static size_t UniqueId() noexcept
  {
    static const size_t value = NextUniqueId(SequenceIndex<Tag>());

    return value;
  }
};

} // namespace genebits::engine

#endif