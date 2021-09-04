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

template<std::unsigned_integral Entity>
class Registry
{
public:
  Registry() = default;

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

  template<typename... Components>
  Entity Create(Components&&... components)
  {
    const Entity entity = manager_.Generate();

    Access<Components...>().template Insert<Components...>(entity, std::forward<Components>(components)...);

    return entity;
  }

  template<typename... Components>
  void Destroy(const Entity entity)
  {
    View<Components...>().Destroy(entity);
  }

  template<typename... Components>
  void DestroyAll()
  {
    View<Components...>().DestroyAll();
  }

  template<typename... Components, typename UnaryFunction>
  void Each(UnaryFunction function)
  {
    return View<Components...>().Each(function);
  }

  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return View<Component>().template Unpack<Component>(entity);
  }

  template<typename Component, typename... Components>
  [[nodiscard]] size_t Size() noexcept
  {
    return View<Component, Components...>().Size();
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    return manager_.CirculatingCount();
  }

  template<typename... Components>
  [[nodiscard]] BasicView<Entity, Components...> View() noexcept
  {
    return BasicView<Entity, Components...>(*this);
  }

private:
  template<std::unsigned_integral Entity, typename... Components>
  friend class BasicView;

  template<typename... Components>
  Storage<Entity>& Access()
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

template<typename Entity, typename Invocable, typename... Components>
concept RegistryExtendedIterationFunc = std::is_invocable_v<Invocable, Entity, Components...>;

template<typename Invocable, typename... Components>
concept RegistryReducedIterationFunc = std::is_invocable_v<Invocable, Components...>;

template<typename Entity, typename Invocable, typename... Components>
concept RegistryIterationFunc = RegistryReducedIterationFunc<Invocable,
  Components...> || RegistryExtendedIterationFunc<Entity, Invocable, Components...>;

template<std::unsigned_integral Entity, typename... Components>
class BasicView
{
public:
  constexpr explicit BasicView(Registry<Entity>& registry) noexcept
    : registry_(registry),
      archetypes_(registry.graph_.ViewArchetypes(registry.graph_.template AssureView<Components...>()))
  {}

  void Destroy(const Entity entity)
  {
#ifndef NDEBUG
    bool found = false;
#endif

    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity))
      {
        storage->Erase(entity);

#ifndef NDEBUG
        found = true;
#endif

        break;
      }
    }

    ASSERT(found, "Entity does not exist in view");

    registry_.manager_.Release(entity);
  }

  void DestroyAll()
  {
    static constexpr bool cDestroyEverything = sizeof...(Components) == 0;

    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if constexpr (!cDestroyEverything)
      {
        const auto end = storage->Size();

        for (auto entity : *storage)
        {
          registry_.manager_.Release(entity);
        }
      }

      storage->Clear();
    }

    if constexpr (cDestroyEverything) registry_.manager_.ReleaseAll();
    else if (registry_.Size() == 0)
    {
      registry_.manager_.ReleaseAll(); // Good because it clears the queue and resets the generator.
    }
  }

  template<typename UnaryFunction>
  requires RegistryIterationFunc<Entity, UnaryFunction, Components...>
  void Each(UnaryFunction function)
  {
    static constexpr bool cExtended = RegistryExtendedIterationFunc<Entity, UnaryFunction, Components...>;

    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      auto data = std::make_tuple(storage->template Access<std::remove_cvref_t<Components>>()...);

      const auto end = storage->Size();

      for (size_t i = 0; i != end; ++i)
      {
        if constexpr (cExtended) function((*storage)[i], std::get<Index<Components, Components...>::value>(data)[i]...);
        else
          function(std::forward<Components>(std::get<Index<Components, Components...>::value>(data)[i])...);
      }
    }
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    size_t sum = 0;

    for (auto archetype : archetypes_)
    {
      const auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      sum += storage->Size();
    }

    return sum;
  }

  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    auto end = archetypes_.end() - 1;

    for (auto it = archetypes_.begin(); it != end; ++it)
    {
      const auto storage = registry_.storages_[*it];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity)) return storage->template Unpack<Component>(entity);
    }

    ASSERT(registry_.storages_[*end], "Storage not initialized");

    return registry_.storages_[*end]->template Unpack<Component>(entity);
  }

  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(const_cast<const BasicView*>(this)->Unpack<Component>(entity));
  }

private:
  template<typename T, typename... Ts>
  struct Index;

  template<typename T, typename... Ts>
  struct Index<T, T, Ts...> : std::integral_constant<std::size_t, 0>
  {};

  template<typename T, typename U, typename... Ts>
  struct Index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value>
  {};

private:
  Registry<Entity>& registry_;
  const FastVector<ArchetypeId>& archetypes_;
};

} // namespace genebits::engine

#endif
