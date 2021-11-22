#ifndef GENEBITS_ENGINE_ECS_REGISTRY_H
#define GENEBITS_ENGINE_ECS_REGISTRY_H

#include <algorithm>
#include <concepts>
#include <tuple>
#include <type_traits>

#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/archetype_graph.h"
#include "genebits/engine/ecs/entity_manager.h"
#include "genebits/engine/ecs/storage.h"

namespace genebits::engine
{
///
/// Recommended entity type.
///
using Entity = uint_fast32_t;

template<std::unsigned_integral Entity, typename... Components>
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
/// @tparam Entity Entity integral type.
///
template<std::unsigned_integral Entity = Entity>
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
  ///    This method is the slowest way to unpack and should not be used in performance critical paths.
  ///    This method exists only for the sake of simplicity. Prefer unpacking during iteration or constructing
  ///    a view and unpacking with reduced search space.
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
  [[nodiscard]] PolyView<Entity, Components...> View()
  {
    return PolyView<Entity, Components...>(*this);
  }

private:
  template<std::unsigned_integral, typename...>
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
    const ArchetypeId archetype = graph_.template AssureArchetype<Components...>();

    if (storages_.Size() <= archetype) storages_.Resize(archetype + 1, nullptr);

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
  ArchetypeGraph graph_;

  FastVector<Storage<Entity>*> storages_;
};

///
/// Concept used to determine if an invokable can be used as an extended iteration function in the view.
///
/// The function be invocable with the entity as the first argument and all components after.
///
/// @tparam Entity Entity integral type.
/// @tparam Invocable Invocable functor type.
/// @tparam Components Component types of the view.
///
template<typename Entity, typename Invocable, typename... Components>
concept ExtendedUnpackFunction = requires(Invocable invocable, Entity entity, Components... components)
{
  invocable(entity, components...);
};

///
/// Concept used to determine if an invokable can be used as an reduced iteration function in the view.
///
/// The function be invocable with all components as the only arguments.
///
/// @tparam Invocable Invocable functor type.
/// @tparam Components Component types of the view.
///
template<typename Invocable, typename... Components>
concept ReducedUnpackFunction = requires(Invocable invocable, Components... components)
{
  invocable(components...);
};

///
/// Concept used to determine if an invocable can be used as an iteration function in the view.
///
/// Requires that the function either meets the requirements of either the reduced function or the extended function.
///
/// @tparam Entity Entity integral type.
/// @tparam Invocable Invocable functor type.
/// @tparam Components Component types of the view.
///
template<typename Entity, typename Invocable, typename... Components>
concept UnpackFunction =
  ReducedUnpackFunction<Invocable, Components...> || ExtendedUnpackFunction<Entity, Invocable, Components...>;

///
/// Unpacks the data from a tuple and invokes a function with it.
///
/// Entity data is usually obtained from view iteration.
///
/// @tparam Entity Entity type.
/// @tparam Function Function type.
/// @tparam Components Components to unpack.
///
/// @param[in] function Instance of function to invoke.
/// @param[in] data Data to unpack.
///
template<std::unsigned_integral Entity, typename Function, typename... Components>
requires UnpackFunction<Entity, Function, Components...>
constexpr void UnpackAndApply(
  Function& function, [[maybe_unused]] std::tuple<Entity*, std::remove_cvref_t<Components>*...>& data)
{
  if constexpr (ExtendedUnpackFunction<Entity, Function, Components...>)
  {
    function(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
  }
  else
  {
    function(*std::get<std::remove_cvref_t<Components>*>(data)...);
  }
}

///
/// View of selected components from a single archetype storage.
///
/// @note For view across archetypes, see PolyView.
///
/// @tparam Entity Entity type.
/// @tparam Components Components to unpack.
///
template<std::unsigned_integral Entity, typename... Components>
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
  requires UnpackFunction<Entity, Function, Components...>
  void ForEach(Function function)
  {
    for (auto& data : *this)
    {
      UnpackAndApply<Entity, Function, Components...>(function, data);
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
  ///    This method is much slower than the unpacking that takes place during iteration. However, this is faster than
  ///    unpacking using the registry in most cases.
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
  ///    This method is much slower than the unpacking that takes place during iteration. However, this is faster than
  ///    unpacking using the registry in most cases.
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
    using Data = std::tuple<Entity*, std::remove_cvref_t<Components>*...>;

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
    template<std::unsigned_integral, typename...>
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
  template<std::unsigned_integral, typename...>
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
/// View of a registry.
///
/// Views are a reduced space of entities. They reduce the space by giving it component requirements. In other words,
/// for an entity to be part of a view it must contain all the component types in the view.
///
/// The registry operates on views to reduce the search space for many operations. Sometimes the registry does not
/// construct the optimal views for the task, and most methods are oriented towards simplicity. It is recommended that
/// you construct the view yourself in performance critical paths to make sure that you have reduced the search space as
/// small as possible.
///
/// The biggest use case for views is for iteration over a set of components.
///
/// @warning
///  This view caches the archetypes, this means that the view must be reconstructed when new archetypes are added.
///  Be careful when reusing views for long periods of time. If your not sure about when to reconstruct the view,
///  reconstruct it for every operation, the overhead is quite low.
///
/// @tparam Components Component types in the view.
///
template<std::unsigned_integral Entity, typename... Components>
class PolyView
{
public:
  static constexpr bool cNoComponents = sizeof...(Components) == 0;

  ///
  /// Constructor.
  ///
  /// @param[in] registry Registry to construct view for.
  ///
  constexpr explicit PolyView(Registry<Entity>& registry)
    : registry_(registry),
      archetypes_(registry.graph_.ViewArchetypes(registry.graph_.template AssureView<Components...>()))
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
  requires UnpackFunction<Entity, Function, Components...>
  void ForEach(Function function)
  {
    for (auto mono_view : *this)
    {
      for (auto& data : mono_view)
      {
        UnpackAndApply<Entity, Function, Components...>(function, data);
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
  ///    This method is much slower than the unpacking that takes place during iteration. However, this is faster than
  ///    unpacking using the registry in most cases.
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
  ///    This method is much slower than the unpacking that takes place during iteration. However, this is faster than
  ///    unpacking using the registry in most cases.
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
    constexpr MonoView<Entity, Components...> operator*() const noexcept
    {
      return MonoView<Entity, Components...> { registry_.storages_[*archetype_] };
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
    template<std::unsigned_integral, typename...>
    friend class PolyView;

    ///
    /// Constructs an iterator with the archetype array and a registry reference.
    ///
    /// @param[in] archetype Archetype
    ///
    /// @param[in] registry Registry to obtain storages from.
    ///
    constexpr Iterator(const ArchetypeId* archetype, Registry<Entity>& registry) noexcept
      : archetype_(archetype), registry_(registry)
    {}

  private:
    const ArchetypeId* archetype_;
    Registry<Entity>& registry_;
  };

  // Style Exception: STL
  // clang-format off

  [[nodiscard]] Iterator begin() { return Iterator(archetypes_.begin(), registry_); }
  [[nodiscard]] Iterator end() { return Iterator(archetypes_.end(), registry_); }

  // clang-format on

private:
  Registry<Entity>& registry_;
  const FastVector<ArchetypeId>& archetypes_;
};

} // namespace genebits::engine

#endif
