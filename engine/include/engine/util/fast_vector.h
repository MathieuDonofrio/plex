#ifndef GENEBITS_ENGINE_UTIL_FASTVECTOR_H
#define GENEBITS_ENGINE_UTIL_FASTVECTOR_H

#include <iterator>
#include <type_traits>
#include <utility>

#include "engine/util/allocator.h"

namespace genebits::engine
{
///
/// Concept that determines if the type can be used as in a fast vector.
///
/// @tparam Type The type to check
///
template<typename Type>
concept FastVectorType = std::is_copy_constructible_v<Type> || std::is_move_constructible_v<Type>;

///
/// Fast unordered vector optimized for performance.
///
/// Order is never guaranteed. This allows us to optimize certain operations such
/// as erasing.
///
/// Optimizes for POD types. Uses reallocation when possible to optimize growing.
///
/// Uses custom allocators to allow for better allocation stratergies.
///
/// @warning
///     Not a replacement for std::vector. Used internally in the engine in the cases
///     that can benefit from it.
///
/// @tparam Type The type the vector contains.
/// @tparam AllocatorImpl The allocator to use for allocating memory.
///
template<FastVectorType Type, Allocator AllocatorImpl = Mallocator>
class FastVector : private AllocatorImpl
{
private:
  // If the type is small and trivially copyable we don't need to pass it by reference
  // in certain cases. This is sometimes an optimization.
  static constexpr bool cSmallType = sizeof(Type) <= 2 * sizeof(void*);
  static constexpr bool cTypePassedByValue = cSmallType && std::is_trivially_copy_constructible_v<Type>;

public:
  // The following definitions are for STL iterator compatibility

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

  // Internal array accessor
  pointer data() { return pointer(begin()); }
  const_pointer data() const { return const_pointer(begin()); }

  // Explicit array accessor
  reference front() { return begin()[0]; }
  const_reference front() const { return begin()[0]; }
  reference back() { return end()[-1]; }
  const_reference back() const { return end()[-1]; }

  ///
  /// Const array access operator.
  ///
  /// @param index Index to access
  ///
  /// @return Const reference to element at the index.
  ///
  [[nodiscard]] const_reference operator[](const size_type index) const noexcept
  {
    return array_[index];
  }

  ///
  /// Array access operator.
  ///
  /// @param index Index to access
  ///
  /// @return Reference to element at the index.
  ///
  [[nodiscard]] reference operator[](const size_type index) noexcept
  {
    return array_[index];
  }

public:
  ///
  /// Default constructor
  ///
  FastVector()
    : array_(nullptr), size_(0), capacity_(0)
  {
  }

  ///
  /// Destructor
  ///
  ~FastVector()
  {
    if (array_)
    {
      DestroyAll();
      Deallocate();
    }
  }

  ///
  /// Add an element to the back of the vector.
  ///
  /// Best case O(1) operation (common), when vector has sufficient capacity.
  /// Worst case O(n) operation (rare), where n is the size of the vector.
  ///
  /// @param value Element to add
  ///
  void PushBack(const Type& value) noexcept
  {
    PrepareInsertion();
    new (array_ + size_) Type(value);
    ++size_;
  }

  ///
  /// Add an element to the back of the vector.
  ///
  /// Best case O(1) operation (common), when vector has sufficient capacity.
  /// Worst case O(n) operation (rare), where n is the size of the vector.
  ///
  /// @param value Element to add
  ///
  void PushBack(Type&& value) noexcept
  {
    PrepareInsertion();
    new (array_ + size_) Type(std::move(value));
    ++size_;
  }

  ///
  /// Remove the element at the back of the vector.
  ///
  /// Always O(1) operation.
  ///
  /// @param value Element to add
  ///
  void PopBack() noexcept
  {
    --size_;
    array_[size_].~Type();
  }

  ///
  /// Remove the element at the specified index. Uses swap and pop technique.
  ///
  /// @warning
  ///     This changes the order of elements by swapping the element at the end of
  ///     the vector with the one at the erased index.
  ///
  /// Always O(1) operation.
  ///
  /// @param value Element to add
  ///
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

