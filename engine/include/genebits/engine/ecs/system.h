#ifndef GENEBITS_ENGINE_ECS_SYSTEM_H
#define GENEBITS_ENGINE_ECS_SYSTEM_H

#include "genebits/engine/async/task.h"
#include "genebits/engine/ecs/query.h"
#include "genebits/engine/utilities/ref.h"

namespace genebits::engine
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

template<typename Return, Query... Queries>
struct IsSystem<Return(Queries...)> : std::true_type
{};

template<typename Return, Query... Queries>
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
  /// Utility for checking if a query is of a certain template.
  ///
  /// @tparam QueryTemplate The template to check.
  /// @tparam Query The query to check.
  ///
  template<template<class...> typename QueryTemplate, typename Query>
  struct IsQueryType : public std::false_type
  {};

  template<template<class...> typename Query, typename... Types>
  struct IsQueryType<Query, Query<Types...>> : public std::true_type
  {};

public:
  using SystemType = Return(Queries...);
  using ReturnType = Return;

  static constexpr size_t QueryCount = sizeof...(Queries);
  static constexpr bool IsCoroutine = Awaitable<ReturnType>;

  ///
  /// Checks if the system contains a query of a certain template/type.
  ///
  /// @tparam Query The query template to check.
  ///
  template<template<class...> typename Query>
  using ContainsQueryType = std::disjunction<IsQueryType<Query, std::remove_cvref_t<Queries>>...>;

  ///
  /// Invokes the system with the specified registry.
  ///
  /// Will create all system queries for the registry, then call the system function with the queries.
  ///
  /// Always returns a void coroutine task. If the system was a couroutine, it will be awaited, otherwise, the system
  /// will be invoked and returned as a task.
  ///
  /// @param[in] system The system to invoke.
  /// @param[in] registry The registry to use.
  ///
  /// @return Coroutine task of the system invocation.
  ///
  static Task<> Invoke(SystemType* system, Registry& registry)
  {
    if constexpr (IsCoroutine) { co_await system(std::remove_cvref_t<Queries>::Get(registry)...); }
    else
    {
      system(std::remove_cvref_t<Queries>::Get(registry)...);
      co_return;
    }
  }

  ///
  /// Returns the data accesses of the system at compile-time.
  ///
  /// @return Array of data accesses.
  ///
  static consteval auto GetDataAccess() noexcept
  {
    if constexpr (sizeof...(Queries) == 0) return Array<SystemDataAccess, 0> {};
    else
    {
      return (... + std::remove_cvref_t<Queries>::GetDataAccess());
    }
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
  /// Executes the system.
  ///
  /// @param[in] registry The registry to use.
  ///
  /// @return The task of the system invocation.
  ///
  Task<> operator()(Registry& registry) const
  {
    return executor_(registry, system_);
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
  /// @param[in] registry The registry to use.
  ///
  /// @param[in] system The system to invoke.
  ///
  /// @return The task of the system invocation.
  ///
  template<typename SystemType>
  static Task<> Execute(Registry& registry, SystemHandle system)
  {
    return SystemTraits<SystemType>::Invoke(std::bit_cast<SystemType>(system), registry);
  }

private:
  SystemHandle system_;
  Task<> (*executor_)(Registry&, SystemHandle);
};

///
/// Type-erased wrapper for a system.
///
/// Contains executor and extra information about the system.
///
class SystemObject : public RefCounted
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
  SystemObject(SystemType system) noexcept : executor_(system), info_(std::make_unique<Info>())
  {
    info_->data_access = SystemTraits<SystemType>::GetDataAccess();
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
  ///
  /// Struct that contains the extra information about the system.
  ///
  struct Info
  {
    Vector<SystemDataAccess> data_access;
  };

private:
  SystemExecutor executor_;

  std::unique_ptr<Info> info_;
};
} // namespace genebits::engine

#endif
