#include "plex/ecs/view_relations.h"

namespace plex
{
void ViewRelations::AddView(ViewId id)
{
  if (id >= view_archetypes_.size()) view_archetypes_.resize(id + 1);

  auto& view_components = view_components_[id];

  for (ArchetypeId i = 0; i < archetype_components_.size(); i++)
  {
    if (archetype_states_[i])
    {
      auto& archetype_components = archetype_components_[i];

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_archetypes_[id].push_back(i);

        if (view_components.size() == archetype_components.size())
        {
          // Guarantee exact match O(1) operations
          std::swap(view_archetypes_[id].front(), view_archetypes_[id].back());
        }
      }
    }
  }
}

void ViewRelations::AddArchetype(ArchetypeId id)
{
  auto& archetype_components = archetype_components_[id];

  for (ViewId i = 0; i < view_components_.size(); i++)
  {
    if (view_states_[i])
    {
      auto& view_components = view_components_[i];

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_archetypes_[i].push_back(id);

        if (view_components.size() == archetype_components.size())
        {
          // Guarantee exact match O(1) operations
          std::swap(view_archetypes_[i].front(), view_archetypes_[i].back());
        }
      }
    }
  }
}
} // namespace plex
