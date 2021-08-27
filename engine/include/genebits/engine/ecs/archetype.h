#ifndef GENEBITS_ENGINE_ECS_ARCHETYPE_H
#define GENEBITS_ENGINE_ECS_ARCHETYPE_H

#include <type_traits>

#include "genebits/engine/util/meta.h"

namespace genebits::engine
{
namespace
{
  // Type meta programing for sorting variadic templates.

  // Style exception: STL

  template<typename T1, typename T2>
  struct Compare
  {
    static constexpr bool value = Meta<T1>::FullName().compare(Meta<T2>::FullName()) < 0;

    using type = std::conditional_t<value, T1, T2>;
  };

  template<typename T1, typename... Types>
  struct TypeMin
  {
    using type = T1;
  };

  template<typename T1, typename T2, typename... Types>
  struct TypeMin<T1, T2, Types...>
  {
    using type = typename std::
      conditional_t<Compare<T2, T1>::value, typename TypeMin<T2, Types...>::type, typename TypeMin<T1, Types...>::type>;
  };

  template<typename Type, typename List>
  struct Remove;

  template<typename Type, typename... Types, template<typename...> class List>
  struct Remove<Type, List<Types...>>
  {
    using type = List<>;
  };

  template<typename T1, typename... Types, template<typename...> class List>
  struct Remove<T1, List<T1, Types...>>
  {
    using type = List<Types...>;
  };

  template<typename Type, typename T1, typename... Types, template<typename...> class List>
  struct Remove<Type, List<T1, Types...>>
  {
    using type = typename Remove<Type, List<Types..., T1>>::type;
  };

  template<typename Sorted, typename Unsorted>
  struct SortInto;

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<Types...>>
  {
    using type = Sorted<>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, Types...>>
  {
    using type = Sorted<T1>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename T2,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, T2, Types...>>
  {
    using type =
      std::conditional_t<Compare<T2, T1>::value, Sorted<SortedTypes..., T2, T1>, Sorted<SortedTypes..., T1, T2>>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename T2,
    typename T3,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, T2, T3, Types...>>
  {
  private:
    using NextSmallest = typename TypeMin<T1, T2, T3, Types...>::type;
    using NextSorted = Sorted<SortedTypes..., NextSmallest>;
    using NextUnsorted = typename Remove<NextSmallest, Unsorted<T1, T2, T3, Types...>>::type;

  public:
    using type = typename SortInto<NextSorted, NextUnsorted>::type;
  };

  template<typename... Types>
  struct Combined
  {
    using Sorted = typename SortInto<Combined<>, Combined<Types...>>::type;
  };
} // namespace

///
/// Archetype alias.
///
/// Makes sure that all components are sorted by name. This means that the alias will return the
/// same type even if the components are given in a different order.
///
/// @tparam Components The variadic template of all the component types in the archetype.
///
template<typename... Components>
using Archetype = typename Combined<Components...>::Sorted;

} // namespace genebits::engine

#endif
