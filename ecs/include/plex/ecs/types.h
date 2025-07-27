#ifndef PLEX_ECS_TYPES_H
#define PLEX_ECS_TYPES_H

#include <algorithm>
#include <memory>
#include <mutex>

#include "plex/containers/vector.h"
#include "plex/utilities/type_info.h"
#include "plex/utilities/type_sort.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
///
/// Entity identifier type.
///
/// 32 bit should always be sufficient (~4.2 billion entities).
///
using Entity = uint32_t;

using ComponentId = size_t;
using ViewId = size_t;
using ArchetypeId = size_t;

// Tags used to use a different unique id sequence for every class of types.
// This helps keep unique ids nice and compact.

struct ComponentIdTag
{};
struct ArchetypeIdTag
{};
struct ViewIdTag
{};

///
/// Maximum amount of archetypes and views allowed.
///
/// @note Having a cap defined allows us to avoid some branches in certain hot paths. This is purely for optimization
/// purposes and is not a limitation.
///
static constexpr size_t MaxArchetypes = 4096;

///
/// Returns the component id for the component type.
///
/// Ids come from a packed sequence starting at 0.
///
/// @tparam Component Component type to get id for.
///
/// @return Component identifier.
///
template<typename Component>
ComponentId GetComponentId() noexcept
{
  return static_cast<ComponentId>(TypeIndex<Component, ComponentIdTag>());
}

///
/// Returns the archetype id for the component type list.
///
/// Ids come from a packed sequence starting at 0.
///
/// @note The order of the components does not matter.
///
/// @tparam Components Components that compose the archetype.
///
/// @return Archetype identifier.
///
template<typename... Components>
ArchetypeId GetArchetypeId() noexcept
{
  return static_cast<ArchetypeId>(TypeIndex<SortedTypes<Components...>, ArchetypeIdTag>());
}

///
/// Returns the view id for the component type list.
///
/// Ids come from a packed sequence starting at 0.
///
/// @note The order of the components does not matter.
///
/// @tparam Components Components that compose the archetype.
///
/// @return View identifier.
///
template<typename... Components>
ViewId GetViewId() noexcept
{
  return static_cast<ViewId>(TypeIndex<SortedTypes<Components...>, ViewIdTag>());
}

} // namespace plex

#endif
