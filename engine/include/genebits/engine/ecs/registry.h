#ifndef GENEBITS_ENGINE_ECS_REGISTRY_H
#define GENEBITS_ENGINE_ECS_REGISTRY_H

#include <algorithm>
#include <concepts>
#include <tuple>
#include <type_traits>

#include "genebits/engine/ecs/archetype.h"
#include "genebits/engine/ecs/entity_manager.h"
#include "genebits/engine/ecs/storage.h"

namespace genebits::engine
{
///
/// Entity identifier type.
///
/// 32 bit should always be sufficient (~4.2 billion entities).
///
using Entity = uint_fast32_t;

template<typename...>
class View;

///
/// Registry is where all entities and their components are stored and managed.
///
/// This registry is an archetype aware ECS container. It organizes entities and components based on their archetype.
/// Archetypes are simply the set of components that an entity has.
///
/// Every archetype has its own storage. Each storage is a SOA, where each component in the archetype has its
/// own densely packed array.
///
/// To access data from the registry, you must create a view of the registry with the desired components.
///
class Registry final
{
public:
  template<typename...>
  friend class View;

  ///
  /// Constructor.
  ///
  Registry()
  {
    storages_.resize(MaxArchetypes);
  }

  ///
  /// Destructor.
  ///
  ~Registry()
  {
    for (auto storage : storages_)
    {
      if (storage) delete storage;
    }
  }

  Registry(const Registry&) = delete;
  Registry(Registry&&) = delete;
  Registry& operator=(const Registry&) = delete;
  Registry& operator=(Registry&&) = delete;

  ///
  /// Creates a new entity and with the given components.
  ///
  /// @note
  ///    The fastest way to create a component is to initialize all its components during creation, this avoids
  ///    archetype swapping.
  ///
  /// @tparam Components List of component types used as initial archetype.
  ///
  /// @param[in] components Component data to create entity with.
  ///
  /// @return Unique identifier of the created entity.
  ///
  template<typename... Components>
  Entity Create(Components&&... components)
  {
    const Entity entity = entity_manager_.Obtain();

    AssureStorage<Components...>().Insert(entity, std::forward<Components>(components)...);

    return entity;
  }

  ///
  /// Destroys the entity and all its attached components.
  ///
  /// @note If some or all the component types of the entity are known during destruction, they can be provided as
  /// template arguments to reduce the overhead of finding the correct storage for the entity. If the exact archetype is
  /// provided, there will be no overhead for finding the storage. In most cases, this is not worth doing, as the
  /// overhead is usually very small.
  ///
  /// @warning
  ///    If the provided templated component types do not belong to the entity, the behaviour of this method
  ///    is undefined.
  ///
  /// @tparam Components Optional partial or complete list of component types of the entity's archetype.
  ///
  /// @param[in] entity Entity to destroy.
  ///
  template<typename... Components>
  void Destroy(const Entity entity)
  {
    ViewFor<Components...>().Destroy(entity);
  }

  ///
  /// Destroys all the entities who's archetype contains all of the provided component types.
  ///
  /// @tparam Components List of component types of entities to destroy.
  ///
  template<typename... Components>
  void DestroyAll()
  {
    ViewFor<Components...>().DestroyAll();
  }

  ///
  /// Returns a reference to the component data of the given component type for given entity.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity)
  {
    return ViewFor<Component>().template Unpack<Component>(entity);
  }

  ///
  /// Returns whether or not the entity has all of the specified components.
  ///
  /// @tparam Components Required component types.
  ///
  /// @param[in] entity Entity to check.
  ///
  /// @return True if the entity has all the components.
  ///
  template<typename... Components>
  bool HasComponents(Entity entity)
  {
    return ViewFor<Components...>().Contains(entity);
  }

  ///
  /// Returns the amount of entities with the specified components.
  ///
  /// @tparam Components Component types to check size for.
  ///
  /// @return Amount of entities with provided component types.
  ///
  template<typename... Components>
  requires(sizeof...(Components) > 0) [
    [nodiscard]] size_t EntityCount() noexcept
  {
    return ViewFor<Components...>().Size();
  }

