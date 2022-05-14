#ifndef GENEBITS_ENGINE_ECS_QUERY_H
#define GENEBITS_ENGINE_ECS_QUERY_H

#include "genebits/engine/ecs/registry.h"

namespace genebits::engine
{
///
/// Query categories are unique identifiers that identify where the query is getting its data from. This is used to
/// determine data dependencies.
///
/// @note Ids from [0, 255] are reserved.
///
/// Here are some examples:
/// - Two queries that obtain data from entity components are in the same category because we need to determine the data
///   dependencies between the two queries.
/// - The same data type stored as a resource and as a component should not create a data dependency because they are
///   stored differently. This means that the queries would be in different categories.
///
/// There is also the special 'None' category (id = 0) that is used to represent queries that don't have a category.
/// This means that they cannot produce data dependencies.
///
struct QueryCategory
{
  enum : std::uint32_t
  {
    None = 0, // Does not belong to any category. Cannot have any data dependencies.
    Resource = 1,
    Component = 2,
  };

  static constexpr const uint32_t LastReservedId = 255;
};

///
/// Structure containing the information about a single data access of a query.
///
/// Queries contain zero or more data accesses. Depending on the nature of the data access, the execution order of the
/// queries can vary.
///
struct QueryDataAccess
{
  std::string_view name; // Name of the data obtained using: TypeInfo<Type>::Name()
  uint32_t category_id; // Category of the data. (Query category)

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
  struct IsValidQueryDataAccessList : std::false_type
  {};

  ///
  /// Checks whether or not a type is a valid query data access list.
  ///
  /// Specialization for arrays.
  ///
  /// @tparam N Number of elements for array.
  ///
  template<size_t N>
  struct IsValidQueryDataAccessList<Array<QueryDataAccess, N>> : std::true_type
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
concept Query = requires(Registry& registry)
{
  {
    std::remove_cvref_t<Type>::GetDataAccess()
    } -> QueryDataAccessList;
  {
    std::remove_cvref_t<Type>::Get(registry)
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
/// @tparam Types The types to check.
///
template<size_t Category, typename... Types>
struct QueryDataAccessFactory
{
  static consteval Array<QueryDataAccess, sizeof...(Types)> GetDataAccess() noexcept
  {
    return { QueryDataAccess {
      TypeInfo<Types>::Name(), // Name of the type
      Category, // Query category
      std::is_const_v<Types>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Types>::value // Check ThreadSafe trait to see if the access is thread-safe.
    }... };
  }
};
} // namespace genebits::engine

#endif // GENEBITS_QUERY_H
