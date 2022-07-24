#include "genebits/engine/utilities/type_info.h"

#include <atomic>
#include <map>
#include <mutex>

namespace plex
{
namespace details
{
  size_t TypeIndex(std::string_view type_name, std::string_view tag_name)
  {
    using MapKeyType = std::pair<std::string_view, std::string_view>;

    static std::map<std::string_view, size_t> sequences;
    static std::map<MapKeyType, size_t> mappings;

    static std::mutex mutex;

    // Not performance critical. Only gets called once per unique (type,sequence) entry.

    const MapKeyType mappings_key { type_name, tag_name };

    const std::scoped_lock<std::mutex> lock(mutex);

    const auto it = mappings.find(mappings_key);

    if (it == mappings.end())
    {
      // If this is a new mapping, we must assign a new unique index from the sequence.

      size_t& sequence = sequences[tag_name];

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
} // namespace details
} // namespace plex
