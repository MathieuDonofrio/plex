#ifndef GENEBITS_ENGINE_STORAGE_H
#define GENEBITS_ENGINE_STORAGE_H

#include <concepts>

#include "genebits/engine/util/allocator.h"
#include "genebits/engine/util/concepts.h"
#include "genebits/engine/util/erased_ptr.h"
#include "genebits/engine/util/fast_vector.h"
#include "genebits/engine/util/meta.h"
#include "genebits/engine/util/type_map.h"

namespace genebits::engine
{

template<std::unsigned_integral Entity, Allocator AllocatorImpl = Mallocator>
class SharedSparseArray : private AllocatorImpl
{
public:
  constexpr SharedSparseArray() noexcept : array_(nullptr), capacity_(0), references_(0) {}

  ~SharedSparseArray()
  {
    if (!references_ && array_)
    {
      AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(array_), sizeof(Entity) * capacity_ });
    }

    references_--;
  }

  SharedSparseArray(const SharedSparseArray&) = delete;
  SharedSparseArray(SharedSparseArray&&) = delete;
  SharedSparseArray& operator=(const SharedSparseArray&) = delete;
  SharedSparseArray& operator=(SharedSparseArray&&) = delete;

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

  [[nodiscard]] constexpr Entity operator[](const Entity entity) const noexcept
  {
    return array_[entity];
  }

  [[nodiscard]] constexpr Entity& operator[](const Entity entity) noexcept
  {
    return array_[entity];
  }

  [[nodiscard]] constexpr size_t Capacity() const noexcept
  {
    return capacity_;
  }

private:
  Entity* array_;
  uint32_t capacity_;
  uint32_t references_;
};

template<std::unsigned_integral Entity, Allocator DenseAllocator = Mallocator, Allocator SparseAllocator = Mallocator>
class Storage : private DenseAllocator
{
public:
  // Style Exception: STL
  // clang-format off

  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = Entity;

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

  [[nodiscard]] constexpr const Entity& operator[](const size_type index) const noexcept
  {
    return dense_[index];
  }

  [[nodiscard]] constexpr Entity& operator[](const size_type index) noexcept
  {
    return dense_[index];
  }

public:
  Storage()
  {
    sparse_ = new SharedSparseArray<Entity>();
  }

  Storage(SharedSparseArray<Entity>* sparse)
  {
    ASSERT(sparse != nullptr, "Sparse array cannot be nullptr");

    sparse_ = sparse;
  }

  ~Storage()
  {
    delete sparse_;
  }

  Storage(const Storage&) = delete;
  Storage(Storage&&) = delete;
  Storage& operator=(const Storage&) = delete;
  Storage& operator=(Storage&&) = delete;

  template<typename... Components>
  void Initialize() noexcept
  {
    ASSERT(!initialized_, "Already initialized");

    ((pools_.template Assure<Components>().Reset(new FastVector<Components, DenseAllocator>())), ...);

    erase_function_ = []([[maybe_unused]] auto storage, [[maybe_unused]] const size_t index)
    { ((storage->template Access<Components>().EraseAt(index)), ...); };
    clear_function_ = []([[maybe_unused]] auto storage) { ((storage->template Access<Components>().Clear()), ...); };

#ifndef NDEBUG
    ((components_.PushBack(Meta<Components>::Hash())), ...);
    initialized_ = true;
#endif
  }

  template<typename... Components>
  void Insert(const Entity entity, const Components&... components) noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(!Contains(entity), "Entity already exists");
    ASSERT(sizeof...(Components) == components_.Size(), "Invalid amount of components");
#ifndef NDEBUG
    ((ASSERT(HasComponent<Components>(), "Component type not valid")), ...);
#endif

    sparse_->Assure(entity);
    (*sparse_)[entity] = static_cast<Entity>(dense_.Size());

    dense_.PushBack(entity);
    ((Access<Components>().PushBack(components)), ...);
  }

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

  void Clear()
  {
    ASSERT(initialized_, "Not initialized");

    dense_.Clear();

    clear_function_(this);
  }

  [[nodiscard]] constexpr bool Contains(const Entity entity) const noexcept
  {
    ASSERT(initialized_, "Not initialized");

    size_t index;

    return entity < sparse_->Capacity() && (index = (*sparse_)[entity]) < dense_.Size() && dense_[index] == entity;
  }

  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist");

    return Access<Component>()[sparse_->operator[](entity)];
  }

  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(const_cast<const Storage*>(this)->Unpack<Component>(entity));
  }

  template<typename Component>
  [[nodiscard]] const FastVector<Component, DenseAllocator>& Access() const noexcept
  {
    ASSERT(initialized_, "Not initialized");
    ASSERT(HasComponent<Component>(), "Component type not valid");

    return *pools_.Get<Component>().template Cast<FastVector<Component, DenseAllocator>>();
  }

  template<typename Component>
  [[nodiscard]] FastVector<Component, DenseAllocator>& Access() noexcept
  {
    return const_cast<FastVector<Component, DenseAllocator>&>(const_cast<const Storage*>(this)->Access<Component>());
  }

  [[nodiscard]] bool Empty() const noexcept
  {
    return dense_.Empty();
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    return dense_.Size();
  }

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

  template<typename Component>
  [[nodiscard]] bool HasComponent() const noexcept
  {
    return std::ranges::find(components_, Meta<Component>::Hash()) != components_.end();
  }
#endif
};

} // namespace genebits::engine

#endif
