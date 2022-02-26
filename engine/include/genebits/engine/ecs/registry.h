#ifndef GENEBITS_ENGINE_ECS_REGISTRY_H
#define GENEBITS_ENGINE_ECS_REGISTRY_H

#include <algorithm>
#include <concepts>
#include <tuple>
#include <type_traits>

#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/entity_manager.h"
#include "genebits/engine/ecs/storage.h"
#include "genebits/engine/ecs/view_relations.h"

namespace genebits::engine
{
///
/// Entity identifier type.
///
/// 32 bit should always be sufficient (~4.2 billion entities).
///
using Entity = uint_fast32_t;

template<typename... Components>
class PolyView;

///
/// High level container for entities of different archetypes.
///
/// The registry is the main interface for using the entity component system. It combines many clever underlying
/// components to create a fast container for entities.
///
/// The registry organizes entities and component data into archetypes. Archetypes are an unordered set of components.
/// Using archetypes allows us to optimize memory layout to make operations faster in most cases.
///
class Registry final
{
public:
  ///
  /// Constructor.
  ///
  Registry() = default;

  ///
  /// Destructor.
  ///
  ~Registry()
  {
    for (auto storage : storages_)
    {
      if (storage) delete storage;
    }
  }

  Registry(const Registry&) = delete;
  Registry(Registry&&) = delete;
  Registry& operator=(const Registry&) = delete;
  Registry& operator=(Registry&&) = delete;

  ///
  /// Creates a new component for the given component data.
  ///
  /// The initial archetype of the component is setup for the list of component types used
  /// during creation.
  ///
  /// This operation is O(1) and very fast.
  ///
  /// @note
  ///    It is recommended that the entity is always created with the exact archetype of its lifetime.
  ///
  /// @tparam Components List of component types used as initial archetype.
  ///
  /// @param[in] components Component data to create entity with.
  ///
  /// @return Identifier of the created entity.
  ///
  template<typename... Components>
  Entity Create(Components&&... components)
  {
    const Entity entity = manager_.Obtain();

    Assure<Components...>().template Insert<Components...>(entity, std::forward<Components>(components)...);

    return entity;
  }

  ///
  /// Destroys the entity and all its associated component data.
  ///
  /// If some or all of the types belonging to the entity's archetype can be provided to reduce the search
  /// space.
  ///
  /// This operation is O(1) all the component types for the entity's archetype are provided. Otherwise,
  /// his operation is O(n) where n is the amount of archetypes with the provided components.
  ///
  /// @warning
  ///    If the specified component types do not belong to the entity, the behaviour of this method
  ///    is undefined.
  ///
  /// @tparam Components Optional partial or complete list of component types of the entity's archetype.
  ///
  /// @param[in] entity Entity to destroy.
  ///
  template<typename... Components>
  void Destroy(const Entity entity)
  {
    View<Components...>().Destroy(entity);
  }

  ///
  /// Destroys all the entities who's archetype contains all of the provided component types.
  ///
  /// @tparam Components List of component types of entities to destroy.
  ///
  template<typename... Components>
  void DestroyAll()
  {
    View<Components...>().DestroyAll();
  }

  ///
  /// Iterates all entities with the provided components and invokes the function with the unpacked
  /// component data.
  ///
  /// @tparam Components Component types to unpack.
  /// @tparam Function Function to invoke.
  ///
  /// @param[in] function Function to invoke for every iteration.
  ///
  template<typename... Components, typename Function>
  void ForEach(Function function)
  {
    return View<Components...>().ForEach(function);
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes with the component.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity)
  {
    return View<Component>().template Unpack<Component>(entity);
  }

  ///
  /// Returns the amount of entities with the specified components.
  ///
  /// @tparam Components Component types to check size for.
  ///
  /// @return Amount of entities with provided component types.
  ///
  template<typename... Components>
  requires(sizeof...(Components) > 0) [[nodiscard]] size_t Size()
  {
    return View<Components...>().Size();
  }

  ///
  /// Returns whether or not the entity has all of the specified components.
  ///
  /// @tparam Components Required component types.
  ///
  /// @param[in] entity Entity to check.
  ///
  /// @return True if the entity has all the components.
  ///
  template<typename... Components>
  bool HasComponents(Entity entity)
  {
    return View<Components...>().Contains(entity);
  }

