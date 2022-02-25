#include "genebits/engine/utilities/meta.h"

#include <atomic>
#include <map>
#include <mutex>

namespace genebits::engine
{
size_t UniqueId(std::string_view full_name, std::string_view sequence_key)
{
  using MapKeyType = std::pair<std::string_view, std::string_view>;

  static std::map<std::string_view, size_t> sequences;
  static std::map<MapKeyType, size_t> mappings;

  static std::mutex mutex;

  // Not performance critical. Only gets called once per unique (type,sequence) entry.

  const MapKeyType mappings_key { full_name, sequence_key };

  const std::scoped_lock<std::mutex> lock(mutex);

  const auto it = mappings.find(mappings_key);

  if (it == mappings.end())
  {
    // If this is a new mapping, we must assign a new unique index from the sequence.

    size_t& sequence = sequences[sequence_key];

    const size_t id = sequence;
    sequence++;

    mappings[mappings_key] = id;

    return id;
  }
  else
  {
    return it->second;
  }
}
} // namespace genebits::engine
