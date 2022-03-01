#ifndef GENEBITS_ENGINE_ECS_ARCHETYPE_H
#define GENEBITS_ENGINE_ECS_ARCHETYPE_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <type_traits>

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/type_info.h"

namespace genebits::engine
{
namespace details
{
  // Type meta programing for sorting variadic templates.

  // Style exception: STL

  ///
  /// Lexically compares the two types.
  ///
  /// @tparam T1 First type.
  /// @tparam T2 Second type.
  ///
  template<typename T1, typename T2>
  struct Compare
  {
    static constexpr bool value = TypeInfo<T1>::Name().compare(TypeInfo<T2>::Name()) < 0;

    using type = std::conditional_t<value, T1, T2>;
  };

  ///
  /// Returns the smallest type.
  ///
  /// @tparam T1 First type.
  /// @tparam Types Other types.
  ///
  template<typename T1, typename... Types>
  struct TypeMin
  {
    using type = T1;
  };

  ///
  /// Returns the smallest type.
  ///
  /// @tparam T1 First type.
  /// @tparam T2 Second type.
  /// @tparam Types Other types.
  ///
  template<typename T1, typename T2, typename... Types>
  struct TypeMin<T1, T2, Types...>
  {
    using type = typename std::
      conditional_t<Compare<T2, T1>::value, typename TypeMin<T2, Types...>::type, typename TypeMin<T1, Types...>::type>;
  };

  ///
  /// Removes a type from a variadic list of types.
  ///
  /// @tparam Type Type to remove.
  /// @tparam List Variadic list of types.
  ///
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

  ///
  /// Sorts an unsorted variadic type list into another variadic container.
  ///
  /// @tparam Sorted Variadic container to sort into.
  /// @tparam Unsorted Variadic container to sort.
  ///
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

  // Could be possible to add more specializations for optimal solution sorting. This may reduce compile-time.

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

  ///
  /// Container for storing sorted types.
  ///
  /// @tparam Types The types to combine.
  ///
  template<typename... Types>
  struct Combined
  {
    using Sorted = typename SortInto<Combined<>, Combined<Types...>>::type;
  };
} // namespace details

///
/// Alias that sorts a component type list into a single orderless type.
///
template<typename... Components>
using ComponentList = typename details::Combined<Components...>::Sorted;

using ComponentId = uint_fast32_t;
using ViewId = uint_fast32_t;
using ArchetypeId = uint_fast32_t;

///
/// Tag used for the unique id sequence of component types.
///
struct ComponentIdTag
{};

///
/// Tag used for the unique id sequence of archetype types.
///
struct ArchetypeIdTag
{};

///
/// Tag used for the unique id sequence of view types.
///
struct ViewIdTag
{};

///
/// Returns the component id for the component type.
///
/// Ids come from a packed sequence starting at 0.
///
/// @tparam Component Component type to get id for.
///
/// @return Component identifier.
///
template<typename Component>
ComponentId GetComponentId()
{
  return static_cast<ComponentId>(TypeInfo<Component>::template Index<ComponentIdTag>());
}

///
/// Returns the archetype id for the component type list.
///
/// Ids come from a packed sequence starting at 0.
///
/// @note The order of the components does not matter.
///
/// @tparam Components Components that compose the archetype.
///
/// @return Archetype identifier.
///
template<typename... Components>
ArchetypeId GetArchetypeId()
{
  return static_cast<ArchetypeId>(TypeInfo<ComponentList<Components...>>::template Index<ArchetypeIdTag>());
}

///
/// Returns the view id for the component type list.
///
/// Ids come from a packed sequence starting at 0.
///
/// @note The order of the components does not matter.
///
/// @tparam Components Components that compose the archetype.
///
/// @return View identifier.
///
template<typename... Components>
ViewId GetViewId()
{
  return static_cast<ViewId>(TypeInfo<ComponentList<Components...>>::template Index<ViewIdTag>());
}

namespace details
{
  ///
  /// Utility for obtaining the runtime component sequence (signature) for list of types.
  ///
  /// @tparam ComponentList Variadic component type list.
  ///
  template<typename ComponentList>
  struct ComponentSequence;

  template<typename... Components, template<typename...> class ComponentList>
  struct ComponentSequence<ComponentList<Components...>>
  {
    ///
    /// Returns the runtime component sequence using the component ids.
    ///
    /// @return Vector of component ids at runtime.
    ///
    static Vector<ComponentId> Value()
    {
      Vector<ComponentId> components;
      components.Reserve(sizeof...(Components));

      (components.PushBack(GetComponentId<Components>()), ...);

      std::ranges::sort(components);

      return components;
    }
  };
} // namespace details

///
/// Returns a sorted vector of all the component ids for the list of component types.
///
/// @tparam Components Component types to get ids for.
///
/// @return Sorted list of component ids.
///
template<typename... Components>
const Vector<ComponentId>& GetComponentIds()
{
  static const auto components = details::ComponentSequence<ComponentList<Components...>>::Value();

  return components;
}

} // namespace genebits::engine

#endif
