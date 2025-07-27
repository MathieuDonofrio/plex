#ifndef PLEX_ECS_STORAGE_H
#define PLEX_ECS_STORAGE_H

#include "plex/containers/type_map.h"
#include "plex/containers/vector.h"
#include "plex/utilities/erased_ptr.h"
#include "plex/utilities/memory.h"
#include "plex/utilities/type_info.h"

namespace plex
{
///
/// Sparse array used to lookup entity indexes.
///
/// Very quick entity to index mappings used for the storage to create a sparse set.
///
/// The sparse array can be shared between storages that use the same entity manager. Sharing the sparse array can
/// reduce memory usage. For example, if there are 10 archetypes, sharing the sparse array could save up to 9 times the
/// lookup table memory, making it more cache friendly.
///
/// @tparam Entity The type of entity to use.
/// @tparam AllocatorImpl Allocator to allocate memory with.
///
template<std::unsigned_integral Entity>
class ArchetypeStorageSparseArray
{
public:
  ///
  /// Constructor.
  ///
  constexpr ArchetypeStorageSparseArray() noexcept : capacity_(32)
  {
    array_ = static_cast<Entity*>(std::malloc(sizeof(Entity) * capacity_));
  }

  ///
  /// Destructor.
  ///
  ~ArchetypeStorageSparseArray()
  {
    std::free(array_);
  }

  ArchetypeStorageSparseArray(const ArchetypeStorageSparseArray&) = delete;
  ArchetypeStorageSparseArray(ArchetypeStorageSparseArray&&) = delete;
  ArchetypeStorageSparseArray& operator=(const ArchetypeStorageSparseArray&) = delete;
  ArchetypeStorageSparseArray& operator=(ArchetypeStorageSparseArray&&) = delete;

  ///
  /// Assures that the entity can be mapped in the sparse array.
  ///
  /// @param[in] entity Entity to verify.
  ///
  void Assure(const Entity entity) noexcept
  {
    if (entity >= capacity_) [[unlikely]]
    {
      capacity_ += static_cast<uint32_t>(entity); // Growth < factor 2
      array_ = static_cast<Entity*>(std::realloc(array_, capacity_ * sizeof(Entity)));
    }
  }

  ///
  /// Const array access operator.
  ///
  /// @param[in] entity Entity to access the index for.
  ///
  /// @return Entity index.
  ///
  [[nodiscard]] constexpr Entity operator[](const Entity entity) const noexcept
  {
    return array_[entity];
  }

  ///
  /// Array access operator.
  ///
  /// @param[in] entity Entity to access the index for.
  ///
  /// @return Entity index.
  ///
  [[nodiscard]] constexpr Entity& operator[](const Entity entity) noexcept
  {
    return array_[entity];
  }

  ///
  /// Returns the capacity of the sparse array.
  ///
  /// @return Sparse array capacity.
  ///
  [[nodiscard]] constexpr size_t Capacity() const noexcept
  {
    return capacity_;
  }

private:
  Entity* array_;
  size_t capacity_;
};

///
/// Storage container for a single archetype.
///
/// Basically a sparse set, but optimized for storing extra type erased data, in this case component data.
///
/// Components are stores as SOA, and every component array is dense.
///
/// Insertion and erasing is constant time.
///
/// @warning There is no pointer stability, never store a pointer to a component.
///
/// @warning Never assume any kind of order. Storage reserves the right to reorder entities and components.
///
template<std::unsigned_integral Entity>
class ArchetypeStorage
{
public:
  // Style Exception: STL
  // clang-format off

  using size_type = size_t;

  using iterator = Entity*;
  using const_iterator = const Entity*;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  using reference = Entity&;
  using const_reference = const Entity&;
  using pointer = Entity*;
  using const_pointer = const Entity*;

  // Forward iterator creation methods.
  [[nodiscard]] iterator begin() { return dense_.begin(); }
  [[nodiscard]] const_iterator begin() const { return dense_.begin(); }
  [[nodiscard]] iterator end() { return dense_.end(); }
  [[nodiscard]] const_iterator end() const { return dense_.end(); }

  // Explicit const forward iterator creation methods.
  [[nodiscard]] const_iterator cbegin() const { return dense_.cbegin(); }
  [[nodiscard]] const_iterator cend() const { return dense_.cend(); }

