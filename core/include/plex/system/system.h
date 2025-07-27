#ifndef PLEX_SYSTEM_SYSTEM_H
#define PLEX_SYSTEM_SYSTEM_H

#include "plex/async/task.h"
#include "plex/system/context.h"
#include "plex/system/query.h"
#include "plex/utilities/ref.h"

namespace plex
{
///
/// Type-erased handle to a system function.
///
using SystemHandle = void*;

///
/// Checks wither or not a type is a system.
///
/// A type is a system if all its arguments are queries.
///
/// @tparam Type Type to check.
///
template<typename Type>
struct IsSystem : std::false_type
{};

template<typename Return, typename... Queries>
struct IsSystem<Return(Queries...)> : std::true_type
{};

template<typename Return, typename... Queries>
struct IsSystem<Return (*)(Queries...)> : std::true_type
{};

///
/// Checks wither or not a type is a system.
///
/// A type is a system if all its arguments are queries.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept System = IsSystem<Type>::value;

///
/// Traits for a system.
///
/// Used to obtain information about a system. Higher level system functionality uses these traits to obtain
/// information.
///
/// @tparam SystemType Type of the system.
///
template<System SystemType>
struct SystemTraits;

template<typename Return, typename... Queries>
struct SystemTraits<Return(Queries...)>
{
private:
  ///
  /// Fetches the query for the given type and the given context. If the query type does not meet the concept of a
  /// query, a global query with the type is assumed.
  ///
  /// @tparam QueryType The query type or type for a global query.
  ///
  /// @param[in] system Handle to the system that is invoking a fetch for the query.
  /// @param[in] global_context The global context (All systems have access).
  /// @param[in] local_context The local context (Only given system has access).
  ///
  /// @return The populated query object.
  ///
  template<typename QueryType>
  static decltype(auto) Fetch(
    [[maybe_unused]] void* system, Context& global_context, [[maybe_unused]] Context& local_context)
  {
    if constexpr (Query<std::remove_cvref_t<QueryType>>)
    {
      return std::remove_cvref_t<QueryType>::Fetch(system, global_context, local_context);
    }
    else
    {
      return global_context.template Get<QueryType>();
    }
  }

  ///
  /// Returns all the data access of the given query. If the query type does not meet the concept of a
  /// query, a global query with the type is assumed.
  ///
  /// For implicit global queries, if passed by copy the data access is data access is read-only.
  ///
  /// @tparam QueryType The query type to obtain data access for.
  ///
  /// @return Data accesses for the query.
  ///
  template<typename QueryType>
  static consteval auto GetDataAccess() noexcept
  {
    if constexpr (Query<std::remove_cvref_t<QueryType>>)
    {
      return std::remove_cvref_t<QueryType>::GetDataAccess();
    }
    else if constexpr (std::is_reference_v<QueryType>)
    {
      return Global<std::remove_reference_t<QueryType>>::GetDataAccess();
    }
    else
    {
      return Global<std::add_const_t<QueryType>>::GetDataAccess();
    }
  }

public:
  using SystemType = Return(Queries...);
  using ReturnType = Return;

  static constexpr size_t QueryCount = sizeof...(Queries);
  static constexpr bool IsCoroutine = Awaitable<ReturnType>;

  ///
  /// Invokes the system with the context.
  ///
  /// Will create all the queries fetching the data from various context data sources.
  ///
  /// @param[in] system The system to invoke.
  /// @param[in] global_context The global context to use, contains all global state.
  ///
  /// @return Coroutine task of the system invocation.
  ///
  static Task<> Invoke(SystemType* system, Context& global_context, Context& local_context)
  {
    const SystemHandle handle = std::bit_cast<SystemHandle>(system);

    if constexpr (IsCoroutine)
    {
      co_await system(Fetch<Queries>(handle, global_context, local_context)...);
    }
    else
    {
      system(Fetch<Queries>(handle, global_context, local_context)...);
      co_return;
    }
  }

  ///
  /// Returns the data accesses of the system at compile-time.
  ///
  /// @return Array of data accesses.
  ///
  static Vector<QueryDataAccess> GetDataAccess() noexcept
  {
    Vector<QueryDataAccess> joined;
    ((std::ranges::copy(GetDataAccess<Queries>(), std::back_inserter(joined))), ...);
    return joined;
  }
};

template<typename Return, typename... Args>
class SystemTraits<Return (*)(Args...)> : public SystemTraits<Return(Args...)>
{};

///
/// Type erased executor wrapper for a system.
///
/// Simply contained the type erased system, and a function that knows how to invoke the system.
///
class SystemExecutor
{
public:
  using IsTriviallyRelocatable = std::true_type;

