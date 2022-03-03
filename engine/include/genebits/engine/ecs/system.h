#ifndef GENEBITS_ENGINE_ECS_SYSTEM_H
#define GENEBITS_ENGINE_ECS_SYSTEM_H

#include "genebits/engine/async/async_latch.h"
#include "genebits/engine/async/shared_task.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/async/thread_pool.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/registry.h"
#include "genebits/engine/utilities/ref.h"

namespace genebits::engine
{
///
/// Information about a system component data access:
///
/// - The identifier of the component of the access.
/// - Whether or not the access is read-only.
///
struct SystemDataAccess
{
  ComponentId id;
  bool read_only;
};

///
/// Holds information about every single data access of a system.
///
using SystemDataAccessList = Vector<SystemDataAccess>;

///
/// Provides a list of system data accesses.
///
class SystemDataAccessProvider
{
public:
  ///
  /// Default constructor.
  ///
  constexpr SystemDataAccessProvider() noexcept : initialized_(false) {}

  ///
  /// Destructor.
  ///
  virtual ~SystemDataAccessProvider() = default;

  ///
  /// Returns the list of all the entity component data the system is allowed to access.
  ///
  /// @return List of all data system accesses.
  ///
  [[nodiscard]] const SystemDataAccessList& GetDataAccess() const noexcept
  {
    if (!initialized_) [[unlikely]]
    {
      new (static_cast<void*>(&access_)) SystemDataAccessList(CreateDataAccessList());
      initialized_ = true;
    }

    return access_;
  }

private:
  ///
  /// Called once by derived class to fetch all the data accesses and create the list.
  ///
  /// @return Newly created list of all data system accesses.
  ///
  virtual SystemDataAccessList CreateDataAccessList() const = 0;

private:
  mutable SystemDataAccessList access_;
  mutable bool initialized_;
};

template<typename... Components>
class System;

///
/// Polymorphic system base.
///
class SystemBase : public SystemDataAccessProvider
{
public:
  ///
  /// Constructor.
  ///
  SystemBase() = default;

  SystemBase(const SystemBase&) = delete;
  SystemBase& operator=(const SystemBase&) = delete;
  SystemBase(SystemBase&&) = delete;
  SystemBase& operator=(SystemBase&&) = delete;

  ///
  /// Returns a task for executing the system update.
  ///
  /// @return Task of the system update.
  ///
  Task<> Update()
  {
    return OnUpdate();
  }

  ///
  /// Sets the registry for this system.
  ///
  /// @param[in] registry Registry to set.
  ///
  void SetRegistry(Registry& registry)
  {
    registry_ = std::addressof(registry);
  }

private:
  template<typename... Components>
  friend class System;

  ///
  /// Called when a new update is requested. Generates a task for the update.
  ///
  /// @return Update task.
  ///
  virtual Task<> OnUpdate() = 0;

  ///
  /// Returns the registry set for this system.
  ///
  /// @return The registry set for this system.
  ///
  Registry& GetRegistry() const
  {
    ASSERT(registry_ != nullptr, "Registry not set");

    return *registry_;
  }

private:
  Registry* registry_;
};

///
/// Templated system.
///
/// This system computes all its data accesses at compile-time from templated components. If the component is const the
/// access is read-only. This system can only view components that are part of its template arguments.
///
/// @tparam[in] Components Accesses components.
///
template<typename... Components>
class System : public SystemBase
{
private:
  ///
  /// Creates the data access list from template arguments. If the template argument is const, the access will be
  /// read-only.
  ///
  /// @return Data access list for the system.
  ///
  SystemDataAccessList CreateDataAccessList() const final
  {
    SystemDataAccessList list;
    list.Reserve(sizeof...(Components));

    (list.PushBack({ GetComponentId<std::remove_cvref_t<Components>>(), std::is_const_v<Components> }), ...);

    return list;
  }

public:
  ///
  /// Returns the full view of the system. Contains all system component accesses.
  ///
  /// @return Full view of the system.
  ///
  PolyView<Components...> GetView()
  {
    return GetRegistry().template View<Components...>();
  }
};

///
/// Ordered container for systems.
///
class SystemGroup
{
public:
  ///
  /// Constructor.
  ///
  SystemGroup() = default;

  SystemGroup(const SystemGroup&) = delete;
  SystemGroup& operator=(const SystemGroup&) = delete;
  SystemGroup(SystemGroup&&) = delete;
  SystemGroup& operator=(SystemGroup&&) = delete;

  ///
  /// Adds a system to the end of the system list.
  ///
  /// @param[in] system Reference to system.
  ///
  void Add(Ref<SystemBase> system)
  {
    registered_systems_.EmplaceBack(std::move(system));
  }

  ///
  /// Returns the amount of systems currently in the group.
  ///
  /// @return Amount of systems in group.
  ///
  [[nodiscard]] constexpr size_t Count() const noexcept
  {
    return registered_systems_.size();
  }

  ///
  /// Returns an vector of all systems in the group in correct order.
  ///
  /// @return Ordered vector of systems in group.
  ///
  [[nodiscard]] Vector<Ref<SystemBase>> GetSystems() noexcept
  {
    return registered_systems_;
  }

private:
  Vector<Ref<SystemBase>> registered_systems_;
};

} // namespace genebits::engine

#endif
