#ifndef GENEBITS_ENGINE_STORAGE_H
#define GENEBITS_ENGINE_STORAGE_H

#include <concepts>

#include "genebits/engine/util/allocator.h"
#include "genebits/engine/util/concepts.h"
#include "genebits/engine/util/erased_ptr.h"
#include "genebits/engine/util/fast_vector.h"
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
  template<typename... Components>
  void Initialize() noexcept
  {
    ((pools_.template Assure<Components>().Reset(new FastVector<Components, DenseAllocator>())), ...);

    erase_function_ = []([[maybe_unused]] auto storage, [[maybe_unused]] const size_t index)
    { ((storage->template Get<Components>().EraseAt(index)), ...); };
    clear_function_ = []([[maybe_unused]] auto storage) { ((storage->template Get<Components>().Clear()), ...); };
  }

  template<typename... Components>
  void Insert(const Entity entity, const Components&... components) noexcept
  {
    // TODO assert components
    ASSERT(!Contains(entity), "Entity already exists");

    sparse_.Assure(entity);
    sparse_[entity] = static_cast<Entity>(dense_.Size());

    dense_.PushBack(entity);
    ((Get<Components>().PushBack(components)), ...);
  }

  void Erase(const Entity entity) noexcept
  {
    ASSERT(Contains(entity), "Entity does not exist");

    const auto index = sparse_[entity];

    dense_.EraseAt(index);
    erase_function_(this, index);
  }

  void Clear()
  {
    dense_.Clear();

    clear_function_(this);
  }

  [[nodiscard]] constexpr bool Contains(const Entity entity) const noexcept
  {
    size_t index;

    return entity < sparse_.Capacity() && (index = sparse_[entity]) < dense_.Size() && dense_[index] == entity;
  }

  template<typename Component>
  [[nodiscard]] const Component& Unpack(const Entity entity) const noexcept
  {
    // TODO assert component
    ASSERT(Contains(entity), "Entity does not exist");

    return Get<Component>()[sparse_[entity]];
  }

  template<typename Component>
  [[nodiscard]] Component& Unpack(const Entity entity) noexcept
  {
    return const_cast<Component&>(const_cast<const Storage*>(this)->Unpack<Component>(entity));
  }

  [[nodiscard]] bool Empty() const noexcept
  {
    return dense_.Empty();
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    return dense_.Size();
  }

public:
  class iterator final
  {
  public:
    using iterator_category = std::random_access_iterator_tag;

    using size_type = size_t;

    iterator(Storage* const ptr, const size_type index) : ptr_ { ptr }, index_ { index } {}

    // Style Exception: STL
    // clang-format off

    iterator& operator+=(const size_type value) { index_ += value; return *this; }
    iterator& operator-=(const size_type value) { index_ -= value; return *this; }

    iterator& operator++() { return ++index_, *this; }
    iterator& operator--() { return --index_, *this; }

    iterator operator+(const size_type value) const { return { ptr_, index_ + value }; }
    iterator operator-(const size_type value) const { return { ptr_, index_ - value }; }

    bool operator==(const iterator other) const { return other._pos == index_; }
    bool operator!=(const iterator other) const { return other._pos != index_; }

    bool operator<(const iterator other) const { return other._pos < index_; }
    bool operator>(const iterator other) const { return other._pos > index_; }

    bool operator<=(const iterator other) const { return other._pos <= index_; }
    bool operator>=(const iterator other) const { return other._pos >= index_; }

    [[nodiscard]] Entity operator*() const { return ptr_->dense_[index_]; }

    template<typename Component>
    [[nodiscard]] const Component& Unpack() const
    {
      ASSERT(ptr_->Contains(index_), "Entity does not exist");

      return ptr_->Get<Component>()[index_];
    }

    template<typename Component>
    [[nodiscard]] Component& Unpack()
    {
      return const_cast<Component&>(const_cast<const iterator*>(this)->Unpack<Component>());
    }

    // clang-format on

  private:
    Storage* const ptr_;
    size_type index_;
  };

  // Style Exception: STL
  // clang-format off

  using const_iterator = const iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = Entity;

  using reference = Entity&;
  using const_reference = const Entity&;
  using pointer = Entity*;
  using const_pointer = const Entity*;

  // Forward iterator creation methods.
  iterator begin() { return iterator(this, 0); }
  const_iterator begin() const { return iterator(this, 0); }
  iterator end() { return iterator(this, Size()); }
  const_iterator end() const { return iterator(this, Size()); }

  // Explicit const forward iterator creation methods.
  const_iterator cbegin() const { return iterator(this, 0); }
  const_iterator cend() const { return iterator(this, Size()); }

  // Reverse iterator creation methods.
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // clang-format on

private:
  template<typename Component>
  const FastVector<Component, DenseAllocator>& Get() const noexcept
  {
    return *pools_.Get<Component>().template Cast<FastVector<Component, DenseAllocator>>();
  }

  template<typename Component>
  FastVector<Component, DenseAllocator>& Get() noexcept
  {
    return const_cast<FastVector<Component, DenseAllocator>&>(const_cast<const Storage*>(this)->Get<Component>());
  }

private:
  using EraseFunction = void (*)(Storage* storage, const size_t);
  using ClearFunction = void (*)(Storage* storage);

  FastVector<Entity, DenseAllocator> dense_;
  SharedSparseArray<Entity, SparseAllocator> sparse_;
  TypeMap<ErasedPtr<void>> pools_;

  // Functions
  EraseFunction erase_function_;
  ClearFunction clear_function_;
};

} // namespace genebits::engine

#endif