  ///
  /// Returns the total amount of entities in the registry.
  ///
  /// This is the sum of the sizes of every storage (every archetype) in the registry.
  ///
  /// @return Amount of entities in the registry.
  ///
  [[nodiscard]] size_t EntityCount() const noexcept
  {
    return entity_manager_.CirculatingCount();
  }

  ///
  /// Obtains a view of the registry for the provided component types.
  ///
  /// @tparam Components Component types to include in the view.
  ///
  /// @return Basic view of the registry for the component types.
  ///
  template<typename... Components>
  [[nodiscard]] View<Components...> ViewFor()
  {
    return View<Components...>(*this);
  }

private:
  ///
  /// Returns the storage for the archetype.
  ///
  /// Will properly initialize the storage if it does not exist.
  ///
  /// @tparam Components Component types that compose the archetype.
  ///
  /// @return Reference to assured storage.
  ///
  template<typename... Components>
  Storage<Entity>& AssureStorage()
  {
    const ArchetypeId id = relations_.template AssureArchetype<std::remove_cvref_t<Components>...>();

    auto storage = storages_[id];

    if (storage) [[likely]]
    {
      return *storage;
    }
    else
    {
      return InitializeStorage<std::remove_cvref_t<Components>...>();
    }
  }

  ///
  /// Initializes the storage for the archetype.
  ///
  /// @tparam Components Components that make up the archetype.
  ///
  template<typename... Components>
  COLD_SECTION NO_INLINE Storage<Entity>& InitializeStorage()
  {
    const ArchetypeId archetype = relations_.template AssureArchetype<Components...>();

    storages_[archetype] = new Storage<Entity>(&mappings_);
    storages_[archetype]->template Initialize<Components...>();

    return *storages_[archetype];
  }

private:
  SharedSparseArray<Entity> mappings_;
  EntityManager<Entity> entity_manager_;
  ViewRelations relations_;

  Vector<Storage<Entity>*> storages_;
};

///
/// Tuple for all data pointers of an entity.
///
/// They is one pointer for the entity identifier and one pointer for each component.
///
/// @tparam Components All the component types.
///
template<typename... Components>
using EntityData = std::tuple<Entity*, Components*...>;

///
/// Sub view contains entities and component data of a single archetype.
///
/// Can be thought of as a view over a single storage in the registry.
///
/// @tparam Components Component types of the view.
///
template<typename... Components>
class SubView
{
public:
  class SubViewIterator
  {
  private:
    using Self = SubViewIterator;

  public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using value_type = EntityData<std::remove_cvref_t<Components>...>;

    constexpr SubViewIterator() noexcept = default;

    SubViewIterator(Storage<Entity>* storage, size_t offset) noexcept
      : data_(
        storage->data() + offset, (storage->template Access<std::remove_cvref_t<Components>>().data() + offset)...)
    {}

    SubViewIterator(const SubViewIterator& other) noexcept = default;
    SubViewIterator& operator=(const SubViewIterator&) noexcept = default;

    // clang-format off

    Self& operator+=(difference_type amount) noexcept
    {
      std::get<0>(data_) += amount;
      ((std::get<std::remove_cvref_t<Components>*>(data_) += amount), ...);
      return *this;
    }

    Self& operator-=(difference_type amount) noexcept
    {
      std::get<0>(data_) -= amount;
      ((std::get<std::remove_cvref_t<Components>*>(data_) -= amount), ...);
      return *this;
    }

    Self& operator++() noexcept
    {
      ++std::get<0>(data_);
      (++std::get<std::remove_cvref_t<Components>*>(data_), ...);
      return *this;
    }

    Self& operator--() noexcept
    {
      --std::get<0>(data_);
      (--std::get<std::remove_cvref_t<Components>*>(data_), ...);
      return *this;
    }