  // Reverse iterator creation methods.
  [[nodiscard]] reverse_iterator rbegin() { return reverse_iterator(end()); }
  [[nodiscard]] const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  [[nodiscard]] reverse_iterator rend() { return reverse_iterator(begin()); }
  [[nodiscard]] const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Internal array accessors
  [[nodiscard]] pointer data() { return pointer(begin()); }
  [[nodiscard]] const_pointer data() const { return const_pointer(begin()); }
  [[nodiscard]] reference front() { return begin()[0]; }
  [[nodiscard]] const_reference front() const { return begin()[0]; }
  [[nodiscard]] reference back() { return end()[-1]; }
  [[nodiscard]] const_reference back() const { return end()[-1]; }

  // clang-format on

  ///
  /// Const array access operator.
  ///
  /// @param[in] index Index to access.
  ///
  /// @return Entity at index.
  ///
  [[nodiscard]] constexpr const_reference operator[](const size_type index) const noexcept
  {
    return dense_[index];
  }

  ///
  /// Array access operator.
  ///
  /// @param[in] index Index to access.
  ///
  /// @return Entity at index.
  ///
  [[nodiscard]] constexpr reference operator[](const size_type index) noexcept
  {
    return dense_[index];
  }

public:
  ///
  /// Constructor.
  ///
  /// @param[in] sparse Shared sparse array.
  ///
  explicit ArchetypeStorage(ArchetypeStorageSparseArray<Entity>* sparse) noexcept
  {
    ASSERT(sparse != nullptr, "Sparse array cannot be nullptr");

    sparse_ = sparse;
  }

  ///
  /// Destructor.
  ///
  ~ArchetypeStorage() = default;

  ArchetypeStorage(const ArchetypeStorage&) = delete;
  ArchetypeStorage(ArchetypeStorage&&) = delete;
  ArchetypeStorage& operator=(const ArchetypeStorage&) = delete;
  ArchetypeStorage& operator=(ArchetypeStorage&&) = delete;

  ///
  /// Initializes the storage for the component types.
  ///
  /// The components must correspond to the archetype being stored.
  ///
  /// @warning
  ///    Must be correctly called before doing anything with the registry, or else behaviour
  ///    of the storage is undefined.
  ///
  /// @tparam Components List of component types.
  ///
  template<typename... Components>
  requires UniqueTypes<std::remove_cvref_t<Components>...>
  COLD_SECTION NO_INLINE void Initialize() noexcept
  {
    ASSERT(!initialized_, "Already initialized");

    // Set up all the component arrays with type erased vectors.
    ((component_arrays_.template Assure<std::remove_cvref_t<Components>>() =
         std::move(MakeErased<Vector<std::remove_cvref_t<Components>>>())),
      ...);

    // Store functors for the operations that need type information and don't have it.
    erase_function_ = []([[maybe_unused]] auto* storage, [[maybe_unused]] const size_t index)
    { (AccessAndEraseAt<std::remove_cvref_t<Components>>(storage, index), ...); };
    clear_function_ = []([[maybe_unused]] auto storage)
    { ((storage->template Access<std::remove_cvref_t<Components>>().clear()), ...); };

#ifndef NDEBUG
    // When debugging it is useful to have the list of components used at initialization.
    ((components_.push_back(TypeName<Components>())), ...);
    initialized_ = true;
#endif
  }

  ///
  /// Inserts the entity with its component data into the storage.
  ///
  /// @note
  ///    Order of the components does not matter.
  ///
  /// @tparam Components List of component types to add.
  ///
  /// @param[in] entity Entity to insert into the storage.
  /// @param[in] components Component data to move into the storage.
  ///
  template<typename... Components>
  requires UniqueTypes<std::remove_cvref_t<Components>...>
  void Insert(const Entity entity, Components&&... components) noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(!Contains(entity), "Entity already exists");
    ASSERT(sizeof...(Components) == components_.size(), "Invalid amount of components");

    // Runtime check that the components are the same as the ones used to initialize the storage
#ifndef NDEBUG // Because of parameter pack
    ((ASSERT(HasComponent<std::remove_cvref_t<Components>>(), "Component type not valid")), ...);
#endif

    sparse_->Assure(entity);
    (*sparse_)[entity] = static_cast<Entity>(dense_.size());

    dense_.push_back(entity);
    ((Access<std::remove_cvref_t<Components>>().emplace_back(std::forward<Components>(components))), ...);
  }