  ///
  /// Returns the total amount of entities in the registry.
  ///
  /// This is the sum of the sizes of every storage (every archetype) in the registry.
  ///
  /// This operation is O(1) and very fast.
  ///
  /// @return Amount of entities in the registry.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    return manager_.CirculatingCount();
  }

  ///
  /// Obtains a view of the registry for the provided component types.
  ///
  /// @tparam Components Component types to include in the view.
  ///
  /// @return Basic view of the registry for the component types.
  ///
  template<typename... Components>
  [[nodiscard]] PolyView<Components...> View()
  {
    return PolyView<Components...>(*this);
  }

private:
  template<typename...>
  friend class PolyView;

  ///
  /// Assures the storage for the archetype.
  ///
  /// Will properly initialize the storage if it does not exist.
  ///
  /// @tparam Components Component types that compose the archetype.
  ///
  /// @return Reference to assured storage.
  ///
  template<typename... Components>
  Storage<Entity>& Assure()
  {
    const ArchetypeId archetype = relations_.template AssureArchetype<Components...>();

    if (storages_.size() <= archetype) storages_.Resize(archetype + 1, nullptr);

    auto storage = storages_[archetype];

    if (!storage)
    {
      storages_[archetype] = storage = new Storage<Entity>(&mappings_);
      storage->template Initialize<std::remove_cvref_t<Components>...>();
    }

    return *storage;
  }

private:
  SharedSparseArray<Entity> mappings_;
  EntityManager<Entity> manager_;
  ViewRelations relations_;

  Vector<Storage<Entity>*> storages_;
};

///
/// Tuple for all data pointers of an entity.
///
/// They is one pointer for the entity identifier and one pointer for each component.
///
/// @tparam Components All the component types.
///
template<typename... Components>
using EntityData = std::tuple<Entity*, std::remove_cvref_t<Components>*...>;

///
/// Concept used to determine if an functor can be used as an extended entity apply functor.
///
/// To meet the requirements, the functor operator must contain the entity identifier type as
/// the first argument.
///
/// @tparam Functor Functor type.
/// @tparam Components Component types.
///
template<typename Functor, typename... Components>
concept EntityExtendedFunctor = requires(Functor functor, Entity entity, Components... components)
{
  functor(std::forward<Entity>(entity), std::forward<Components>(components)...);
};

///
/// Concept used to determine if an functor can be used as an reduced entity apply functor.
///
/// To meet the requirements, the functor operator must not contain the entity identifier.
///
/// @tparam Functor Functor type.
/// @tparam Components Component types.
///
template<typename Functor, typename... Components>
concept EntityReducedFunctor = requires(Functor functor, Components... components)
{
  functor(std::forward<Components>(components)...);
};

///
/// Concept used to determine if an functor can be used as an entity apply functor.
///
/// To meet the requirements, must be EntityReducedFunctor or EntityExtendedFunctor.
///
/// @tparam Functor Functor type.
/// @tparam Components Component types.
///
template<typename Functor, typename... Components>
concept EntityFunctor = EntityReducedFunctor<Functor, Components...> || EntityExtendedFunctor<Functor, Components...>;