    Self operator++(int) noexcept { Self copy(*this); operator++(); return copy; }
    Self operator--(int) noexcept { Self copy(*this); operator--(); return copy; }

    [[nodiscard]] friend Self operator+(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp += amount; return tmp; }
    [[nodiscard]] friend Self operator-(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp -= amount; return tmp; }
    [[nodiscard]] friend Self operator+(difference_type amount, const Self& it) noexcept
    { return it + amount; }

    [[nodiscard]] friend difference_type operator-(const Self& lhs, const Self& rhs) noexcept
    {
      return std::get<0>(lhs.data_) - std::get<0>(rhs.data_);
    }

    const value_type& operator*() const noexcept { return data_; }

    [[nodiscard]] friend bool operator==(const Self& lhs, const Self& rhs) noexcept
    {
      return std::get<0>(lhs.data_) == std::get<0>(rhs.data_);
    }

    [[nodiscard]] friend bool operator!=(const Self& lhs, const Self& rhs) noexcept
    {
      return !(lhs == rhs);
    }

    [[nodiscard]]
    friend std::strong_ordering operator<=>(const Self& lhs, const Self& rhs) noexcept
    {
      return std::get<0>(lhs.data_) <=> std::get<0>(rhs.data_);
    }

    // clang-format on

  private:
    value_type data_;
  };

  using iterator = SubViewIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  // clang-format off

  [[nodiscard]] iterator begin() const noexcept { return {storage_, 0}; }
  [[nodiscard]] iterator end() const noexcept { return { storage_, storage_->Size() }; }

  [[nodiscard]] reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
  [[nodiscard]] reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

  [[nodiscard]] const Entity* ebegin() const noexcept { return storage_->begin(); }
  [[nodiscard]] const Entity* eend() const noexcept { return storage_->end(); }

  // clang-format on

public:
  ///
  /// Checks if the entity is in the view.
  ///
  /// @param[in] entity Entity to find.
  ///
  /// @returns True if the entity exists in the view, false otherwise.
  ///
  [[nodiscard]] bool Contains(const Entity entity) const noexcept
  {
    return storage_->Contains(entity);
  }

  ///
  /// Returns the amount of entities in the view.
  ///
  /// @return Amount of entities in the view.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    return storage_->Size();
  }

  ///
  /// Returns a const reference to the component data for the entity.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return Reference to the unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist in the view");

    return storage_->template Unpack<Component>(entity);
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return Reference to the unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(static_cast<const SubView*>(this)->Unpack<Component>(entity));
  }

private:
  template<typename...>
  friend class View;

  ///
  /// Constructs a view from a storage.
  ///
  /// @param[in] storage Storage to construct view with.
  ///
  constexpr SubView(Storage<Entity>* storage) : storage_(storage) {}

private:
  Storage<Entity>* storage_;
};

///
/// View contains entities of possibly different archetypes.
///
/// Can be thought of as a view over the entire registry for the given components.
///
/// Quite a bit of work being done here. Registry will use the view to do its higher level operations.
///
/// @warning
///     The view only guarantees that it will contain the archetypes that meet its requirements at the time of creation.
///     For example, if a new archetype is created with all the required components after this view was created, it will
///     not be in the view. For this reason, it is good practice to recreate views when needed.
///
/// @tparam Components Required component types for the view.
///
template<typename... Components>
class View
{
public:
  class ViewIterator
  {
  private:
    using Self = ViewIterator;

  public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using value_type = SubView<std::remove_cvref_t<Components>...>;

    constexpr ViewIterator(const ArchetypeId* archetype, Registry& registry) noexcept
      : archetype_(archetype), registry_(registry)
    {}

    ViewIterator(const ViewIterator& other) noexcept = default;
    ViewIterator& operator=(const ViewIterator&) noexcept = default;

    // clang-format off

    constexpr Self& operator+=(difference_type amount) noexcept { archetype_ += amount; return *this; }
    constexpr Self& operator-=(difference_type amount) noexcept { archetype_ -= amount; return *this; }

