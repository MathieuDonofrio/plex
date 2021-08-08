#ifndef GENEBITS_ENGINE_UTIL_FASTVECTOR_H
#define GENEBITS_ENGINE_UTIL_FASTVECTOR_H

#include <iterator>
#include <type_traits>
#include <utility>

#include "engine/util/allocator.h"

namespace genebits::engine
{
// Make vector faster than std::vector in certain cases by making these optimizations:
// - Less flexibility: harder to use wrong
// - Unordered: Some possibilities to optimize
// - Genebits Allocators: Less overhead than std::allocator & more strategies flexibility
// - POD Optimizations: Example using c realloc

template<typename Type>
concept FastVectorType = std::is_copy_constructible_v<Type> || std::is_move_constructible_v<Type>;

template<FastVectorType Type, Allocator AllocatorImpl = Mallocator>
class FastVector : private AllocatorImpl
{
private:
  static constexpr bool cSmallType = sizeof(Type) <= 2 * sizeof(void*);
  static constexpr bool cTypePassedByValue = cSmallType && std::is_trivially_copy_constructible_v<Type>;

public:
  using iterator_category = std::random_access_iterator_tag;

  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = Type;
  using iterator = Type*;
  using const_iterator = const Type*;

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = std::reverse_iterator<iterator>;

  using reference = Type&;
  using const_reference = const Type&;
  using pointer = Type*;
  using const_pointer = const Type*;

  // Forward iterator creation methods.
  iterator begin() { return array_; }
  const_iterator begin() const { return array_; }
  iterator end() { return array_ + size_; }
  const_iterator end() const { return array_ + size_; }

  // Explicit const forward iterator creation methods.
  const_iterator cbegin() const { return array_; }
  const_iterator cend() const { return array_ + size_; }

  // Reverse iterator creation methods.
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  pointer data() { return pointer(begin()); }
  const_pointer data() const { return const_pointer(begin()); }

  reference front() { return begin()[0]; }
  const_reference front() const { return begin()[0]; }
  reference back() { return end()[-1]; }
  const_reference back() const { return end()[-1]; }

  [[nodiscard]] const_reference operator[](const size_type index) const noexcept
  {
    return array_[index];
  }

  [[nodiscard]] reference operator[](const size_type index) noexcept
  {
    return array_[index];
  }

public:
  FastVector()
    : array_(nullptr),
      size_(0), capacity_(0)
  {
  }

  ~FastVector()
  {
    DestroyAll();
    Deallocate();
  }

  void PushBack(const Type& value) noexcept
  {
    PrepareInsertion();
    new (array_ + size_) Type(value);
    ++size_;
  }

  void PushBack(Type&& value) noexcept
  {
    PrepareInsertion();
    new (array_ + size_) Type(std::move(value));
    ++size_;
  }

  void PopBack() noexcept
  {
    --size_;
    array_[size_].~Type();
  }

  void Erase(size_t index) noexcept
  {
    if (size_ == 1) [[unlikely]]
    {
      array_[index].~Type();
      size_ = 0;
    }
    else [[likely]]
    {
      --size_;
      array_[index] = std::move(array_[size_]);
    }
  }

  template<typename... Args>
  requires std::is_constructible_v<Type, Args...>
  void Resize(const size_t new_size, Args&&... args) noexcept
  {
    if (static_cast<uint32_t>(new_size) > capacity_)
    {
      Grow(static_cast<uint32_t>(new_size));
    }

    if (static_cast<uint32_t>(new_size) < size_)
    {
      for (size_t i = size_; i != new_size; i--)
      {
        array_[i].~Type();
      }
    }
    else
    {
      for (size_t i = size_; i != new_size; i++)
      {
        new (array_ + i) Type(std::forward<Args>(args)...);
      }
    }

    size_ = static_cast<uint32_t>(new_size);
  }

  void Reserve(const size_t min_capacity) noexcept
  {
    if (static_cast<uint32_t>(min_capacity) > capacity_)
    {
      Grow(static_cast<uint32_t>(min_capacity));
    }
  }

