#ifndef GENEBITS_ENGINE_STORAGE_H
#define GENEBITS_ENGINE_STORAGE_H

#include "genebits/engine/util/allocator.h"
#include "genebits/engine/util/concepts.h"
#include "genebits/engine/util/erased_ptr.h"
#include "genebits/engine/util/fast_vector.h"
#include "genebits/engine/util/meta.h"
#include "genebits/engine/util/type_map.h"

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
template<std::unsigned_integral Entity, Allocator AllocatorImpl = Mallocator>
class SharedSparseArray : private AllocatorImpl
{
public:
  ///
  /// Constructor.
  ///
  constexpr SharedSparseArray() noexcept : array_(nullptr), capacity_(0) {}

  ///
  /// Destructor.
  ///
  ~SharedSparseArray()
  {
    AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(array_), sizeof(Entity) * capacity_ });
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
    if (entity >= capacity_)
    {
      Block block { reinterpret_cast<char*>(array_), capacity_ * sizeof(Entity) };

      capacity_ += static_cast<uint32_t>(entity + 1);

      AllocatorImpl::Reallocate(block, capacity_ * sizeof(Entity));

      array_ = reinterpret_cast<Entity*>(block.ptr);
      capacity_ = static_cast<uint32_t>(block.size / sizeof(Entity));
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
/// Basically a sparse set, but optimized for storing extra data, in this case component data.
///
/// Component data is contiguously stored in memory and can achieve near vector iteration speeds.
/// Insertion and erasing is constant time.
///
/// @warning
///  The order is never guaranteed.
///
/// @warning
///  Storage data is type erased using an unsafe initialization design. This allows for more performance
///  but is undefined behaviour if the storage is not initialized.
///
/// @tparam DenseAllocator Allocator to use for dense arrays.
/// @tparam SparseAllocator Allocator to use for sparse array.
///
template<std::unsigned_integral Entity, Allocator DenseAllocator = Mallocator, Allocator SparseAllocator = Mallocator>
class Storage : private DenseAllocator
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
  explicit Storage(SharedSparseArray<Entity, SparseAllocator>* sparse) noexcept
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

    // Set up all the pools with type erased vectors.
    ((pools_.template Assure<Components>().Reset(new FastVector<Components, DenseAllocator>())), ...);

    // Store functors for the operations that need type information and don't have it.
    erase_function_ = []([[maybe_unused]] auto storage, [[maybe_unused]] const size_t index)
    { ((storage->template Access<Components>().EraseAt(index)), ...); };
    clear_function_ = []([[maybe_unused]] auto storage) { ((storage->template Access<Components>().Clear()), ...); };

#ifndef NDEBUG
    // When debugging it is useful to have the list of components used at initialization.
    ((components_.PushBack(Meta<Components>::Hash())), ...);
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
    ASSERT(sizeof...(Components) == components_.Size(), "Invalid amount of components");
    ((ASSERT(HasComponent<std::remove_cvref_t<Components>>(), "Component type not valid")), ...);
#endif

    sparse_->Assure(entity);
    (*sparse_)[entity] = static_cast<Entity>(dense_.Size());

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

    return entity < sparse_->Capacity() && (index = (*sparse_)[entity]) < dense_.Size() && dense_[index] == entity;
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
    return const_cast<Component&>(const_cast<const Storage*>(this)->Unpack<Component>(entity));
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
  [[nodiscard]] const FastVector<Component, DenseAllocator>& Access() const noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(HasComponent<Component>(), "Component type not valid");

    return *pools_.Get<Component>().template Cast<FastVector<Component, DenseAllocator>>();
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
  [[nodiscard]] FastVector<Component, DenseAllocator>& Access() noexcept
  {
    return const_cast<FastVector<Component, DenseAllocator>&>(const_cast<const Storage*>(this)->Access<Component>());
  }

  ///
  /// Returns whether or not the storage contains any entities.
  ///
  /// @return True if the storage is empty, false otherwise.
  ///
  [[nodiscard]] bool Empty() const noexcept
  {
    return dense_.Empty();
  }

  ///
  /// Returns the amount of entities in the storage.
  ///
  /// @return Size of storage.
  ///
  [[nodiscard]] size_t Size() const noexcept
  {
    return dense_.Size();
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
    return std::ranges::find(components_, Meta<Component>::Hash()) != components_.end();
  }
#endif

private:
  using EraseFunction = void (*)(Storage* storage, const size_t);
  using ClearFunction = void (*)(Storage* storage);

  SharedSparseArray<Entity, SparseAllocator>* sparse_;

  FastVector<Entity, DenseAllocator> dense_;
  TypeMap<ErasedPtr<void>> pools_;

  // Functions
  EraseFunction erase_function_;
  ClearFunction clear_function_;

  // Used for debugging purposes
#ifndef NDEBUG
  bool initialized_ = false;
  FastVector<size_t> components_;
#endif
};

} // namespace genebits::engine

#endif