  ///
  /// Resizes the vector and constructs all the new elements (if any) using
  /// the provided arguments.
  ///
  /// If used to increase the size, new elements will be constructed.
  /// If used to decrease the size, old elements will be destroyed.
  ///
  /// Will grow the internal array is the new size if larger than the capacity.
  ///
  /// Best case O(n) operation, where n is the difference between current size and old size.
  /// Worst case O(n) operation, where n is the size of the vector.
  ///
  /// @tparam Args
  /// @param new_size
  /// @param args
  ///
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

  ///
  /// If necessary, increases the capacity of the vector to be able to fit
  /// at least the specified amount.
  ///
  /// Best case O(1).
  /// Worst case O(n), where n is the size of the vector.
  ///
  /// @param min_capacity Minimum capacity the vector should have.
  ///
  void Reserve(const size_t min_capacity) noexcept
  {
    if (static_cast<uint32_t>(min_capacity) > capacity_)
    {
      Grow(static_cast<uint32_t>(min_capacity));
    }
  }

  ///
  /// Clears the vector of all its contents without deallocating the memory.
  ///
  /// Always O(n), where n is the size of the vector.
  ///
  void Clear() noexcept
  {
    DestroyAll();

    size_ = 0;
  }

  ///
  /// Returns the amount of elements currently stored in the vector.
  ///
  /// @return Size of the vector.
  ///
  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return size_;
  }

  ///
  /// Returns the current maximum amount of elements that can be stored
  /// in the vector without needing to grow the internal array.
  ///
  /// @return Capacity of the vector.
  ///
  [[nodiscard]] constexpr size_t Capacity() const noexcept
  {
    return capacity_;
  }

  ///
  /// Returns whether or not the vector is empty.
  ///
  /// @return True if the vector is empty, false otherwise.
  ///
  [[nodiscard]] constexpr bool Empty() const noexcept
  {
    return size_ == 0;
  }

public:
  ///
  /// Copy constructor.
  ///
  /// @param other Vector to copy from.
  ///
  FastVector(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    CopyFrom(other);
  }

  ///
  /// Move constructor
  ///
  /// @param other Vector to move into this one.
  ///
  constexpr FastVector(FastVector<Type, AllocatorImpl>&& other) noexcept
    : array_(other.array_), size_(other.size_), capacity_(other.capacity_)
  {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param other Vector to copy from.
  ///
  FastVector& operator=(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    if (array_)
    {
      DestroyAll();
      Deallocate();
    }

    CopyFrom(other);
  }

  ///
  /// Move assignment operator.
  ///
  /// @param other Vector to move into this one.
  ///
  constexpr FastVector& operator=(FastVector<Type, AllocatorImpl>&& other) noexcept
  {
    array_ = other.array_;
    other.array_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
  }

protected:
  ///
  /// Grows the internal array to at least fit the specified amount of elements.
  ///
  /// @param min_capacity Minimum capacity to grow to.
  ///
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

  ///
  /// Grows internal array using pseudo-golden ratio growth.
  ///
  void GoldenGrow() noexcept
  {
    // We want the growth rate to be close to the golden ratio (~1.618). The golden ration
    // maximizes reuse and is the optimal reallocation size. For simplicity and calculation
    // speed we use 1.5 and small constant amount.
    const uint32_t new_capacity = capacity_ + (capacity_ >> 1) + 8; // capacity * 1.5 + 8

    Grow(new_capacity);
  }

  ///
  /// Makes sure the vector has enough available space to add an new element to it.
  ///
  void PrepareInsertion() noexcept
  {
    if (size_ == capacity_) [[unlikely]]
      GoldenGrow();
  }

  ///
  /// Destroys all the elements in the vector.
  ///
  /// @warning
  ///     Does not change size.
  ///
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

  ///
  /// Deallocates the current internal array.
  ///
  void Deallocate() noexcept
  {
    AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(array_), capacity_ });
    array_ = nullptr;
  }

  ///
  /// Copies an other vector into this vector.
  ///
  /// @tparam OtherAllocator The allocator the other vector uses.
  ///
  /// @param other The other vector
  ///
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
