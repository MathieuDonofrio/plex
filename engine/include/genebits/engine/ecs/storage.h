#ifndef GENEBITS_ENGINE_STORAGE_H
#define GENEBITS_ENGINE_STORAGE_H

#include "genebits/engine/containers/type_map.h"
#include "genebits/engine/containers/vector.h"
#include "genebits/engine/os/memory.h"
#include "genebits/engine/utilities/concepts.h"
#include "genebits/engine/utilities/erased_ptr.h"
#include "genebits/engine/utilities/type_info.h"

namespace genebits::engine
{
///
/// Sparse array used to lookup entity indexes.
///
/// Very quick entity to index mappings used for the storage to create a sparse set.
///
/// The sparse array can be shared between storages that use the same entity generator sequence as long as
/// no entity can be in more than one storage. Since there is one storage per archetype this is guaranteed.
/// Sharing the sparse array reduces overall memory use by a substantial amount. For example, if there are
/// 10 archetypes, sharing the sparse array could save up to 9 times the memory of one sparse array.
///
/// @warning Uses a lot of memory.
///
/// @tparam Entity The type of entity to use.
/// @tparam AllocatorImpl Allocator to allocate memory with.
///
template<std::unsigned_integral Entity>
class SharedSparseArray
{
public:
  ///
  /// Constructor.
  ///
  constexpr SharedSparseArray() noexcept : capacity_(32)
  {
    array_ = static_cast<Entity*>(std::malloc(sizeof(Entity) * capacity_));
  }

  ///
  /// Destructor.
  ///
  ~SharedSparseArray()
  {
    std::free(array_);
  }

  SharedSparseArray(const SharedSparseArray&) = delete;
  SharedSparseArray(SharedSparseArray&&) = delete;
  SharedSparseArray& operator=(const SharedSparseArray&) = delete;
  SharedSparseArray& operator=(SharedSparseArray&&) = delete;

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
/// Component data is contiguously stored in memory and can achieve near vector iteration speeds.
/// Insertion and erasing is constant time.
///
/// @warning
///  Never assume any order. Storage reserves freedom of moving entities around at any time.
///
template<std::unsigned_integral Entity>
class Storage
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
  [[nodiscard]] constexpr const Entity& operator[](const size_type index) const noexcept
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
  [[nodiscard]] constexpr Entity& operator[](const size_type index) noexcept
  {
    return dense_[index];
  }

public:
  ///
  /// Constructor.
  ///
  /// @param[in] sparse Shared sparse array.
  ///
  explicit Storage(SharedSparseArray<Entity>* sparse) noexcept
  {
    ASSERT(sparse != nullptr, "Sparse array cannot be nullptr");

    sparse_ = sparse;
  }

  ///
  /// Destructor.
  ///
  ~Storage() = default;

  Storage(const Storage&) = delete;
  Storage(Storage&&) = delete;
  Storage& operator=(const Storage&) = delete;
  Storage& operator=(Storage&&) = delete;

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
  requires UniqueTypes<Components...>
  void Initialize() noexcept
  {
    ASSERT(!initialized_, "Already initialized");

    // Set up all the component arrays with type erased vectors.
    ((component_arrays_.template Assure<Components>() = std::move(MakeErased<Vector<Components>>())), ...);

    // Store functors for the operations that need type information and don't have it.
    erase_function_ = []([[maybe_unused]] auto* storage, [[maybe_unused]] const size_t index)
    { (AccessAndEraseAt<Components>(storage, index), ...); };
    clear_function_ = []([[maybe_unused]] auto storage) { ((storage->template Access<Components>().Clear()), ...); };

#ifndef NDEBUG
    // When debugging it is useful to have the list of components used at initialization.
    ((components_.PushBack(TypeInfo<Components>::HashCode())), ...);
    initialized_ = true;
#endif
  }

  ///
  /// Inserts the entity with its component data into the storage.
  ///
  /// This operation O(1) and is very fast.
  ///
  /// @note
  ///    Order of the components does not matter.
  ///
  /// @warning
  ///    The component types must be exactly the same as the
  ///
  /// @tparam Components List of component types to add.
  ///
  /// @param[in] entity Entity to insert into the storage.
  /// @param[in] components Component data to move into the storage.
  ///
  template<typename... Components>
  requires UniqueTypes<Components...>
  void Insert(const Entity entity, Components&&... components) noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(!Contains(entity), "Entity already exists");

#ifndef NDEBUG
    ASSERT(sizeof...(Components) == components_.size(), "Invalid amount of components");
    ((ASSERT(HasComponent<std::remove_cvref_t<Components>>(), "Component type not valid")), ...);
#endif

    sparse_->Assure(entity);
    (*sparse_)[entity] = static_cast<Entity>(dense_.size());

    dense_.PushBack(entity);
    ((Access<std::remove_cvref_t<Components>>().PushBack(std::forward<Components>(components))), ...);
  }

  ///
  /// Erases the entity from the storage.
  ///
  /// This operation O(1) and is very fast.
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

    dense_.PopBack();

    erase_function_(this, index);
  }

  ///
  /// Clears the entire storage.
  ///
  /// This operation is O(1) if all component types are trivial. Otherwise the complexity is O(n).
  ///
  void Clear()
  {
    ASSERT(initialized_, "Not initialized");

    dense_.Clear();

    clear_function_(this);
  }

  ///
  /// Checks if the entity is in the storage.
  ///
  /// This operation is O(1) and is very fast.
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
  /// This operation is O(1) and is very fast.
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
  /// This operation is O(1) and is very fast.
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
    return const_cast<Component&>(static_cast<const Storage*>(this)->Unpack<Component>(entity));
  }

  ///
  /// Directly accesses the internal array of the dense storage of the component.
  ///
  /// This method is O(1) and is very fast.
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

    return *static_cast<Vector<Component>*>(component_arrays_.template Get<Component>().Get());
  }

  ///
  /// Directly accesses the internal array of the dense storage of the component.
  ///
  /// This method is O(1) and is very fast.
  ///
  /// @tparam Component The component type to access array for.
  ///
  /// @return Reference to dense array for the component type.
  ///
  template<typename Component>
  [[nodiscard]] Vector<Component>& Access() noexcept
  {
    return const_cast<Vector<Component>&>(static_cast<const Storage*>(this)->Access<Component>());
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
    return std::ranges::find(components_, TypeInfo<Component>::HashCode()) != components_.end();
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
  static void AccessAndEraseAt(Storage<Entity>* storage, size_t index)
  {
    auto& component_array = storage->template Access<Component>();
    component_array.UnorderedErase(component_array.begin() + index);
  }

private:
  using EraseFunction = void (*)(Storage<Entity>* storage, const size_t);
  using ClearFunction = void (*)(Storage<Entity>* storage);

  SharedSparseArray<Entity>* sparse_;
  Vector<Entity> dense_;

  TypeMap<ErasedPtr<void>> component_arrays_;

  // Indirect functions
  EraseFunction erase_function_;
  ClearFunction clear_function_;

  // Used for debugging purposes
#ifndef NDEBUG
  bool initialized_ = false;
  Vector<size_t> components_;
#endif
};

} // namespace genebits::engine

#endif