  ///
  /// Erases the entity from the storage.
  ///
  /// @param[in] entity Entity to erase.
  ///
  void Erase(const Entity entity) noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(Contains(entity), "Entity does not exist");

    const auto index = (*sparse_)[entity];
    const auto back_entity = dense_.back();

    (*sparse_)[back_entity] = index;
    dense_[index] = back_entity;

    dense_.pop_back();

    erase_function_(this, index);
  }

  ///
  /// Clears the entire storage.
  ///
  void Clear()
  {
    ASSERT(initialized_, "Not initialized");

    dense_.clear();

    clear_function_(this);
  }

  ///
  /// Checks if the entity is in the storage.
  ///
  /// @param[in] entity The entity to check.
  ///
  /// @return True if the entity is in the storage, false otherwise.
  ///
  [[nodiscard]] constexpr bool Contains(const Entity entity) const noexcept
  {
    ASSERT(initialized_, "Not initialized");

    size_t index;

    return entity < sparse_->Capacity() && (index = (*sparse_)[entity]) < dense_.size() && dense_[index] == entity;
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// @note
  ///    This method of unpacking is slightly slower than the unpacking during iteration.
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
    ASSERT(Contains(entity), "Entity does not exist");

    return Access<Component>()[sparse_->operator[](entity)];
  }

  ///
  /// Returns a reference to the component data for the entity.
  ///
  /// @note
  ///    This method of unpacking is slightly slower than the unpacking during iteration.
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
    return const_cast<Component&>(static_cast<const ArchetypeStorage*>(this)->Unpack<Component>(entity));
  }

  ///
  /// Directly accesses the internal array of the dense storage of the component.
  ///
  /// @tparam Component The component type to access array for.
  ///
  /// @return Reference to dense array for the component type.
  ///
  template<typename Component>
  [[nodiscard]] const Vector<Component>& Access() const noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(HasComponent<Component>(), "Component type not valid");

    return *static_cast<Vector<Component>*>(component_arrays_.template Get<Component>().get());
  }

  ///
  /// Directly accesses the internal array of the dense storage of the component.
  ///
  /// @tparam Component The component type to access array for.
  ///
  /// @return Reference to dense array for the component type.
  ///
  template<typename Component>
  [[nodiscard]] Vector<Component>& Access() noexcept
  {
    return const_cast<Vector<Component>&>(static_cast<const ArchetypeStorage*>(this)->Access<Component>());
  }

  ///
  /// Returns whether or not the storage contains any entities.
  ///
  /// @return True if the storage is empty, false otherwise.
  ///
  [[nodiscard]] bool Empty() const noexcept
  {
    return dense_.empty();
  }

  ///
  /// Returns the amount of entities in the storage.
  ///
  /// @return Size of storage.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    return dense_.size();
  }

private:
#ifndef NDEBUG
  ///
  /// Debug utility for checking if the storage was initialized with a component type.
  ///
  /// @tparam Component Component type to check.
  ///
  /// @return True if the storage was initialized with component, false otherwise.
  ///
  template<typename Component>
  [[nodiscard]] bool HasComponent() const noexcept
  {
    return std::ranges::find(components_, TypeName<Component>()) != components_.end();
  }
#endif

  ///
  /// Static utility function for accessing and erasing at an index.
  ///
  /// @tparam Component Component type to access.
  ///
  /// @param[in] storage This storage.
  /// @param[in] index Index to erase at.
  ///
  template<typename Component>
  static void AccessAndEraseAt(ArchetypeStorage<Entity>* storage, size_t index)
  {
    auto& component_array = storage->template Access<Component>();
    component_array.SwapAndPop(component_array.begin() + index);
  }

private:
  using EraseFunction = void (*)(ArchetypeStorage<Entity>* storage, const size_t);
  using ClearFunction = void (*)(ArchetypeStorage<Entity>* storage);

  ArchetypeStorageSparseArray<Entity>* sparse_;
  Vector<Entity> dense_;

  TypeMap<ErasedPtr<void>> component_arrays_;

  // Indirect functions
  EraseFunction erase_function_;
  ClearFunction clear_function_;

  // Used for debugging purposes
#ifndef NDEBUG
  bool initialized_ = false;
  Vector<std::string_view> components_;
#endif
};

} // namespace plex

#endif
