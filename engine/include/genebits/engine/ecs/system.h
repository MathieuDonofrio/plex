#ifndef GENEBITS_ENGINE_ECS_SYSTEM_H
#define GENEBITS_ENGINE_ECS_SYSTEM_H

#include <list>
#include <set>
#include <span>

#include "genebits/engine/async/async_latch.h"
#include "genebits/engine/async/shared_task.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/async/thread_pool.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/registry.h"
#include "genebits/engine/os/allocator.h"
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

class SystemDataAccessProvider
{
public:
  constexpr SystemDataAccessProvider() noexcept : initialized_(false) {}

  virtual ~SystemDataAccessProvider() = default;

  [[nodiscard]] const SystemDataAccessList& GetDataAccess() const noexcept
  {
    if (!initialized_) [[unlikely]]
    {
      // Resolve data access only once
      new (&access_) SystemDataAccessList(std::move(CreateDataAccessList()));

      initialized_ = true;
    }

    return access_;
  }

private:
  virtual SystemDataAccessList CreateDataAccessList() const = 0;

private:
  mutable SystemDataAccessList access_;
  mutable bool initialized_;
};

struct SystemContext
{
  Registry* registry = nullptr;
  ThreadPool* thread_pool = nullptr;
};

using SystemTask = SharedTask<>;

template<typename... Components>
class System;

class SystemGroup;

class SystemBase : public SystemDataAccessProvider
{
public:
  SystemBase() = default;

  virtual ~SystemBase() = default;

  SystemBase(const SystemBase&) = delete;
  SystemBase& operator=(const SystemBase&) = delete;
  SystemBase(SystemBase&&) = delete;
  SystemBase& operator=(SystemBase&&) = delete;

  Task<> Update()
  {
    return OnUpdate();
  }

protected:
  const SystemContext& GetContext() const noexcept
  {
    return context_;
  }

private:
  virtual Task<> OnUpdate() = 0;

  virtual void OnActivate() {}

  virtual void OnDeactivate() {}

private:
  SystemContext context_; // TODO

  SystemTask update_task_;
};

template<typename... Components>
class System : public SystemBase
{
private:
  SystemDataAccessList CreateDataAccessList() const final
  {
    SystemDataAccessList list;
    list.Reserve(sizeof...(Components));

    (list.PushBack({ GetComponentId<std::remove_cvref_t<Components>>(), std::is_const_v<Components> }), ...);

    return list;
  }

public:
  // Public interface

  PolyView<Components...> GetView()
  {
    return GetContext().registry->template View<Components...>();
  }

  decltype(auto) Schedule() // TODO Make more generalized scheduling
  {
    return GetContext().thread_pool->Schedule();
  }
};

class SystemGroup
{
public:
  SystemGroup() = default;

  SystemGroup(const SystemGroup&) = delete;
  SystemGroup& operator=(const SystemGroup&) = delete;
  SystemGroup(SystemGroup&&) = delete;
  SystemGroup& operator=(SystemGroup&&) = delete;

  void Add(Ref<SystemBase> system)
  {
    registered_systems_.PushBack(std::move(system));
  }

  [[nodiscard]] constexpr size_t Count() const noexcept
  {
    return registered_systems_.size();
  }

  [[nodiscard]] constexpr Ref<SystemBase>* RawSystems() noexcept
  {
    return registered_systems_.data();
  }

private:
  Vector<Ref<SystemBase>> registered_systems_;
};

} // namespace genebits::engine

#endif
