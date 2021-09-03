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

template<typename T, typename... Ts>
struct Index;

template<typename T, typename... Ts>
struct Index<T, T, Ts...> : std::integral_constant<std::size_t, 0>
{};

template<typename T, typename U, typename... Ts>
struct Index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value>
{};

template<typename Entity, typename Invocable, typename... Components>
concept RegistryExtendedIterationFunc = std::is_invocable_v<Invocable, Entity, Components...>;

template<typename Invocable, typename... Components>
concept RegistryReducedIterationFunc = std::is_invocable_v<Invocable, Components...>;

template<typename Entity, typename Invocable, typename... Components>
concept RegistryIterationFunc = RegistryReducedIterationFunc<Invocable,
  Components...> || RegistryExtendedIterationFunc<Entity, Invocable, Components...>;

template<std::unsigned_integral Entity>
class Registry
{
public:
  ~Registry()
  {
    for (auto storage : storages_)
    {
      if (storage) delete storage;
    }
  }

  template<typename... Components>
  Entity Create(Components&&... components)
  {
    const ArchetypeId archetype = graph_.template AssureArchetype<Components...>();

    if (storages_.Size() <= archetype) storages_.Resize(archetype + 1, nullptr);

    auto& storage = storages_[archetype];

    if (!storage)
    {
      storage = new Storage<Entity>(&mappings_);
      storage->template Initialize<std::remove_cvref_t<Components>...>();
    }

    const Entity entity = manager_.Generate();

    storage->template Insert<Components...>(entity, std::forward<Components>(components)...);

    return entity;
  }

  template<typename... Components>
  void Destroy(const Entity entity)
  {
    const ViewId view = graph_.template AssureView<Components...>();

    const auto& archetypes = graph_.ViewArchetypes(view);

#ifndef NDEBUG
    bool found = false;
#endif

    for (const auto archetype : archetypes)
    {
      auto storage = storages_[archetype];

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

    manager_.Release(entity);
  }

  template<typename... Components>
  void DestroyAll()
  {
    static constexpr bool cDestroyEverything = sizeof...(Components) == 0;

    const ViewId view = graph_.template AssureView<Components...>();

    const auto& archetypes = graph_.ViewArchetypes(view);

    for (const auto archetype : archetypes)
    {
      auto storage = storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if constexpr (!cDestroyEverything)
      {
        const auto end = storage->Size();

        for (auto entity : *storage)
        {
          manager_.Release(entity);
        }
      }

      storage->Clear();
    }

    if constexpr (cDestroyEverything) manager_.ReleaseAll();
    else if (Size() == 0)
      manager_.ReleaseAll(); // Good because it clears the queue and resets the generator.
  }

  template<typename... Components, typename UnaryFunction>
  requires RegistryIterationFunc<Entity, UnaryFunction, Components...>
  void ForEach(UnaryFunction function)
  {
    static constexpr bool cExtended = RegistryExtendedIterationFunc<Entity, UnaryFunction, Components...>;

    const ViewId view = graph_.template AssureView<Components...>();

    const auto& archetypes = graph_.ViewArchetypes(view);

    for (const auto archetype : archetypes)
    {
      auto storage = storages_[archetype];

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

  template<typename Component, typename... Components>
  [[nodiscard]] size_t Size() noexcept
  {
    const ViewId view = graph_.template AssureView<Component, Components...>();

    const auto& archetypes = graph_.ViewArchetypes(view);

    size_t sum = 0;

    for (auto archetype : archetypes)
    {
      const auto storage = storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      sum += storage->Size();
    }

    return sum;
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    return manager_.CirculatingCount();
  }

  // TODO Expensive unpacking

  // TODO archetype switching
private:
  SharedSparseArray<Entity> mappings_;
  EntityManager<Entity> manager_;
  ArchetypeGraph graph_;

  FastVector<Storage<Entity>*> storages_;
};

} // namespace genebits::engine

#endif
