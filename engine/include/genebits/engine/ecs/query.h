#ifndef GENEBITS_ENGINE_ECS_QUERY_H
#define GENEBITS_ENGINE_ECS_QUERY_H

#include <string_view>

#include "genebits/engine/containers/array.h"
#include "genebits/engine/ecs/context.h"

namespace genebits::engine
{
///
/// Structure containing the information about a single data access of a query.
///
/// Queries contain zero or more data accesses. Depending on the nature of the data access, the execution order of the
/// queries can vary.
///
struct QueryDataAccess
{
  std::string_view name; // Name of the data obtained using: TypeInfo<Type>::Name()
  std::string_view category; // Category of the data obtained using: Type::GetCategory()

  // Flags
  bool read_only; // Whether the data is read-only or not.
  bool thread_safe; // Whether the data is thread-safe or not.
};

namespace details
{
  ///
  /// Checks whether or not a type is a valid query data access list.
  ///
  /// @tparam Type Type to check.
  ///
  template<typename Type>
  struct IsValidQueryDataAccessList : public std::false_type
  {};

  ///
  /// Checks whether or not a type is a valid query data access list.
  ///
  /// Specialization for arrays.
  ///
  /// @tparam N Number of elements for array.
  ///
  template<size_t N>
  struct IsValidQueryDataAccessList<Array<QueryDataAccess, N>> : public std::true_type
  {};
} // namespace details

///
/// Concept used to determine whether or not a type is a valid list of QueryDataAccess.
///
/// @tparam Type The type to check.
///
template<typename Type>
concept QueryDataAccessList = details::IsValidQueryDataAccessList<Type>::value;

///
/// Query concept for systems.
///
/// Queries dispatched during the invocation of a system. They are used to fetch specified data from the registry.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept Query = requires(Context& data_sources)
{
  {
    std::remove_cvref_t<Type>::GetCategory()
    } -> std::convertible_to<std::string_view>;
  {
    std::remove_cvref_t<Type>::GetDataAccess()
    } -> QueryDataAccessList;
  {
    std::remove_cvref_t<Type>::FetchData(data_sources)
    } -> std::same_as<std::remove_cvref_t<Type>>;
};

///
/// Implementation of the GetDataAccess requirement of a query.
///
/// @note Queries are not required to use this for generating GetDataAccess, but this is the standard way to do it.
///
/// Uses const qualification to check if a type is read-only.
/// Uses IsThreadSafe trait to check if a type is thread-safe.
///
/// @tparam Query The query type.
/// @tparam Types The types to check.
///
template<typename Query, typename... Types>
struct QueryDataAccessFactory
{
  ///
  /// Returns all the data accesses for the query at compile-time.
  ///
  /// @return Array of data accesses.
  ///
  static consteval Array<QueryDataAccess, sizeof...(Types)> GetDataAccess() noexcept
  {
    const std::string_view category = Query::GetCategory();

    return { QueryDataAccess {
      TypeInfo<Types>::Name(), // Name of the type
      category, // Query category
      std::is_const_v<Types>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Types>::value // Check ThreadSafe trait to see if the access is thread-safe.
    }... };
  }
};
} // namespace genebits::engine

#endif // GENEBITS_QUERY_H
