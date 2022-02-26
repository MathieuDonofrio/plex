#ifndef GENEBITS_ENGINE_CONTAINERS_VECTOR_H
#define GENEBITS_ENGINE_CONTAINERS_VECTOR_H

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/os/allocator.h"
#include "genebits/engine/utilities/type_traits.h"

namespace genebits::engine
{
///
/// Concept that determines if the type can be used as in a vector.
///
/// @tparam Type The type to check
///
template<typename Type>
concept VectorType = std::is_copy_constructible_v<Type> || std::is_move_constructible_v<Type>;

namespace details
{
  template<VectorType Type>
  class VectorBase
  {
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

    ///
    /// Returns the amount of elements currently stored in the vector.
    ///
    /// @return Size of the vector.
    ///
    [[nodiscard]] constexpr size_t size() const noexcept
    {
      return size_;
    }

    ///
    /// Returns the current maximum amount of elements that can be stored
    /// in the vector without needing to grow the internal array.
    ///
    /// @return Capacity of the vector.
    ///
    [[nodiscard]] constexpr size_t capacity() const noexcept
    {
      return capacity_;
    }

    ///
    /// Returns whether or not the vector is empty.
    ///
    /// @return True if the vector is empty, false otherwise.
    ///
    [[nodiscard]] constexpr bool empty() const noexcept
    {
      return size_ == 0;
    }

  protected:
    ///
    /// Default constructor
    ///
    constexpr VectorBase() noexcept : array_(nullptr), size_(0), capacity_(0) {}

    ///
    /// Parametric constructor
    ///
    constexpr VectorBase(Type* array, uint32_t size, uint32_t capacity) noexcept
      : array_(array), size_(size), capacity_(capacity)
    {}

  protected:
    Type* array_;

    // We use 32 bit size & capacity to reduce the size of the vector on 64 bit.
    uint32_t size_;
    uint32_t capacity_;
  };
} // namespace details

///
/// Fast unordered vector optimized for performance.
///
/// Order is never guaranteed. This allows us to optimize certain operations such
/// as erasing.
///
/// Optimized for small trivially copyable types. Uses reallocation when possible to optimize growing.
///
/// Uses custom allocators to allow for better allocation strategies.
///
/// @warning
///     Not a replacement for std::vector. Used internally in the engine in the cases
///     that can benefit from it.
///
/// @tparam Type The type the vector contains.
/// @tparam AllocatorType The allocator to use for allocating memory.
///
template<VectorType Type, Allocator AllocatorType = Mallocator>
class Vector : public details::VectorBase<Type>, private AllocatorType
{
private:
  using SuperClass = typename details::VectorBase<Type>;

public:
  using iterator = typename SuperClass::iterator;
  using const_iterator = typename SuperClass::const_iterator;
  using reference = typename SuperClass::reference;
  using size_type = typename SuperClass::size_type;

  ///
  /// Default constructor
  ///
  constexpr Vector() noexcept : details::VectorBase<Type>() {}

  // TODO constructor with initializer list

  ///
  /// Destructor
  ///
  ~Vector() noexcept
  {
    if (this->array_) [[likely]]
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
    new (this->end()) Type(value);
    ++this->size_;
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
    new (this->end()) Type(std::forward<Type>(value));
    ++this->size_;
  }

  // TODO emplace back

  ///
  /// Remove the element at the back of the vector.
  ///
  /// Always O(1) operation.
  ///
  void PopBack() noexcept
  {
    ASSERT(this->size_ > 0, "Vector is empty");

    --this->size_;
    if constexpr (!std::is_trivially_destructible_v<Type>) this->array_[this->size_].~Type();
  }

  // TODO use normal erase & and SwapAndPop

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
    ASSERT(this->size_ > 0, "Vector is empty");

    if (this->size_ == 1) [[unlikely]]
    {
      if constexpr (!std::is_trivially_destructible_v<Type>) it->~Type();
      this->size_ = 0;
    }
    else [[likely]]
    {
      --this->size_;
      *it = std::move(*this->end());
    }
  }

  // TODO erase at is pointless

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
    Erase(this->array_ + index);
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
    if (static_cast<uint32_t>(new_size) > this->capacity_) Grow(static_cast<uint32_t>(new_size));

    if (static_cast<uint32_t>(new_size) < this->size_) std::destroy(this->begin() + new_size, this->end());
    else
    {
      for (size_t i = this->size_; i != new_size; i++)
      {
        new (this->array_ + i) Type(std::forward<Args>(args)...);
      }
    }

