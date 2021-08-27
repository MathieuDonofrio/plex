#include "genebits/engine/util/meta.h"

#include <atomic>
#include <map>
#include <mutex>

namespace genebits::engine
{
size_t UniqueId(std::string_view full_name, size_t sequence_index)
{
  // Not performance critical. Should only get called once per <type,sequence index>.

  static std::map<size_t, size_t> sequences;
  static std::map<std::pair<std::string_view, size_t>, size_t> mappings;
  static std::mutex mutex;

  const std::pair<std::string_view, size_t> key { full_name, sequence_index };

  const std::scoped_lock<std::mutex> lock(mutex);

  const auto it = mappings.find(key);

  if (it == mappings.end())
  {
    size_t& sequence = sequences[sequence_index];

    const size_t id = sequence;
    sequence++;

    mappings[key] = id;

    return id;
  }
  else
  {
    return it->second;
  }
}
} // namespace genebits::engine
