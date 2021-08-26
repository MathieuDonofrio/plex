#include "genebits/engine/util/meta.h"

#include <atomic>
#include <map>
#include <mutex>

namespace genebits::engine
{
namespace
{
  constexpr size_t cMaxSequences = 1 << 10; // 1024

  std::atomic_uint32_t sequences[cMaxSequences];
  std::map<std::pair<std::string_view, size_t>, size_t> mappings;
  std::mutex mutex;

  ///
  /// Returns a unique integer identifier at runtime.
  ///
  /// The internal counter for the specified sequence is incremented every time
  /// this method is called.
  ///
  /// @note
  ///     The sequence index could be a hash.
  ///
  /// @note
  ///     There is a max amount of sequences (1024). Sequences will be reused if there are
  ///     collisions.
  ///
  /// @param[in] sequence_index Index of the sequence to get the next unique id from.
  ///
  /// @return size_t next index in the specified sequence
  ///
  size_t NextUniqueId(size_t sequence_index) noexcept
  {
    constexpr size_t cMaxSequences = 1 << 10; // 1024

    static std::atomic_uint32_t sequences[cMaxSequences]; // 32 bit is enough (4294967296 unique ids per sequence)

    const size_t actual_index = sequence_index & (cMaxSequences - 1);

    return sequences[actual_index].fetch_add(1, std::memory_order_relaxed);
  }
} // namespace

size_t UniqueId(std::string_view full_name, size_t sequence_index)
{
  const std::pair<std::string_view, size_t> key { full_name, sequence_index };

  mutex.lock();

  auto it = mappings.find(key);

  auto end = mappings.end();

  mutex.unlock();

  if (it != end) return it->second;
  else
  {
    size_t id = NextUniqueId(sequence_index);

    mutex.lock();

    mappings[key] = id;

    mutex.unlock();

    return id;
  }
}
} // namespace genebits::engine
