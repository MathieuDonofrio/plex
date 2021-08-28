
#include "genebits/engine/ecs/archetype_graph.h"

namespace genebits::engine
{
void ArchetypeGraph::AddView(ViewId id)
{
  if (id >= view_to_archetypes.Size()) view_to_archetypes.Resize(id + 1);

  auto& view_components = views[id].components;

  for (ComponentId i = 0; i < archetypes.Size(); i++)
  {
    if (archetypes[i].initialized)
    {
      auto& archetype_components = archetypes[i].components;

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_to_archetypes[id].PushBack(i);

        if (view_components.Size() == archetype_components.Size())
        {
          std::swap(view_to_archetypes[id].front(), view_to_archetypes[id].back());
        }
      }
    }
  }
}

void ArchetypeGraph::AddArchetype(ArchetypeId id)
{
  auto& archetype_components = archetypes[id].components;

  for (ComponentId i = 0; i < views.Size(); i++)
  {
    if (views[i].initialized)
    {
      auto& view_components = views[id].components;

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_to_archetypes[i].PushBack(id);

        if (view_components.Size() == archetype_components.Size())
        {
          std::swap(view_to_archetypes[i].front(), view_to_archetypes[i].back());
        }
      }
    }
  }
}
} // namespace genebits::engine