    constexpr Self& operator++() noexcept { return ++archetype_, *this; }
    constexpr Self& operator--() noexcept { return --archetype_, *this; }

    Self operator++(int) noexcept { Self copy(*this); operator++(); return copy; }
    Self operator--(int) noexcept { Self copy(*this); operator--(); return copy; }

    [[nodiscard]] friend Self operator+(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp += amount; return tmp; }
    [[nodiscard]] friend Self operator-(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp -= amount; return tmp; }
    [[nodiscard]] friend Self operator+(difference_type amount, const Self& it) noexcept
    { return it + amount; }

    [[nodiscard]] friend difference_type operator-(const Self& lhs, const Self& rhs) noexcept
    {
      return lhs.archetype_ - rhs.archetype_;
    }

    [[nodiscard]] constexpr value_type operator*() const noexcept
    {
      return registry_.storages_[*archetype_];
    }

    [[nodiscard]] friend bool operator==(const Self& lhs, const Self& rhs) noexcept
    {
      return lhs.archetype_ == rhs.archetype_;
    }

    [[nodiscard]] friend bool operator!=(const Self& lhs, const Self& rhs) noexcept
    {
      return !(lhs == rhs);
    }

    [[nodiscard]]
    friend std::strong_ordering operator<=>(const Self& lhs, const Self& rhs) noexcept
    {
      return lhs.archetype_ <=> rhs.archetype_;
    }

    // clang-format on

  private:
    const ArchetypeId* archetype_;
    Registry& registry_;
  };

  using iterator = ViewIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  // clang-format off

  [[nodiscard]] iterator begin() const noexcept { return { archetypes_.begin(), registry_ }; }
  [[nodiscard]] iterator end() const noexcept { return { archetypes_.end(), registry_ }; }

  [[nodiscard]] reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
  [[nodiscard]] reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

  // clang-format on

public:
  static constexpr bool cNoComponents = sizeof...(Components) == 0;

  ///
  /// Constructor.
  ///
  /// @param[in] registry Registry to construct view for.
  ///
  constexpr explicit View(Registry& registry)
    : registry_(registry),
      archetypes_(registry.relations_.ViewArchetypes(registry.relations_.template AssureView<Components...>()))
  {}

  ///
  /// Destroys the entity and all its attached components.
  ///
  /// @warning
  ///    If the view does not contain the entity, the behaviour of this method is undefined.
  ///
  /// @param[in] entity Entity to destroy.
  ///
  void Destroy(const Entity entity)
  {
    ASSERT(Contains(entity), "Entity does not exist in view");

    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity))
      {
        storage->Erase(entity);

        registry_.entity_manager_.Release(entity);

        return;
      }
    }
  }

  ///
  /// Destroys all entities in the view.
  ///
  void DestroyAll()
  {
    for (const auto archetype : archetypes_)
    {
      auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if constexpr (!cNoComponents) // Release all later
      {
        for (auto entity : *storage)
        {
          registry_.entity_manager_.Release(entity);
        }
      }

      storage->Clear();
    }

    if constexpr (cNoComponents)
    {
      // This releases everything at once very cheaply.
      registry_.entity_manager_.ReleaseAll();
    }
  }

  ///
  /// Checks if the entity is in the view.
  ///
  /// @param[in] entity Entity to find.
  ///
  /// @returns True if the entity exists in the view, false otherwise.
  ///
  [[nodiscard]] bool Contains(const Entity entity) const noexcept
  {
    for (const auto archetype : archetypes_)
    {
      const auto storage = registry_.storages_[archetype];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity)) return true;
    }

    return false;
  }

  ///
  /// Returns the amount of entities in the view.
  ///
  /// @return Amount of entities in the view.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    if constexpr (!cNoComponents)
    {
      size_t sum = 0;

      for (const auto archetype : archetypes_)
      {
        const auto storage = registry_.storages_[archetype];

        ASSERT(storage, "Storage not initialized");

        sum += storage->Size();
      }

      return sum;
    }
    else
    {
      return registry_.EntityCount();
    }
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist in the view");

    const auto end = archetypes_.end() - 1;

    for (auto it = archetypes_.begin(); it != end; ++it)
    {
      const auto storage = registry_.storages_[*it];

      ASSERT(storage, "Storage not initialized");

      if (storage->Contains(entity)) return storage->template Unpack<Component>(entity);
    }

    ASSERT(registry_.storages_[*end], "Storage not initialized");

    return registry_.storages_[*end]->template Unpack<Component>(entity);
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// This operation is O(n) where n is the amount of archetypes in the view.
  ///
  /// @note
  ///    Prefer obtaining unpacked components directly from iterating when possible.
  ///
  /// @tparam Component The component to obtain reference of.
  ///
  /// @param[in] entity Entity to unpack data for.
  ///
  /// @return The unpacked component data.
  ///
  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(static_cast<const View*>(this)->Unpack<Component>(entity));
  }

