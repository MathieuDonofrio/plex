#ifndef GENEBITS_ENGINE_UTIL_FAST_VECTOR_H
#define GENEBITS_ENGINE_UTIL_FAST_VECTOR_H

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/util/allocator.h"

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
/// Uses custom allocators to allow for better allocation strategies.
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
  // Style Exception: STL
  // clang-format off

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
  [[nodiscard]] constexpr iterator begin() { return array_; }
  [[nodiscard]] constexpr const_iterator begin() const { return array_; }
  [[nodiscard]] constexpr iterator end() { return array_ + size_; }
  [[nodiscard]] constexpr const_iterator end() const { return array_ + size_; }

  // Explicit const forward iterator creation methods.
  [[nodiscard]] constexpr const_iterator cbegin() const { return array_; }
  [[nodiscard]] constexpr const_iterator cend() const { return array_ + size_; }

  // Reverse iterator creation methods.
  [[nodiscard]] constexpr reverse_iterator rbegin() { return reverse_iterator(end()); }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  [[nodiscard]] constexpr reverse_iterator rend() { return reverse_iterator(begin()); }
  [[nodiscard]] constexpr const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Internal array accessors
  [[nodiscard]] constexpr pointer data() { return pointer(begin()); }
  [[nodiscard]] constexpr const_pointer data() const { return const_pointer(begin()); }
  [[nodiscard]] constexpr reference front() { return begin()[0]; }
  [[nodiscard]] constexpr const_reference front() const { return begin()[0]; }
  [[nodiscard]] constexpr reference back() { return end()[-1]; }
  [[nodiscard]] constexpr const_reference back() const { return end()[-1]; }

  // Size
  [[nodiscard]] constexpr size_t size() const { return size_; }

  // clang-format on

  ///
  /// Const array access operator.
  ///
  /// @param[in] index Index to access
  ///
  /// @return Const reference to element at the index.
  ///
  [[nodiscard]] constexpr const Type& operator[](const size_type index) const noexcept
  {
    ASSERT(index < size_, "Index out of bounds");
    return array_[index];
  }

  ///
  /// Array access operator.
  ///
  /// @param[in] index Index to access
  ///
  /// @return Reference to element at the index.
  ///
  [[nodiscard]] constexpr Type& operator[](const size_type index) noexcept
  {
    ASSERT(index < size_, "Index out of bounds");
    return array_[index];
  }

