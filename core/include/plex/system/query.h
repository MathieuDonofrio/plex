#ifndef PLEX_SYSTEM_QUERY_H
#define PLEX_SYSTEM_QUERY_H

#include <array>
#include <ranges>
#include <string_view>

#include "plex/containers/carray.h"
#include "plex/system/context.h"
#include "plex/utilities/puple.h"

namespace plex
{
///
/// Structure containing the information about a single data access of a query.
///
/// Queries contain zero or more data accesses. Depending on the nature of the data access, the execution order of the
/// queries can vary.
///
struct QueryDataAccess
{
  std::string_view source; // Name of the data source
  std::string_view section; // The section of the data source that is being accessed, empty if everything is accessed

  // Flags
  bool read_only; // Whether the data is read-only or not.
  bool thread_safe; // Whether the data is thread-safe or not.
};

///
/// Concept used to determine whether or not a type is a valid range of QueryDataAccess.
///
/// @tparam Type The type to check.
///
template<typename Type>
concept QueryDataAccessRange =
  std::ranges::random_access_range<Type> && std::same_as<std::ranges::range_value_t<Type>, QueryDataAccess>;

// clang-format off

///
/// Query concept for systems.
///
/// Queries dispatched during the invocation of a system. They are used to fetch specified data from the registry.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept Query = requires(void* handle, Context global_context, Context local_context)
{
  // Returns information about every data access.
  { Type::GetDataAccess() } -> QueryDataAccessRange;

  // Obtains the data for the query.
  { Type::Fetch(handle, global_context, local_context) } -> std::convertible_to<Type>;
};

// clang-format on

///
/// Global query.
///
/// Fetches the data directly from the global context.
///
/// @warning If the queried objects do not exist in the global context, the behaviour of this query is undefined.
///
/// @tparam Types The types of objects to query.
///
template<typename... Types>
requires(sizeof...(Types) > 0)
struct Global : public Puple<Types...>
{
  using Puple<Types...>::Puple;

  static Global Fetch(void*, Context& global_context, Context&)
  {
    return { (&global_context.template Get<Types>())... };
  }

  static consteval std::array<QueryDataAccess, sizeof...(Types)> GetDataAccess() noexcept
  {
    return { QueryDataAccess {
      TypeName<Types>(),
      {}, // Access entire data source
      std::is_const_v<Types>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Types>::value // Check ThreadSafe trait to see if the access is thread-safe.
    }... };
  }
};

///
/// Global query.
///
/// Fetches the data directly from the global context.
///
/// @warning If the queried objects do not exist in the global context, the behaviour of this query is undefined.
///
/// @tparam Types The types of objects to query.
///
template<typename... Types>
requires(sizeof...(Types) > 0)
struct Local : public Puple<Types...>
{
  using Puple<Types...>::Puple;

  static Local Fetch(void*, Context&, Context& local_context)
  {
    return { (&local_context.template Get<Types>())... };
  }

  static consteval std::array<QueryDataAccess, sizeof...(Types)> GetDataAccess() noexcept
  {
    return {};
  }
};
} // namespace plex

DEFINE_PUPLE_LIKE(::plex::Global)
DEFINE_PUPLE_LIKE(::plex::Local)

#endif
