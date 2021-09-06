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
template<std::unsigned_integral Entity, typename... Components>
class BasicView;

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
template<std::unsigned_integral Entity>
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
  /// @tparam UnaryFunction Function to invoke.
  ///
  /// @param[in] function Function to invoke for every iteration.
  ///
  template<typename... Components, typename UnaryFunction>
  void ForEach(UnaryFunction function)
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
  [[nodiscard]] BasicView<Entity, Components...> View()
  {
    return BasicView<Entity, Components...>(*this);
  }

private:
  template<std::unsigned_integral, typename...>
  friend class BasicView;

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
concept ViewExtendedIterationFunc = requires(Invocable invocable, Entity entity, Components... components)
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
concept ViewReducedIterationFunc = requires(Invocable invocable, Components... components)
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
concept ViewIterationFunc =
  ViewReducedIterationFunc<Invocable, Components...> || ViewExtendedIterationFunc<Entity, Invocable, Components...>;

///
/// View of a registry.
///
/// Views are a reduces space of entities. They reduce the space by giving it component requirements. In other words,
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
class BasicView
{
public:
  static constexpr bool cNoComponents = sizeof...(Components) == 0;

  ///
  /// Constructor.
  ///
  /// @param[in] registry Registry to construct view for.
  ///
  constexpr explicit BasicView(Registry<Entity>& registry)
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
  /// @tparam UnaryFunction Function type to invoke for each entity.
  ///
  /// @param[in] function Function to invoke for each entity.
  ///
  template<typename UnaryFunction>
  requires ViewIterationFunc<Entity, UnaryFunction, Components...>
  void ForEach(UnaryFunction function)
  {
    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if constexpr (ViewExtendedIterationFunc<Entity, UnaryFunction, Components...>)
      {
        // Extended iteration (With entity identifier)

        auto data =
          std::make_tuple(storage->begin(), storage->template Access<std::remove_cvref_t<Components>>().begin()...);

        const auto end = storage->end();

        while (std::get<0>(data) != end)
        {
          function(*std::get<0>(data), *std::get<Index<Components, Components...>::value + 1>(data)...);

          ++std::get<0>(data);
          ((++std::get<Index<Components, Components...>::value + 1>(data)), ...);
        }
      }
      else if constexpr (!cNoComponents)
      {
        // Reduced iteration (Without entity identifier)

        auto data = std::make_tuple(storage->template Access<std::remove_cvref_t<Components>>().begin()...);

        const auto end = std::get<0>(data) + storage->Size();

        while (std::get<0>(data) != end)
        {
          function(*std::get<Index<Components, Components...>::value>(data)...);

          ((++std::get<Index<Components, Components...>::value>(data)), ...);
        }
      }
      else
      {
        // Void iteration (No unpacking)

        const auto end = storage->Size();

        for (size_t i = 0; i != end; ++i)
        {
          function();
        }
      }
    }
  }

  ///
  /// Destroys the entity and all its associated component data.
  ///
  /// his operation is O(n) where n is the amount of archetypes in the view. A reordering optimization
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
    return const_cast<Component&>(const_cast<const BasicView*>(this)->Unpack<Component>(entity));
  }

private:
  ///
  /// Utility used for obtaining the index of the type in a variadic template.
  ///
  /// @tparam Type Type to obtain index for.
  /// @tparam List List of types to search for index.
  ///
  template<typename Type, typename... List>
  struct Index;

  template<typename Type, typename... List>
  struct Index<Type, Type, List...> : std::integral_constant<std::size_t, 0>
  {};

  template<typename Type1, typename Type2, typename... List>
  struct Index<Type1, Type2, List...> : std::integral_constant<std::size_t, 1 + Index<Type1, List...>::value>
  {};

private:
  Registry<Entity>& registry_;
  const FastVector<ArchetypeId>& archetypes_;
};

} // namespace genebits::engine

#endif