public:
  ///
  /// Default constructor
  ///
  constexpr FastVector() noexcept : array_(nullptr), size_(0), capacity_(0) {}

  ///
  /// Destructor
  ///
  ~FastVector() noexcept
  {
    if (array_) [[likely]]
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
  /// @param[in] value Element to add
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
  /// @param[in] value Element to add
  ///
  void PushBack(Type&& value) noexcept
  {
    PrepareInsertion();
    new (array_ + size_) Type(std::forward<Type>(value));
    ++size_;
  }

  ///
  /// Remove the element at the back of the vector.
  ///
  /// Always O(1) operation.
  ///
  void PopBack() noexcept
  {
    ASSERT(size_ > 0, "Vector is empty");

    --size_;
    if constexpr (!std::is_trivially_destructible_v<Type>) array_[size_].~Type();
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
  /// @param[in] value Iterator for element to erase
  ///
  void Erase(iterator it) noexcept
  {
    ASSERT(size_ > 0, "Vector is empty");

    if (size_ == 1) [[unlikely]]
    {
      if constexpr (!std::is_trivially_destructible_v<Type>) it->~Type();
      size_ = 0;
    }
    else [[likely]]
    {
      --size_;
      *it = std::move(array_[size_]);
    }
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
  /// @param[in] index Index of element to erase
  ///
  void EraseAt(size_t index) noexcept
  {
    Erase(array_ + index);
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
  /// @tparam Args Argument types for new construction.
  ///
  /// @param[in] new_size New size of the vector
  /// @param[in] args Arguments used for construction if needed.
  ///
  template<typename... Args>
  requires std::is_constructible_v<Type, Args...>
  void Resize(const size_t new_size, Args&&... args) noexcept
  {
    if (static_cast<uint32_t>(new_size) > capacity_) Grow(static_cast<uint32_t>(new_size));

    if (static_cast<uint32_t>(new_size) < size_) std::destroy(array_ + new_size, array_ + size_);
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
  /// @param[in] min_capacity Minimum capacity the vector should have.
  ///
  void Reserve(const size_t min_capacity) noexcept
  {
    if (static_cast<uint32_t>(min_capacity) > capacity_) [[likely]] { Grow(static_cast<uint32_t>(min_capacity)); }
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
  /// @param[in] other Vector to copy from.
  ///
  FastVector(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    CopyFrom(other);
  }

  ///
  /// Move constructor
  ///
  /// @param[in] other Vector to move into this one.
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
  /// @param[in] other Vector to copy from.
  ///
  /// @return Reference to this vector.
  ///
  FastVector& operator=(const FastVector<Type, AllocatorImpl>& other) noexcept
  {
    // Avoid self-assignment
    if (other.array_ == array_) return *this;

    if (array_)
    {
      DestroyAll();
      Deallocate();
    }

    CopyFrom(other);

    return *this;
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Vector to move into this one.
  ///
  /// @return Reference to this vector.
  ///
  constexpr FastVector& operator=(FastVector<Type, AllocatorImpl>&& other) noexcept
  {
    // Avoid self-move
    if (other.array_ == array_) return *this;

    array_ = other.array_;
    other.array_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;

    return *this;
  }

  ///
  /// Equality operator.
  ///
  /// @param[in] other Vector to compare.
  ///
  /// @return True if vectors are equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator==(const FastVector<Type, AllocatorImpl>& other) const noexcept
  {
    if (array_ == other.array_) return true; // Checks for same instance or two empty vectors.
    if (size_ != other.size_) return false;

    return std::equal(begin(), end(), other.begin());
  }

  ///
  /// Inequality operator.
  ///
  /// @param[in] other Vector to compare.
  ///
  /// @return True if vectors are not equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator!=(const FastVector<Type, AllocatorImpl>& other) const noexcept
  {
    return !(*this == other);
  }

protected:
  ///
  /// Grows the internal array to at least fit the specified amount of elements.
  ///
  /// @param[in] min_capacity Minimum capacity to grow to.
  ///
  void Grow(const uint32_t min_capacity) noexcept
  {
    const size_t current_capacity_bytes = sizeof(Type) * capacity_;
    const size_t min_capacity_bytes = sizeof(Type) * min_capacity;

    if constexpr (!std::is_trivially_copy_constructible_v<Type> && !std::is_trivially_move_constructible_v<Type>)
    {
      Block block = AllocatorImpl::Allocate(min_capacity_bytes);

      if (array_) [[likely]]
      {
        if constexpr ((cTypePassedByValue && std::is_copy_constructible_v<Type>) || !std::is_move_constructible_v<Type>)
        {
          std::uninitialized_copy(cbegin(), cend(), reinterpret_cast<Type*>(block.ptr));
          std::destroy(begin(), end());
        }
        else
        {
          std::uninitialized_move(begin(), end(), reinterpret_cast<Type*>(block.ptr));
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
    std::destroy(begin(), end());
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
  /// @param[in] other The other vector
  ///
  template<Allocator OtherAllocator>
  requires std::is_copy_constructible_v<Type>
  void CopyFrom(const FastVector<Type, OtherAllocator>& other) noexcept
  {
    const Block block = AllocatorImpl::Allocate(sizeof(Type) * other.size_);

    array_ = reinterpret_cast<Type*>(block.ptr);
    size_ = other.size_;
    capacity_ = static_cast<uint32_t>(block.size / sizeof(Type));

    std::uninitialized_copy(other.cbegin(), other.cend(), array_);
  }

private:
  Type* array_;

  // We use 32 bit size & capacity to reduce the size of the vector.
  uint32_t size_;
  uint32_t capacity_;
};

} // namespace genebits::engine

#endif