///
/// Unpacks the entity identifier and entity data, then invokes the functor.
///
/// Overload used for functors such as lambda's.
///
/// @tparam Functor Type of functor to apply.
/// @tparam Components Component types of the entity data.
///
/// @param[in] invocable Invocable to apply.
/// @param[in] data Entity data.
///
template<typename Functor, typename... Components>
requires EntityExtendedFunctor<Functor, Components...>
constexpr void EntityApply(Functor& func, EntityData<Components...>& data)
{
  func(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity data, then invokes the functor.
///
/// Overload used for functors such as lambda's.
///
/// @tparam Functor Type of functor to apply.
/// @tparam Components Component types of the entity data.
///
/// @param[in] invocable Invocable to apply.
/// @param[in] data Entity data.
///
template<typename Functor, typename... Components>
requires EntityReducedFunctor<Functor, Components...>
constexpr void EntityApply(Functor& func, EntityData<Components...>& data)
{
  func(*std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity identifier and entity data, then invokes the member function.
///
/// Overload used for const member function.
///
/// @tparam Type Type of instance to invoke member function with.
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Member function to apply.
/// @param[in] instance Instance to invoke function on.
/// @param[in] data Entity data.
///
template<class Type, typename... Components>
constexpr void EntityApply(
  void (Type::*func)(Entity, Components...) const, Type* instance, EntityData<Components...>& data)
{
  instance->*func(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity identifier and entity data, then invokes the member function.
///
/// Overload used for member function.
///
/// @tparam Type Type of instance to invoke member function with.
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Member function to apply.
/// @param[in] instance Instance to invoke function on.
/// @param[in] data Entity data.
///
template<class Type, typename... Components>
constexpr void EntityApply(void (Type::*func)(Entity, Components...), Type* instance, EntityData<Components...>& data)
{
  instance->*func(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity data, then invokes the member function.
///
/// Overload used for const member function.
///
/// @tparam Type Type of instance to invoke member function with.
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Member function to apply.
/// @param[in] instance Instance to invoke function on.
/// @param[in] data Entity data.
///
template<class Type, typename... Components>
constexpr void EntityApply(void (Type::*func)(Components...) const, Type* instance, EntityData<Components...>& data)
{
  instance->*func(*std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity data, then invokes the member function.
///
/// Overload used for member function.
///
/// @tparam Type Type of instance to invoke member function with.
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Member function to apply.
/// @param[in] instance Instance to invoke function on.
/// @param[in] data Entity data.
///
template<class Type, typename... Components>
constexpr void EntityApply(void (Type::*func)(Components...), Type* instance, EntityData<Components...>& data)
{
  instance->*func(*std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity identifier and entity data, then invokes the function.
///
/// Overload used for free functions.
///
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Function to apply.
/// @param[in] data Entity data.
///
template<typename... Components>
constexpr void EntityApply(void (*func)(Entity, Components...), EntityData<Components...>& data)
{
  func(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity data, then invokes the function.
///
/// Overload used for free functions.
///
/// @tparam Components Component types of the entity data.
///
/// @param[in] func Function to apply.
/// @param[in] data Entity data.
///
template<typename... Components>
constexpr void EntityApply(void (*func)(Components...), EntityData<Components...>& data)
{
  func(*std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// View that contains entities of a single archetype that contains all the components in the view.
///
/// @tparam Components Component types of the view.
///
template<typename... Components>
class MonoView
{
public:
  ///
  /// Iterates all entities in the view, unpacks the component data efficiently and invokes the function.
  ///
  /// Iteration is very fast with near vector speeds.
  ///
  /// @tparam Function Function type to invoke for each entity.
  ///
  /// @param[in] function Function to invoke for each entity.
  ///
  template<typename Function>
  requires EntityFunctor<Function, Components...>
  void ForEach(Function function)
  {
    for (auto& data : *this)
    {
      EntityApply<Function, Components...>(function, data);
    }
  }

  ///
  /// Checks if the entity is in the view.
  ///
  /// @param[in] entity Entity to find.
  ///
  /// @returns True if the entity exists in the view, false otherwise.
  ///
  bool Contains(const Entity entity) const noexcept
  {
    return storage_->Contains(entity);
  }

  ///
  /// Returns the amount of entities in the view.
  ///
  /// @return Amount of entities in the view.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    return storage_->Size();
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist in the view");

    return storage_->template Unpack<Component>(entity);
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(static_cast<const MonoView*>(this)->Unpack<Component>(entity));
  }

public:
  ///
  /// Entity iterator. Iterates on a selection of entities and components from a single archetype
  /// storage.
  ///
  class Iterator
  {
  public:
    using Data = EntityData<Components...>;

    ///
    /// Constructs an entity iterator using data.
    ///
    /// @param[in] data Data for iterator.
    ///
    constexpr Iterator(Data data) noexcept : data_(data) {}

    ///
    /// Copy constructor.
    ///
    /// @param[in] other Iterator to copy.
    ///
    constexpr Iterator(const Iterator& other) noexcept : data_(other.data_) {}

    ///
    /// Copy assignment operator.
    ///
    /// @param[in] other Iterator to assign.
    ///
    /// @return Reference to assigned iterator.
    ///
    constexpr Iterator& operator=(const Iterator& other) noexcept
    {
      data_ = other.data_;
      return *this;
    }

    ///
    /// Add-assign operator.
    ///
    /// @return Reference to iterator after add.
    ///
    constexpr Iterator& operator+=(size_t amount) noexcept
    {
      std::get<0>(data_) += amount;
      ((std::get<std::remove_cvref_t<Components>*>(data_) += amount), ...);
      return *this;
    }

    ///
    /// Subtract-assign operator.
    ///
    /// @return Reference to iterator after add.
    ///
    constexpr Iterator& operator-=(size_t amount) noexcept
    {
      std::get<0>(data_) -= amount;
      ((std::get<std::remove_cvref_t<Components>*>(data_) -= amount), ...);
      return *this;
    }

    ///
    /// Pre-increment operator.
    ///
    /// @return Reference to iterator after increment.
    ///
    constexpr Iterator& operator++() noexcept
    {
      ++std::get<0>(data_);
      (++std::get<std::remove_cvref_t<Components>*>(data_), ...);
      return *this;
    }

    ///
    /// Pre-decrement operator.
    ///
    /// @return Reference to iterator before decrement.
    ///
    constexpr Iterator& operator--() noexcept
    {
      --std::get<0>(data_);
      (--std::get<std::remove_cvref_t<Components>*>(data_), ...);
      return *this;
    }

    ///
    /// Post-increment operator.
    ///
    /// @return Copy of iterator before increment.
    ///
    constexpr const Iterator operator++(int) noexcept
    {
      Iterator copy(*this);
      operator++();
      return copy;
    }

    ///
    /// Post-decrement operator.
    ///
    /// @return Copy of iterator before decrement.
    ///
    constexpr const Iterator operator--(int) noexcept
    {
      Iterator copy(*this);
      operator--();
      return copy;
    }

    ///
    /// Add operator.
    ///
    /// @return Result.
    ///
    constexpr Iterator operator+(size_t amount) const noexcept
    {
      return Iterator(std::make_tuple<Entity*, std::remove_cvref_t<Components>*...>(
        std::get<0>(data_) + amount, (std::get<std::remove_cvref_t<Components>*>(data_) + amount)...));
    }

    ///
    /// Subtract operator.
    ///
    /// @return Result.
    ///
    constexpr Iterator operator-(size_t amount) const noexcept
    {
      return Iterator(std::make_tuple<Entity*, std::remove_cvref_t<Components>*...>(
        std::get<0>(data_) - amount, (std::get<std::remove_cvref_t<Components>*>(data_) - amount)...));
    }

    ///
    /// Returns reference to entity data.
    ///
    /// @return Reference to data.
    ///
    constexpr const Data& operator*() const noexcept
    {
      return data_;
    }

    ///
    /// Returns reference to entity data.
    ///
    /// @return Reference to data.
    ///
    constexpr Data& operator*() noexcept
    {
      return data_;
    }

    ///
    /// Returns pointer to entity data.
    ///
    /// @return Pointer to data.
    ///
    constexpr const Data* operator->() const noexcept
    {
      return *data_;
    }

    ///
    /// Returns pointer to entity data.
    ///
    /// @return Pointer to data.
    ///
    constexpr Data* operator->() noexcept
    {
      return *data_;
    }

    ///
    /// Compares the iterator to another iterator.
    ///
    /// @param other Entity iterator.
    ///
    /// @return True if iterator is equal to other iterator, false otherwise.
    ///
    constexpr bool operator==(const Iterator& other) const noexcept
    {
      return std::get<0>(data_) == std::get<0>(other.data_);
    }

    ///
    /// Compares the iterator to an entity iterator.
    ///
    /// @param other Entity iterator.
    ///
    /// @return True if iterator is not equal to other iterator, false otherwise..
    ///
    constexpr bool operator!=(const Iterator& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    template<typename...>
    friend class MonoView;

    ///
    /// Constructs an entity iterator using a storage and an offset.
    ///
    /// @param[in] storage Storage to iterate on.
    /// @param[in] offset Offset of the iteration in the entity array of the storage.
    ///
    constexpr Iterator(Storage<Entity>* storage, size_t offset) noexcept
      : data_(
        storage->data() + offset, (storage->template Access<std::remove_cvref_t<Components>>().data() + offset)...)
    {}

  private:
    Data data_;
  };

  // Style Exception: STL
  // clang-format off

  Iterator begin() noexcept { return Iterator(storage_, 0); }
  Iterator end() noexcept { return Iterator(storage_, storage_->Size()); }

  Entity* ebegin() noexcept { return storage_->begin(); }
  Entity* eend() noexcept { return storage_->end(); }

  // clang-format on

private:
  template<typename...>
  friend class PolyView;

  ///
  /// Constructs a view from a storage.
  ///
  /// @param[in] storage Storage to construct view with.
  ///
  constexpr MonoView(Storage<Entity>* storage) : storage_(storage) {}

private:
  Storage<Entity>* storage_;
};

///
/// View that contains entities of all archetypes that contain all the components in the view.
///
/// @warning
///     This view only guarantees that it will contain the archetypes that meet its requirements
///     at the time of creation. If a new archetype is created with all the required components after
///     this view was created, it will not be in the view. For this reason, it is good practice to
///     recreate views when needed.
///
/// @tparam Components Required component types for the view.
///
template<typename... Components>
class PolyView
{
public:
  static constexpr bool cNoComponents = sizeof...(Components) == 0;

  ///
  /// Constructor.
  ///
  /// @param[in] registry Registry to construct view for.
  ///
  constexpr explicit PolyView(Registry& registry)
    : registry_(registry),
      archetypes_(registry.relations_.ViewArchetypes(registry.relations_.template AssureView<Components...>()))
  {}

  ///
  /// Iterates all entities in the view, unpacks the component data efficiently and invokes the function.
  ///
  /// Iteration is very fast with near vector speeds.
  ///
  /// There is some overhead due to having multiple storages to iterate on. This causes "gaps" between contiguous blocks
  /// of memory and may cause cache misses in views with a high archetype to entity ratio.
  ///
  /// @tparam Function Function type to invoke for each entity.
  ///
  /// @param[in] function Function to invoke for each entity.
  ///
  template<typename Function>
  requires EntityFunctor<Function, Components...>
  void ForEach(Function function)
  {
    for (auto mono_view : *this)
    {
      for (auto& data : mono_view)
      {
        EntityApply<Function, Components...>(function, data);
      }
    }
  }

  ///
  /// Destroys the entity and all its associated component data.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view. A reordering optimization
  /// ensures that the operation is O(1) if the view has the exact components of the entity to destroy.
  ///
  /// @warning
  ///    If the view does not contain the entity, the behaviour of this method is undefined.
  ///
  /// @param[in] entity Entity to destroy.
  ///
  void Destroy(const Entity entity)
  {
    ASSERT(Contains(entity), "Entity does not exist in view");

    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity))
      {
        storage->Erase(entity);

        registry_.manager_.Release(entity);

        return;
      }
    }
  }

  ///
  /// Destroys all entities in the view.
  ///
  /// This operation is O(n) most of the time. If the view has no components this operation can be O(1) if every
  /// component in the registry is trivially destructible.
  ///
  void DestroyAll()
  {
    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if constexpr (!cNoComponents)
      {
        for (auto entity : *storage)
        {
          registry_.manager_.Release(entity);
        }
      }

      storage->Clear();
    }

    if constexpr (cNoComponents) registry_.manager_.ReleaseAll(); // Release everything
    else if (registry_.Size() == 0)
    {
      registry_.manager_.ReleaseAll(); // Good because it clears the queue and resets the generator.
    }
  }

  ///
  /// Checks if the entity is in the view.
  ///
  /// @param[in] entity Entity to find.
  ///
  /// @returns True if the entity exists in the view, false otherwise.
  ///
  bool Contains(const Entity entity) const noexcept
  {
    for (const auto archetype : archetypes_)
    {
      const auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity)) return true;
    }

    return false;
  }

  ///
  /// Returns the amount of entities in the view.
  ///
  /// @return Amount of entities in the view.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    if constexpr (!cNoComponents)
    {
      size_t sum = 0;

      for (const auto archetype : archetypes_)
      {
        const auto storage = registry_.storages_[archetype];

        ASSERT(storage, "Storage not initialized");

        sum += storage->Size();
      }

      return sum;
    }
    else
    {
      return registry_.Size();
    }
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist in the view");

    const auto end = archetypes_.end() - 1;

    for (auto it = archetypes_.begin(); it != end; ++it)
    {
      const auto storage = registry_.storages_[*it];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity)) return storage->template Unpack<Component>(entity);
    }

    ASSERT(registry_.storages_[*end], "Storage not initialized");

    return registry_.storages_[*end]->template Unpack<Component>(entity);
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(static_cast<const PolyView*>(this)->Unpack<Component>(entity));
  }

public:
  ///
  /// Poly view iterator. Iterates on single views in the view.
  ///
  class Iterator
  {
  public:
    ///
    /// Add-assign operator.
    ///
    /// @return Reference to iterator after add.
    ///
    constexpr Iterator& operator+=(size_t amount) noexcept
    {
      archetype_ += amount;
      return *this;
    }

    ///
    /// Subtract-assign operator.
    ///
    /// @return Reference to iterator after add.
    ///
    constexpr Iterator& operator-=(size_t amount) noexcept
    {
      archetype_ -= amount;
      return *this;
    }

    ///
    /// Pre-increment operator.
    ///
    /// @return Reference to iterator before increment.
    ///
    constexpr Iterator& operator++() noexcept
    {
      return ++archetype_, *this;
    }

    ///
    /// Pre-decrement operator.
    ///
    /// @return Reference to iterator before decrement.
    ///
    constexpr Iterator& operator--() noexcept
    {
      return --archetype_, *this;
    }

    ///
    /// Post-increment operator.
    ///
    /// @return Copy of iterator before increment.
    ///
    constexpr const Iterator operator++(int) noexcept
    {
      Iterator copy(*this);
      operator++();
      return copy;
    }

    ///
    /// Post-decrement operator.
    ///
    /// @return Copy of iterator before decrement.
    ///
    constexpr const Iterator operator--(int) noexcept
    {
      Iterator copy(*this);
      operator--();
      return copy;
    }

    ///
    /// Add operator.
    ///
    /// @return Result.
    ///
    constexpr Iterator operator+(size_t amount) const noexcept
    {
      return Iterator(archetype_ + amount, registry_);
    }

    ///
    /// Subtract operator.
    ///
    /// @return Result.
    ///
    constexpr Iterator operator-(size_t amount) const noexcept
    {
      return Iterator(archetype_ - amount, registry_);
    }

    ///
    /// Returns the reference to the storage.
    ///
    /// @return Storage reference.
    ///
    constexpr MonoView<Components...> operator*() const noexcept
    {
      return MonoView<Components...> { registry_.storages_[*archetype_] };
    }

    ///
    /// Returns whether or not iterators are equal.
    ///
    /// @param[in] other Iterator to compare.
    ///
    /// @return True if both iterators are equal, false otherwise.
    ///
    constexpr bool operator==(const Iterator& other) const noexcept
    {
      return archetype_ == other.archetype_;
    }

    ///
    /// Returns whether or not iterators are equal.
    ///
    /// @param[in] other Iterator to compare.
    ///
    /// @return True if both iterators are not equal, false otherwise.
    ///
    constexpr bool operator!=(const Iterator& other) const noexcept
    {
      return archetype_ != other.archetype_;
    }

  private:
    template<typename...>
    friend class PolyView;

    ///
    /// Constructs an iterator with the archetype array and a registry reference.
    ///
    /// @param[in] archetype Archetype
    ///
    /// @param[in] registry Registry to obtain storages from.
    ///
    constexpr Iterator(const ArchetypeId* archetype, Registry& registry) noexcept
      : archetype_(archetype), registry_(registry)
    {}

  private:
    const ArchetypeId* archetype_;
    Registry& registry_;
  };

  // Style Exception: STL
  // clang-format off

  [[nodiscard]] Iterator begin() { return Iterator(archetypes_.begin(), registry_); }
  [[nodiscard]] Iterator end() { return Iterator(archetypes_.end(), registry_); }

  // clang-format on

private:
  Registry& registry_;
  const Vector<ArchetypeId>& archetypes_;
};

} // namespace genebits::engine

#endif