    this->size_ = static_cast<uint32_t>(new_size);
  }

  // TODO shrink to fit. Should be pretty cheap because of realloc.

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
    if (static_cast<uint32_t>(min_capacity) > this->capacity_) [[likely]] { Grow(static_cast<uint32_t>(min_capacity)); }
  }

  ///
  /// Clears the vector of all its contents without deallocating the memory.
  ///
  /// Always O(n), where n is the size of the vector.
  ///
  void Clear() noexcept
  {
    DestroyAll();

    this->size_ = 0;
  }

public:
  ///
  /// Copy constructor.
  ///
  /// @param[in] other Vector to copy from.
  ///
  Vector(const Vector<Type, AllocatorType>& other) noexcept
  {
    CopyFrom(other);
  }

  ///
  /// Move constructor
  ///
  /// @param[in] other Vector to move into this one.
  ///
  constexpr Vector(Vector<Type, AllocatorType>&& other) noexcept
    : details::VectorBase<Type>(other.array_, other.size_, other.capacity_)
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
  Vector& operator=(const Vector<Type, AllocatorType>& other) noexcept
  {
    // TODO use swap

    // Avoid self-assignment
    if (other.array_ == this->array_) return *this;

    if (this->array_)
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
  constexpr Vector& operator=(Vector<Type, AllocatorType>&& other) noexcept
  {
    // TODO use swap

    // Avoid self-move
    if (other.array_ == this->array_) return *this;

    this->array_ = other.array_;
    other.array_ = nullptr;
    this->size_ = other.size_;
    other.size_ = 0;
    this->capacity_ = other.capacity_;
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
  [[nodiscard]] constexpr bool operator==(const Vector<Type, AllocatorType>& other) const noexcept
  {
    if (this->array_ == other.array_) return true; // Checks for same instance or two empty vectors.
    if (this->size_ != other.size_) return false;

    return std::equal(this->begin(), this->end(), other.begin());
  }

  ///
  /// Inequality operator.
  ///
  /// @param[in] other Vector to compare.
  ///
  /// @return True if vectors are not equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator!=(const Vector<Type, AllocatorType>& other) const noexcept
  {
    return !(*this == other);
  }

protected:
  ///
  /// Grows the internal array to at least fit the specified amount of elements.
  ///
  /// @param[in] new_capacity Minimum capacity to grow to.
  ///
  void Grow(const uint32_t new_capacity) noexcept
  {
    const size_t current_capacity_bytes = sizeof(Type) * this->capacity_;
    const size_t new_capacity_bytes = sizeof(Type) * new_capacity;

    if constexpr (IsRelocatable<Type>::value)
    {
      Block block { reinterpret_cast<char*>(this->array_), current_capacity_bytes };

      AllocatorType::Reallocate(block, new_capacity_bytes);

      this->array_ = reinterpret_cast<Type*>(block.ptr);
    }
    else
    {
      Block block = AllocatorType::Allocate(new_capacity_bytes);

      if (this->array_) [[likely]]
      {
        std::uninitialized_move(this->begin(), this->end(), reinterpret_cast<Type*>(block.ptr));
        std::destroy(this->begin(), this->end());

        AllocatorType::Deallocate(Block { reinterpret_cast<char*>(this->array_), current_capacity_bytes });
      }

      this->array_ = reinterpret_cast<Type*>(block.ptr);
    }

    this->capacity_ = new_capacity;
  }

  ///
  /// Grows internal array by at least a factor of two.
  ///
  void Grow() noexcept
  {
    // TODO find better strategies?
    // Maybe specialize for sizeof(Type) & AllocatorType

    const uint32_t new_capacity = ((this->capacity_ << 1) | 0xF);

    Grow(new_capacity);
  }

  ///
  /// Makes sure the vector has enough available space to add an new element to it.
  ///
  void PrepareInsertion() noexcept
  {
    if (this->size_ == this->capacity_) [[unlikely]]
      Grow();
  }

  ///
  /// Destroys all the elements in the vector.
  ///
  /// @warning
  ///     Does not change size.
  ///
  void DestroyAll() noexcept
  {
    std::destroy(this->begin(), this->end());
  }

  ///
  /// Deallocates the current internal array.
  ///
  void Deallocate() noexcept
  {
    AllocatorType::Deallocate(Block { reinterpret_cast<char*>(this->array_), this->capacity_ });
    this->array_ = nullptr;
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
  void CopyFrom(const Vector<Type, OtherAllocator>& other) noexcept
  {
    const Block block = AllocatorType::Allocate(sizeof(Type) * other.size_);

    this->array_ = reinterpret_cast<Type*>(block.ptr);
    this->size_ = other.size_;
    this->capacity_ = static_cast<uint32_t>(block.size / sizeof(Type));

    std::uninitialized_copy(other.cbegin(), other.cend(), this->array_);
  }
};

} // namespace genebits::engine

#endif
