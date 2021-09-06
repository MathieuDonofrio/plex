#ifndef GENEBITS_ENGINE_ECS_ARCHETYPE_GRAPH_H
#define GENEBITS_ENGINE_ECS_ARCHETYPE_GRAPH_H

#include "genebits/engine/ecs/archetype.h"

namespace genebits::engine
{
///
/// Archetype graph for keeping track of what archetypes are associated with what view.
///
/// The graph is flattened for performance.
///
class ArchetypeGraph final
{
public:
  ArchetypeGraph()
  {
    // Assure the empty view. This guarantees that it will be first in the arrays.
    AssureView();
  }

  ///
  /// If the view never existed it will be baked into the flattened graph for quick access.
  ///
  /// @tparam Components Unordered list of component types.
  ///
  /// @return The view id that was assured.
  ///
  template<typename... Components>
  ViewId AssureView()
  {
    const ViewId id = GetViewId<std::remove_cvref_t<Components>...>();

    if (id >= view_states_.Size() || !view_states_[id])
    {
      mutex_.lock();

      Initialize<ViewId, Components...>(view_components_, view_states_, id);
      AddView(id);

      mutex_.unlock();
    }

    return id;
  }

  ///
  /// If the archetype never existed it will be baked into the flattened graph for quick access.
  ///
  /// @tparam Components Unordered list of component types.
  ///
  /// @return The archetype id that was assured.
  ///
  template<typename... Components>
  ArchetypeId AssureArchetype()
  {
    const ArchetypeId id = GetArchetypeId<std::remove_cvref_t<Components>...>();

    if (id >= archetype_states_.Size() || !archetype_states_[id])
    {
      mutex_.lock();

      Initialize<ArchetypeId, Components...>(archetype_components_, archetype_states_, id);
      AddArchetype(id);

      mutex_.unlock();
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
  [[nodiscard]] constexpr const FastVector<ArchetypeId>& ViewArchetypes(const ViewId id) const noexcept
  {
    ASSERT(id < view_states_.Size() && view_states_[id], "View not initialized");

    return view_archetypes_[id];
  }

private:
  ///
  /// Initializes an unordered list of components and states for a given id.
  ///
  /// Components and states are seperated for SoA access performance.
  ///
  /// @tparam IdType Type of identifier.
  /// @tparam Components List of component types.
  ///
  /// @param[in] components The components list.
  /// @param[in] states The states list.
  /// @param[in] id The id to initialize for.
  ///
  template<std::unsigned_integral IdType, typename... Components>
  static void Initialize(FastVector<FastVector<ComponentId>>& components, FastVector<bool>& states, const IdType id)
  {
    if (id >= states.Size())
    {
      components.Resize(id + 1);
      states.Resize(id + 1);
    }

    components[id] = GetComponentIds<std::remove_cvref_t<Components>...>();
    states[id] = true;
  }

  ///
  /// Bakes the view into the graph.
  ///
  /// @warning
  ///    Only call once after initialization.
  ///
  /// @param[in] id Identifier of the view to add.
  ///
  void AddView(ViewId id);

  ///
  /// Bakes the archetype into the graph.
  ///
  /// @warning
  ///    Only call once after initialization.
  ///
  /// @param[in] id Identifier of the archetype to add.
  ///
  void AddArchetype(ArchetypeId id);

private:
  FastVector<FastVector<ComponentId>> archetype_components_;
  FastVector<bool> archetype_states_;

  FastVector<FastVector<ComponentId>> view_components_;
  FastVector<FastVector<ArchetypeId>> view_archetypes_;
  FastVector<bool> view_states_;

  std::mutex mutex_;
};
} // namespace genebits::engine

#endif
