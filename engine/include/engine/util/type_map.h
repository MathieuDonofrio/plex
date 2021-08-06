#ifndef GENEBITS_ENGINE_UTIL_TYPE_MAP_H
#define GENEBITS_ENGINE_UTIL_TYPE_MAP_H

#include <limits>
#include <type_traits>

#include "engine/util/concepts.h"
#include "engine/util/meta.h"

namespace genebits::engine
{

// TODO maybe count size for debug builds and assert

template<typename Value, size_t Size = 512>
class TypeMap
{
public:
  TypeMap()
    : type_names_ {}
  {
  }

  template<typename Type>
  constexpr Value& Get() noexcept
  {
    constexpr const char* type_name = Meta<Type>::FullName().data();

    size_t key = Key<Type>();

    [[unlikely]] while (type_names_[key] != type_name)
    {
      if (!type_names_[key])
      {
        type_names_[key] = type_name;
        break;
      }
      key++;
    }

    return values_[key];
  }

private:
  template<typename Type>
  [[nodiscard]] static consteval size_t Key() noexcept
  {
    return static_cast<size_t>(Meta<Type>::Hash() % Size);
  }

private:
  const char* type_names_[Size];
  Value values_[Size];
};

} // namespace genebits::engine

#endif
