#ifndef PLEX_UTILITIES_TYPE_SORT_H
#define PLEX_UTILITIES_TYPE_SORT_H

#include "type_info.h"

namespace plex
{
namespace details
{
  // Utility for sorting list of types

  ///
  ///  Lexically compares the two types using their compile time name and returns true if T1 is smaller than T2.
  ///
  /// @tparam T1 First type.
  /// @tparam T2 Second type.
  ///
  template<typename T1, typename T2>
  constexpr bool Compare = TypeName<T1>().compare(TypeName<T2>()) < 0;

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
    : std::conditional<Compare<T2, T1>, typename TypeMin<T2, Types...>::type, typename TypeMin<T1, Types...>::type>
  {};

  ///
  /// Removes a type from a variadic list of types.
  ///
  /// @tparam Type Type to remove.
  /// @tparam List Variadic list of types.
  ///
  template<typename Type, typename List>
  struct Remove;

  // If the first element in the list of the type we are looking for, return the list without this type
  template<typename T1, typename... Types, template<typename...> class List>
  struct Remove<T1, List<T1, Types...>>
  {
    using type = List<Types...>;
  };

  // If the first element in the list is not the type we are looking for, rotate the list.
  template<typename Type, typename T1, typename... Types, template<typename...> class List>
  struct Remove<Type, List<T1, Types...>>
  {
    using type = typename Remove<Type, List<Types..., T1>>::type;
  };

  ///
  /// Sorts an unsorted variadic type list and inserts it into another variadic type in order.
  ///
  /// @tparam Sorted Variadic container to sort into.
  /// @tparam Unsorted Variadic container to sort.
  ///
  template<typename Sorted, typename Unsorted>
  struct SortInto;

  // If there are no types in the unsorted list, return the sorted list.
  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<Types...>>
  {
    using type = Sorted<SortedTypes...>;
  };

  // If there is one type in the unsorted list, add the type to the end of the sorted list and return.
  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, Types...>>
  {
    using type = Sorted<SortedTypes..., T1>;
  };

  // If there are two types left in the unsorted list, find the smaller one and it first to the end of the sorted list,
  // then add the bigger one and return the sorted list.
  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename T2,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, T2, Types...>>
    : std::conditional<Compare<T2, T1>, Sorted<SortedTypes..., T2, T1>, Sorted<SortedTypes..., T1, T2>>
  {};

  // Find the smallest type, add it to the end of the sorted list and remove it from the unsorted list.
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
} // namespace details
} // namespace plex

///
/// The default variadic template type to use as list for sorting types.
///
template<typename...>
struct _S // Keep small to avoid overhead
{};

namespace plex
{
///
/// Sort the types lexicographically and returns an instantiation of given variadic template with all types in order.
///
/// @tparam VariadicType The variadic template type to create the sorted variadic type with.
/// @tparam Types The types to sort.
///
template<template<typename...> class VariadicType, typename... Types>
using SortedTypesFor = typename details::SortInto<VariadicType<>, VariadicType<Types...>>::type;

///
/// Sort the types lexicographically and returns an instantiation of a variadic template with all types in order.
///
/// @tparam Types The types to sort.
///
template<typename... Types>
using SortedTypes = SortedTypesFor<_S, Types...>;
} // namespace plex

#endif