private:
  Registry& registry_;
  const Vector<ArchetypeId>& archetypes_;
};

namespace details
{
  template<typename Type>
  struct IsInstanceOfEntityData : std::false_type
  {};

  template<typename... Components>
  struct IsInstanceOfEntityData<EntityData<Components...>> : std::true_type
  {};

  template<typename Type>
  concept InstanceOfEntityData = IsInstanceOfEntityData<std::remove_cvref_t<Type>>::value;

  template<typename Type>
  struct IsInstanceOfSubView : std::false_type
  {};

  template<typename... Components>
  struct IsInstanceOfSubView<SubView<Components...>> : std::true_type
  {};

  template<typename Type>
  concept InstanceOfSubView = IsInstanceOfSubView<std::remove_cvref_t<Type>>::value;
} // namespace details

// clang-format off

///
/// Concept used to determine whether a type is a SubView iterator.
///
/// @tparam Type Type to check
///
template<typename Type>
concept SubViewIterator = requires(Type value)
{
  { value.operator*() } -> details::InstanceOfEntityData;
};

///
/// Concept used to determine whether a type is a View iterator.
///
/// @tparam Type Type to check
///
template<typename Type>
concept ViewIterator = requires(Type value)
{
  { value.operator*() } -> details::InstanceOfSubView;
};

///
/// Concept used to determine whether a type is a SubView iterator.
///
/// @tparam Type Type to check
///
template<typename Type>
concept SubViewRange = requires(Type value)
{
  { value.begin() } -> SubViewIterator;
  { value.end() } -> SubViewIterator;
};

///
/// Concept used to determine whether a type is a View iterator.
///
/// @tparam Type Type to check
///
template<typename Type>
concept ViewRange = requires(Type value)
{
  { value.begin() } -> ViewIterator;
  { value.end() } -> ViewIterator;
};

///
/// Concept used to determine if a function can be used as a reduced entity invocable function.
///
/// To meet the requirements, the function operator must contain only components as its arguments.
///
/// @example
/// @code
/// [](Position& position, Velocity& velocity) {...}
/// @endcode
///
/// @tparam Function Function type.
/// @tparam Components Component types.
///
template<typename Function, typename... Components>
concept EntityReducedInvocable = requires(Function function, Components... components)
{
  function(components...);
};

///
/// Concept used to determine if a function can be used as a extended entity invocable function.
///
/// To meet the requirements, the function operator must contain the entity and components as its arguments.
///
/// @example
/// @code
/// [](Entity entity, Position& position, Velocity& velocity) {...}
/// @endcode
///
/// @tparam Function Function type.
/// @tparam Components Component types.
///
template<typename Function, typename... Components>
concept EntityExtendedInvocable = requires(Function function, Entity entity, Components... components)
{
  function(entity, components...);
};

// clang-format on

