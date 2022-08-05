#ifndef PLEX_SYSTEM_QUERY_H
#define PLEX_SYSTEM_QUERY_H

#include <string_view>

#include "plex/containers/array.h"
#include "plex/system/context.h"

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
  // Returns the data category of the query.
  // This allows different query types to use the same data type.
  { std::remove_cvref_t<Type>::GetCategory() } -> std::convertible_to<std::string_view>;

  // Returns information about every data access.
  { std::remove_cvref_t<Type>::GetDataAccess() } -> QueryDataAccessList;

  // Obtains the data for the query.
  { std::remove_cvref_t<Type>::Fetch(handle, global_context, local_context) } -> std::same_as<std::remove_cvref_t<Type>>;
};

// clang-format on

///
/// Implementation of the GetDataAccess requirement of a query.
///
/// @note Queries are not required to use this for generating GetDataAccess.
///
/// Uses const qualification to check if a type is read-only.
/// Uses IsThreadSafe trait to check if a type is thread-safe.
///
/// @tparam Query The query type.
/// @tparam Types The types to check.
///
template<typename Query, typename... Types>
struct BasicQueryDataAccessFactory
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
      TypeName<Types>(), // Name of the type
      category, // Query category
      std::is_const_v<Types>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Types>::value // Check ThreadSafe trait to see if the access is thread-safe.
    }... };
  }
};

namespace details
{
  template<typename... Types>
  class BasicQueryBase;

  // Fancy pointer specializations for single type

  template<typename Type>
  class BasicQueryBase<Type>
  {
  public:
    constexpr BasicQueryBase(Type* value) noexcept : value_(value) {}

    const Type& operator*() const noexcept
    {
      return *value_;
    }

    Type& operator*() noexcept
    {
      return *value_;
    }

    const Type* operator->() const noexcept
    {
      return value_;
    }

    Type* operator->() noexcept
    {
      return value_;
    }

    operator Type() const noexcept
    {
      return *value_;
    }

  private:
    Type* value_;
  };

  template<typename Type>
  class BasicQueryBase<Type const>
  {
  public:
    constexpr BasicQueryBase(const Type* value) noexcept : value_(value) {}

    const Type& operator*() const noexcept
    {
      return *value_;
    }

    const Type* operator->() const noexcept
    {
      return value_;
    }

    operator Type() const noexcept
    {
      return *value_;
    }

  private:
    const Type* value_;
  };
} // namespace details

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
struct Global : public details::BasicQueryBase<Types...>, public BasicQueryDataAccessFactory<Global<Types...>, Types...>
{
  constexpr Global(Types*... values) noexcept : details::BasicQueryBase<Types...>(values...) {}

  static Global Fetch(void*, Context& global_context, Context&)
  {
    return { (&global_context.template Get<Types>())... };
  }

  static constexpr std::string_view GetCategory()
  {
    return "Global";
  }
};
} // namespace plex

#endif
