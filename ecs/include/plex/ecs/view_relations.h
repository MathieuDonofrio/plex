#ifndef PLEX_ECS_VIEW_RELATIONS_H
#define PLEX_ECS_VIEW_RELATIONS_H

#include <algorithm>
#include <memory>
#include <mutex>

#include "plex/containers/vector.h"
#include "plex/ecs/types.h"
#include "plex/utilities/type_info.h"
#include "plex/utilities/type_sort.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
namespace details
{
  ///
  /// Utility for obtaining the runtime component sequence (signature) for list of types.
  ///
  /// @tparam ComponentList Variadic component type list.
  ///
  template<typename ComponentList>
  struct ComponentSequence;

  template<typename... Components, template<typename...> class ComponentList>
  struct ComponentSequence<ComponentList<Components...>>
  {
    ///
    /// Returns the runtime component sequence using the component ids.
    ///
    /// @return Vector of component ids at runtime.
    ///
    static Vector<ComponentId> Value()
    {
      Vector<ComponentId> components;
      components.reserve(sizeof...(Components));

      (components.push_back(GetComponentId<Components>()), ...);

      std::ranges::sort(components);

      return components;
    }
  };

  ///
  /// Returns a sorted vector of all the component ids for the list of component types.
  ///
  /// @tparam Components Component types to get ids for.
  ///
  /// @return Sorted list of component ids.
  ///
  template<typename... Components>
  static const Vector<ComponentId>& GetComponentIds()
  {
    static const auto components = details::ComponentSequence<SortedTypes<Components...>>::Value();

    return components;
  }
} // namespace details

///
/// Keeps track of what archetypes are in every view in an array ready for lookup.
///
class ViewRelations final
{
public:
  ViewRelations()
  {
    archetype_states_.resize(MaxArchetypes);
    view_states_.resize(MaxArchetypes);

    // Assure the empty view. This guarantees that it will be first in the arrays.
    AssureView();
  }

  ///
  /// If the view never existed it will be baked into the flattened graph for quick access.
  ///
  /// @note Thread-safe
  ///
  /// @tparam Components Unordered list of component types.
  ///
  /// @return The view id that was assured.
  ///
  template<typename... Components>
  ViewId AssureView()
  {
    const ViewId id = GetViewId<std::remove_cvref_t<Components>...>();

    if (!view_states_[id]) [[unlikely]]
    {
      InitializeView<Components...>();
    }

    return id;
  }

  ///
  /// If the archetype never existed it will be baked into the flattened graph for quick access.
  ///
  /// @note Thread-safe
  ///
  /// @tparam Components Unordered list of component types.
  ///
  /// @return The archetype id that was assured.
  ///
  template<typename... Components>
  ArchetypeId AssureArchetype()
  {
    const ArchetypeId id = GetArchetypeId<std::remove_cvref_t<Components>...>();

    if (!archetype_states_[id]) [[unlikely]]
    {
      InitializeArchetype<Components...>();
    }

    return id;
  }

  ///
  /// Returns the list of the ids of all archetypes that the view can see.
  ///
  /// Very fast, simply a single lookup.
  ///
  /// @param[in] id View identifier.
  ///
  /// @return List of archetypes for the view.
  ///
  [[nodiscard]] constexpr const Vector<ArchetypeId>& ViewArchetypes(const ViewId id) const noexcept
  {
    ASSERT(view_states_[id], "View not initialized");

    return view_archetypes_[id];
  }

private:
  ///
  /// Initializes an unordered list of components and states for a given id.
  ///
  /// Components and states are seperated for SoA access cache performance benefits.
  ///
  /// @tparam IdType Type of identifier.
  /// @tparam Components List of component types.
  ///
  /// @param[in] components The components list.
  /// @param[in] states The states list.
  /// @param[in] id The id to initialize for.
  ///
  /// @return True if already initialized, false otherwise.
  ///
  template<std::unsigned_integral IdType, typename... Components>
  static bool Initialize(Vector<Vector<ComponentId>>& components, Vector<bool>& states, const IdType id)
  {
    if (id >= components.size())
    {
      components.resize(id + 1);
    }

    components[id] = details::GetComponentIds<std::remove_cvref_t<Components>...>();

    states[id] = true;

    return true;
  }

  ///
  /// Initializes the view for the id and the component types.
  ///
  /// @tparam Components The component types of the view.
  ///
  /// @param[in] id The view id.
  ///
  template<typename... Components>
  COLD_SECTION NO_INLINE void InitializeView()
  {
    const ViewId id = GetViewId<std::remove_cvref_t<Components>...>();

    ASSERT(id < MaxArchetypes, "Too many views.");

    std::lock_guard lg(mutex_);

    if (!view_states_[id])
    {
      Initialize<ViewId, Components...>(view_components_, view_states_, id);
      AddView(id);
    }
  }

  ///
  /// Initializes the archetype for the id and the component types.
  ///
  /// @tparam Components The component types of the archetype.
  ///
  /// @param[in] id The archetype id.
  ///
  template<typename... Components>
  COLD_SECTION NO_INLINE void InitializeArchetype()
  {
    const ArchetypeId id = GetArchetypeId<std::remove_cvref_t<Components>...>();

    ASSERT(id < MaxArchetypes, "Too many archetypes.");

    std::lock_guard lg(mutex_);

    if (!archetype_states_[id])
    {
      Initialize<ArchetypeId, Components...>(archetype_components_, archetype_states_, id);
      AddArchetype(id);
    }
  }

  ///
  /// Adds the view into the graph. To be called once per view during initialization.
  ///
  /// @param[in] id Identifier of the view to add.
  ///
  void AddView(ViewId id);

  ///
  /// Adds the archetype into the graph. To be called once per archetype during initialization.
  ///
  /// @param[in] id Identifier of the archetype to add.
  ///
  void AddArchetype(ArchetypeId id);

private:
  Vector<Vector<ArchetypeId>> view_archetypes_;

  Vector<Vector<ComponentId>> archetype_components_;
  Vector<Vector<ComponentId>> view_components_;

  std::mutex mutex_;

  Vector<bool> archetype_states_;
  Vector<bool> view_states_;
};

} // namespace plex

#endif