  void Clear() noexcept
  {
    DestroyAll();

    size_ = 0;
  }

  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return size_;
  }

  [[nodiscard]] constexpr size_t Capacity() const noexcept
  {
    return capacity_;
  }

  [[nodiscard]] constexpr bool Empty() const noexcept
  {
    return size_ == 0;
  }

public:
  FastVector(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    CopyFrom(other);
  }

  constexpr FastVector(FastVector<Type, AllocatorImpl>&& other) noexcept
    : array_(other.array_), size_(other.size_), capacity_(other.capacity_)
  {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  constexpr FastVector& operator=(FastVector<Type, AllocatorImpl>&& other) noexcept
  {
    array_ = other.array_;
    other.array_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
  }

  FastVector& operator=(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    if (array_)
    {
      DestroyAll();
      Deallocate();
    }

    CopyFrom(other);
  }

protected:
  void Grow(const uint32_t min_capacity) noexcept
  {
    const size_t current_capacity_bytes = sizeof(Type) * capacity_;
    const size_t min_capacity_bytes = sizeof(Type) * min_capacity;

    if constexpr (!std::is_trivially_copy_constructible_v<Type> && !std::is_trivially_move_constructible_v<Type>)
    {
      Block block = AllocatorImpl::Allocate(min_capacity_bytes);

      if (array_)
      {
        for (uint32_t i = 0; i != size_; i++)
        {
          Type* old_element = array_ + i;
          Type* new_element = reinterpret_cast<Type*>(block.ptr) + i;

          if constexpr (cTypePassedByValue)
          {
            new (new_element) Type(*old_element);
          }
          else if constexpr (std::is_move_constructible_v<Type>)
          {
            new (new_element) Type(std::move(*old_element));
          }
          else
          {
            new (new_element) Type(*old_element);
          }

          if constexpr (!std::is_trivially_destructible_v<Type>)
          {
            old_element->~Type();
          }
        }

        AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(array_), current_capacity_bytes });
      }

      array_ = reinterpret_cast<Type*>(block.ptr);
      capacity_ = static_cast<uint32_t>(block.size / sizeof(Type));
    }
    else
    {
      Block block { reinterpret_cast<char*>(array_), current_capacity_bytes };

      AllocatorImpl::Reallocate(block, min_capacity_bytes);

      array_ = reinterpret_cast<Type*>(block.ptr);
      capacity_ = static_cast<uint32_t>(block.size / sizeof(Type));
    }
  }

  void GoldenGrow() noexcept
  {
    // We want the growth rate to be close to the golden ratio (~1.618). The golden ration
    // maximizes reuse and is the optimal reallocation size. For simplicity and calculation
    // speed we use 1.5 and small constant amount.
    const uint32_t new_capacity = capacity_ + (capacity_ >> 1) + 8; // capacity * 1.5 + 8

    Grow(new_capacity);
  }

  void PrepareInsertion() noexcept
  {
    if (size_ == capacity_) [[unlikely]]
      GoldenGrow();
  }

  void DestroyAll() noexcept
  {
    if constexpr (!std::is_trivially_destructible_v<Type>)
    {
      for (auto& element : *this)
      {
        element.~Type();
      }
    }
  }

  void Deallocate() noexcept
  {
    AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(array_), capacity_ });
  }

  template<Allocator OtherAllocator>
  void CopyFrom(const FastVector<Type, OtherAllocator>& other) noexcept
  {
    const Block block = AllocatorImpl::Allocate(sizeof(Type) * other.size_);

    array_ = reinterpret_cast<Type*>(block.ptr);
    size_ = other.size_;
    capacity_ = static_cast<uint32_t>(block.size / sizeof(Type));

    for (uint32_t i = 0; i != size_; i++)
    {
      const Type* old_element = other.array_ + i;

      new (array_ + i) Type(*old_element);
    }
  }

private:
  Type* array_;

  // We use 32 bit size & capacity to reduce the size of the vector.
  uint32_t size_;
  uint32_t capacity_;
};

} // namespace genebits::engine

#endif