///
/// Unpacks the entity identifier and entity data, then applies the function.
///
/// @tparam Function The function object to apply.
/// @tparam Components Component types of the entity data.
///
/// @param[in] invocable Invocable to apply.
/// @param[in] data Entity data.
///
template<typename Function, typename... Components>
requires EntityExtendedInvocable<Function, Components...>
ALWAYS_INLINE constexpr void EntityApply(const Function& function, const EntityData<Components...>& data)
{
  function(*std::get<0>(data), *std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Unpacks the entity data, then applies the function.
///
/// @tparam Function The function object to apply.
/// @tparam Components Component types of the entity data.
///
/// @param[in] invocable Invocable to apply.
/// @param[in] data Entity data.
///
template<typename Function, typename... Components>
requires EntityReducedInvocable<Function, Components...>
ALWAYS_INLINE constexpr void EntityApply(const Function& function, const EntityData<Components...>& data)
{
  function(*std::get<std::remove_cvref_t<Components>*>(data)...);
}

///
/// Iterates over every entity within the range. For each entity, its components will be unpacked and the given
/// function will be invoked.
///
/// It is recommended to always use this method for iterating over entities instead manually iterating.
///
/// @tparam Iterator SubView iterator type.
/// @tparam Function Function to apply at each iteration.
///
/// @param[in] first, last The range to apply the function to
/// @param[in] function The function object to apply at every iteration.
///
template<SubViewIterator Iterator, typename Function>
FLATTEN ALWAYS_INLINE constexpr void EntityForEach(Iterator first, Iterator last, const Function& function)
{
  // Iteration is unrolled to reduce branching and improve vectorization.

  auto trip_count = (last - first) >> 2;

  // clang-format off

  for (; trip_count > 0; --trip_count)
  {
    EntityApply(function, *first); ++first;
    EntityApply(function, *first); ++first;
    EntityApply(function, *first); ++first;
    EntityApply(function, *first); ++first;
  }

  switch (last - first)
  {
  case 3: EntityApply(function, *first); ++first;
  case 2: EntityApply(function, *first); ++first;
  case 1: EntityApply(function, *first);
  case 0: ;
  }

  // clang-format on
}

///
/// Iterates over every entity within the range. For each entity, its components will be unpacked and the given
/// function will be invoked.
///
/// It is recommended to always use this method for iterating over entities instead manually iterating.
///
/// @tparam Range Range type
/// @tparam Function Function to apply at each iteration.
///
/// @param[in] range The range to apply the function to
/// @param[in] function The function object to apply at every iteration.
///
template<SubViewRange Range, typename Function>
ALWAYS_INLINE constexpr void EntityForEach(Range&& range, const Function& function)
{
  EntityForEach(range.begin(), range.end(), function);
}

///
/// Iterates over every entity within the range. For each entity, its components will be unpacked and the given
/// function will be invoked.
///
/// It is recommended to always use this method for iterating over entities instead manually iterating.
///
/// @tparam Iterator SubView iterator type.
/// @tparam Function Function to apply at each iteration.
///
/// @param[in] first, last The range to apply the function to
/// @param[in] function The function object to apply at every iteration.
///
template<ViewIterator Iterator, typename Function>
void EntityForEach(Iterator first, Iterator last, Function function)
{
  for (; first != last; ++first)
  {
    EntityForEach(*first, function);
  }
}

///
/// Iterates over every entity within the range. For each entity, its components will be unpacked and the given
/// function will be invoked.
///
/// It is recommended to always use this method for iterating over entities instead manually iterating.
///
/// @tparam Range Range type.
/// @tparam Function Function to apply at each iteration.
///
/// @param[in] range The range to apply the function to
/// @param[in] function The function object to apply at every iteration.
///
template<ViewRange Range, typename Function>
void EntityForEach(Range&& range, Function function)
{
  for (auto sub_view : std::forward<Range>(range))
  {
    EntityForEach(sub_view, function);
  }
}

} // namespace genebits::engine

#endif
