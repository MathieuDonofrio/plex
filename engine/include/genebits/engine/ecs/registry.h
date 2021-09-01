#ifndef GENEBITS_ENGINE_ECS_REGISTRY_H
#define GENEBITS_ENGINE_ECS_REGISTRY_H

#include <algorithm>
#include <concepts>
#include <tuple>

#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/archetype_graph.h"
#include "genebits/engine/ecs/entity_manager.h"
#include "genebits/engine/ecs/storage.h"

namespace genebits::engine
{
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
      storage->template Initialize<Components...>();
    }

    const Entity entity = manager_.Generate();

    storage->template Insert<Components...>(entity, std::forward<Components...>(components)...);

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

  void DestroyAll()
  {
    const ViewId view = graph_.template AssureView<>();

    const auto& archetypes = graph_.ViewArchetypes(view);

    for (const auto archetype : archetypes)
    {
      auto storage = storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      storage->Clear();
    }

    manager_.ReleaseAll();
  }

  template<typename... Components, typename Invokable> // TODO Invokable concept
  void ForEach(const Invokable& invokable)
  {
    const ViewId view = graph_.template AssureView<Components...>();

    const auto& archetypes = graph_.ViewArchetypes(view);

    for (const auto archetype : archetypes)
    {
      auto storage_ptr = storages_[archetype];

      ASSERT(storage_ptr, "Storage not initialized");

      auto& storage = *storage_ptr;

      auto data = std::make_tuple(storage.template Access<Components>()...);

      const auto end = storage.Size();

      for (size_t i = 0; i != end; ++i)
      {
        invokable(storage[i], std::get<FastVector<Components>&>(data)[i]...);
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

  // TODO archetype switching

private:
  SharedSparseArray<Entity> mappings_;
  EntityManager<Entity> manager_;
  ArchetypeGraph graph_;

  FastVector<Storage<Entity>*> storages_;
};

} // namespace genebits::engine

#endif
