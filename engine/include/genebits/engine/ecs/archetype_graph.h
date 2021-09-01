#ifndef GENEBITS_ENGINE_ECS_ARCHETYPE_GRAPH_H
#define GENEBITS_ENGINE_ECS_ARCHETYPE_GRAPH_H

#include "genebits/engine/ecs/archetype.h"

namespace genebits::engine
{
class ArchetypeGraph
{
public:
  template<typename... Components>
  ViewId AssureView()
  {
    const ViewId id = GetViewId<std::remove_cvref_t<Components>...>();

    if (id >= views.Size() || !views[id].initialized)
    {
      std::scoped_lock<std::mutex> lock(mutex);

      Initialize<std::remove_cvref_t<Components>...>(views, id);
      AddView(id);
    }

    return id;
  }

  template<typename... Components>
  ArchetypeId AssureArchetype()
  {
    const ArchetypeId id = GetArchetypeId<std::remove_cvref_t<Components>...>();

    if (id >= archetypes.Size() || !archetypes[id].initialized)
    {
      std::scoped_lock<std::mutex> lock(mutex);

      Initialize<std::remove_cvref_t<Components>...>(archetypes, id);
      AddArchetype(id);
    }

    return id;
  }

  [[nodiscard]] constexpr const FastVector<ArchetypeId>& ViewArchetypes(ViewId id) noexcept
  {
    return view_to_archetypes[id];
  }

private:
  struct ComponentCombinationEntry
  {
    FastVector<ComponentId> components;
    bool initialized = false;
  };

  template<typename... Components>
  static void Initialize(FastVector<ComponentCombinationEntry>& entries, const size_t id)
  {
    if (id >= entries.Size()) entries.Resize(id + 1);

    auto& entry = entries[id];

    entry.components = GetComponentIds<std::remove_cvref_t<Components>...>();

    entry.initialized = true;
  }

  void AddView(ViewId id);

  void AddArchetype(ArchetypeId id);

private:
  FastVector<ComponentCombinationEntry> views;
  FastVector<ComponentCombinationEntry> archetypes;

  FastVector<FastVector<ArchetypeId>> view_to_archetypes;

  std::mutex mutex;
};
} // namespace genebits::engine

#endif