  ///
  /// Constructor.
  ///
  /// @tparam SystemType The system type.
  ///
  /// @param[in] system The system to wrap.
  ///
  template<System SystemType>
  constexpr SystemExecutor(SystemType system) noexcept
    : system_(std::bit_cast<SystemHandle>(system)), executor_(SystemExecutor::Execute<decltype(system)>)
  {}

  SystemExecutor(const SystemExecutor&) = default;

  ///
  /// Executes the system for the context.
  ///
  /// @param[in] global_context The global context.
  /// @param[in] local_context The local context.
  ///
  /// @return The task of the system invocation.
  ///
  Task<> operator()(Context& global_context, Context& local_context) const
  {
    return executor_(system_, global_context, local_context);
  }

  ///
  /// Handle to the system.
  ///
  /// @return The system.
  ///
  [[nodiscard]] SystemHandle Handle() const noexcept
  {
    return system_;
  }

private:
  ///
  /// Template function that knows how to invoke the typed erased system.
  ///
  /// @tparam SystemType The system type.
  ///
  /// @param[in] system The system to invoke.
  /// @param[in] global_context The global context.
  /// @param[in] local_context The local context.
  ///
  /// @return The task of the system invocation.
  ///
  template<typename SystemType>
  static Task<> Execute(SystemHandle system, Context& global_context, Context& local_context)
  {
    return SystemTraits<SystemType>::Invoke(std::bit_cast<SystemType>(system), global_context, local_context);
  }

private:
  SystemHandle system_;
  Task<> (*executor_)(SystemHandle, Context&, Context&);
};

///
/// Type-erased wrapper for a system.
///
/// Contains system information and local state.
///
class SystemObject
{
public:
  using IsTriviallyRelocatable = std::true_type;

  ///
  /// Constructor.
  ///
  /// @tparam SystemType The system type.
  ///
  /// @param[in] system The system to wrap.
  ///
  template<System SystemType>
  explicit SystemObject(SystemType system) noexcept
    : executor_(system), data_access_(SystemTraits<SystemType>::GetDataAccess())
  {}

  ///
  /// Executes the system for the context.
  ///
  /// @param[in] global_context The global context.
  ///
  /// @return The task of the system invocation.
  ///
  Task<> operator()(Context& global_context)
  {
    return executor_(global_context, local_context_);
  }

  ///
  /// Checks whether or not one system object has a data dependency on another.
  ///
  /// If there is a dependency, that means that the two systems cannot be executed in parallel.
  ///
  /// @param[in] system The system to check.
  ///
  /// @return Whether or not there is a dependency.
  ///
  [[nodiscard]] bool HasDependency(const SystemObject& system) const noexcept;

  ///
  /// Returns a copy of the executor of the system.
  ///
  /// @return The executor.
  ///
  [[nodiscard]] SystemExecutor GetExecutor() const noexcept
  {
    return executor_;
  }

  ///
  /// Returns the system handle.
  ///
  /// @return The system handle.
  ///
  [[nodiscard]] SystemHandle Handle() const noexcept
  {
    return executor_.Handle();
  }

  ///
  /// Equality operator.
  ///
  /// Compares the handles of the two systems.
  ///
  /// @param[in] lhs The left hand side.
  /// @param[in] rhs The right hand side.
  ///
  /// @return Whether or not the two systems are equal.
  ///
  friend bool operator==(const SystemObject& lhs, const SystemObject& rhs) noexcept
  {
    return lhs.Handle() == rhs.Handle();
  }

  ///
  /// Inequality operator.
  ///
  /// Compares the handles of the two systems.
  ///
  /// @param[in] lhs The left hand side.
  /// @param[in] rhs The right hand side.
  ///
  /// @return Whether or not the two systems are equal.
  ///
  friend bool operator!=(const SystemObject& lhs, const SystemObject& rhs) noexcept
  {
    return !(lhs == rhs);
  }

private:
  SystemExecutor executor_;
  Context local_context_;
  Vector<QueryDataAccess> data_access_;
};
} // namespace plex

#endif
