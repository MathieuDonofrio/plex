#include "genebits/engine/ecs/view_relations.h"

namespace genebits::engine
{
void ViewRelations::AddView(ViewId id)
{
  if (id >= view_archetypes_.size()) view_archetypes_.Resize(id + 1);

  auto& view_components = view_components_[id];

  for (ArchetypeId i = 0; i < archetype_states_.size(); i++)
  {
    if (archetype_states_[i])
    {
      auto& archetype_components = archetype_components_[i];

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_archetypes_[id].PushBack(i);

        if (view_components.size() == archetype_components.size())
        {
          // Always put exact match first in the list. This can make some operations faster.
          std::swap(view_archetypes_[id].front(), view_archetypes_[id].back());
        }
      }
    }
  }
}

void ViewRelations::AddArchetype(ArchetypeId id)
{
  auto& archetype_components = archetype_components_[id];

  for (ViewId i = 0; i < view_states_.size(); i++)
  {
    if (view_states_[i])
    {
      auto& view_components = view_components_[i];

      if (std::includes(
            archetype_components.begin(), archetype_components.end(), view_components.begin(), view_components.end()))
      {
        view_archetypes_[i].PushBack(id);

        if (view_components.size() == archetype_components.size())
        {
          // Always put exact match first in the list. This can make some operations faster.
          std::swap(view_archetypes_[i].front(), view_archetypes_[i].back());
        }
      }
    }
  }
}
} // namespace genebits::engine
