#ifndef GENEBITS_ENGINE_ECS_ARCHETYPE_H
#define GENEBITS_ENGINE_ECS_ARCHETYPE_H

#include <algorithm>
#include <memory>
#include <mutex>

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/type_info.h"
#include "genebits/engine/utilities/type_traits.h"

namespace genebits::engine
{
namespace details
{
  // Type meta programing for sorting variadic templates.

  ///
  /// Lexically compares the two types.
  ///
  /// @tparam T1 First type.
  /// @tparam T2 Second type.
  ///
  template<typename T1, typename T2>
  struct Compare
  {
    static constexpr bool value = TypeName<T1>().compare(TypeName<T2>()) < 0;

    using type = std::conditional_t<value, T1, T2>;
  };

  ///
  /// Returns the smallest type.
  ///
  /// @tparam T1 First type.
  /// @tparam Types Other types.
  ///
  template<typename T1, typename... Types>
  struct TypeMin
  {
    using type = T1;
  };

  ///
  /// Returns the smallest type.
  ///
  /// @tparam T1 First type.
  /// @tparam T2 Second type.
  /// @tparam Types Other types.
  ///
  template<typename T1, typename T2, typename... Types>
  struct TypeMin<T1, T2, Types...>
  {
    using type = typename std::
      conditional_t<Compare<T2, T1>::value, typename TypeMin<T2, Types...>::type, typename TypeMin<T1, Types...>::type>;
  };

  ///
  /// Removes a type from a variadic list of types.
  ///
  /// @tparam Type Type to remove.
  /// @tparam List Variadic list of types.
  ///
  template<typename Type, typename List>
  struct Remove;

  template<typename Type, typename... Types, template<typename...> class List>
  struct Remove<Type, List<Types...>>
  {
    using type = List<>;
  };

  template<typename T1, typename... Types, template<typename...> class List>
  struct Remove<T1, List<T1, Types...>>
  {
    using type = List<Types...>;
  };

  template<typename Type, typename T1, typename... Types, template<typename...> class List>
  struct Remove<Type, List<T1, Types...>>
  {
    using type = typename Remove<Type, List<Types..., T1>>::type;
  };

  ///
  /// Sorts an unsorted variadic type list into another variadic container.
  ///
  /// @tparam Sorted Variadic container to sort into.
  /// @tparam Unsorted Variadic container to sort.
  ///
  template<typename Sorted, typename Unsorted>
  struct SortInto;

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<Types...>>
  {
    using type = Sorted<>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, Types...>>
  {
    using type = Sorted<T1>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename T2,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, T2, Types...>>
  {
    using type =
      std::conditional_t<Compare<T2, T1>::value, Sorted<SortedTypes..., T2, T1>, Sorted<SortedTypes..., T1, T2>>;
  };

  template<typename... SortedTypes,
    template<typename...>
    class Sorted,
    typename T1,
    typename T2,
    typename T3,
    typename... Types,
    template<typename...>
    class Unsorted>
  struct SortInto<Sorted<SortedTypes...>, Unsorted<T1, T2, T3, Types...>>
  {
  private:
    using NextSmallest = typename TypeMin<T1, T2, T3, Types...>::type;
    using NextSorted = Sorted<SortedTypes..., NextSmallest>;
    using NextUnsorted = typename Remove<NextSmallest, Unsorted<T1, T2, T3, Types...>>::type;

  public:
    using type = typename SortInto<NextSorted, NextUnsorted>::type;
  };

  ///
  /// Container for storing sorted types.
  ///
  /// @tparam Types The types to combine.
  ///
  template<typename... Types>
  struct Combined
  {
    using Sorted = typename SortInto<Combined<>, Combined<Types...>>::type;
  };
} // namespace details

///
/// Alias that sorts a component type list into a single order agnostic type.
///
template<typename... Components>
using ComponentList = typename details::Combined<Components...>::Sorted;

using ComponentId = uint_fast32_t;
using ViewId = uint_fast32_t;
using ArchetypeId = uint_fast32_t;

///
/// Maximum amount of archetypes allowed.
///
/// @note Having a cap defined allows us to avoid some branches in certain hot paths. This is purely for optimization
/// purposes and is not a limitation.
///
static constexpr size_t MaxArchetypes = 4096;

///
/// Maximum amount of views allowed.
///
/// @note Having a cap defined allows us to avoid some branches in certain hot paths. This is purely for optimization
/// purposes and is not a limitation.
///
static constexpr size_t MaxViews = 4096;

///
/// Tag used for the unique id sequence of components.
///
struct ComponentIdTag
{};

///
/// Tag used for the unique id sequence of archetypes.
///
struct ArchetypeIdTag
{};

///
/// Tag used for the unique id sequence of views.
///
struct ViewIdTag
{};

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
  return static_cast<ArchetypeId>(TypeIndex<ComponentList<Components...>, ArchetypeIdTag>());
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
  return static_cast<ViewId>(TypeIndex<ComponentList<Components...>, ViewIdTag>());
}

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
      components.Reserve(sizeof...(Components));

      (components.PushBack(GetComponentId<Components>()), ...);

      std::ranges::sort(components);

      return components;
    }
  };
} // namespace details

///
/// Returns a sorted vector of all the component ids for the list of component types.
///
/// @tparam Components Component types to get ids for.
///
/// @return Sorted list of component ids.
///
template<typename... Components>
const Vector<ComponentId>& GetComponentIds()
{
  static const auto components = details::ComponentSequence<ComponentList<Components...>>::Value();

  return components;
}

///
/// Keeps track of what archetypes are in every view in an array ready for lookup.
///
class ViewRelations final
{
public:
  ViewRelations()
  {
    archetype_states_.Resize(MaxArchetypes);
    view_states_.Resize(MaxViews);

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
      components.Resize(id + 1);
    }

    components[id] = GetComponentIds<std::remove_cvref_t<Components>...>();

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

    ASSERT(id < MaxViews, "Too many views.");

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

} // namespace genebits::engine

#endif
