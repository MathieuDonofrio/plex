#ifndef GENEBITS_ENGINE_CONTAINERS_VECTOR_H
#define GENEBITS_ENGINE_CONTAINERS_VECTOR_H

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/os/memory.h"
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
  template<VectorType Type, std::unsigned_integral InternalSizeType>
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
      return static_cast<size_t>(size_);
    }

    ///
    /// Returns the current maximum amount of elements that can be stored
    /// in the vector without needing to grow the internal array.
    ///
    /// @return Capacity of the vector.
    ///
    [[nodiscard]] constexpr size_t capacity() const noexcept
    {
      return static_cast<size_t>(capacity_);
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
    /// Parametric constructor
    ///
    constexpr VectorBase(Type* array, uint32_t size, uint32_t capacity) noexcept
      : array_(array), size_(size), capacity_(capacity)
    {}

    ///
    /// Swaps contents of other vector base with this one.
    ///
    /// @param[in] other Other vector
    ///
    void Swap(VectorBase& other)
    {
      std::swap(array_, other.array_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }

  protected:
    Type* array_;
    InternalSizeType size_;
    InternalSizeType capacity_;
  };
} // namespace details

template<VectorType Type, typename AllocatorType = std::allocator<Type>>
class Vector : public details::VectorBase<Type, uint32_t>, private AllocatorType
{
private:
  using SuperClass = typename details::VectorBase<Type, uint32_t>;

public:
  using size_type = typename SuperClass::size_type;
  using difference_type = typename SuperClass::difference_type;
  using value_type = typename SuperClass::value_type;

  using iterator = typename SuperClass::iterator;
  using const_iterator = typename SuperClass::const_iterator;
  using const_reverse_iterator = typename SuperClass::const_reverse_iterator;
  using reverse_iterator = typename SuperClass::reverse_iterator;

  using reference = typename SuperClass::reference;
  using const_reference = typename SuperClass::const_reference;
  using pointer = typename SuperClass::pointer;
  using const_pointer = typename SuperClass::const_pointer;

  using allocator_type = AllocatorType;

public:
  ///
  /// Default constructor
  ///
  constexpr Vector() noexcept(std::is_nothrow_default_constructible_v<allocator_type>) : SuperClass(nullptr, 0, 0) {}

  ///
  /// Destructor
  ///
  ~Vector() noexcept
  {
    if (this->array_) [[likely]]
    {
      std::destroy(this->begin(), this->end());
      Deallocate();
    }
  }

  ///
  /// Move constructor
  ///
  /// @param[in] other Vector to move into this one.
  ///
  constexpr Vector(Vector<Type, AllocatorType>&& other) noexcept
    : SuperClass(other.array_, other.size_, other.capacity_)
  {
    other.array_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ///
  /// Copy constructor.
  ///
  /// @param[in] other Vector to copy from.
  ///
  Vector(const Vector<Type, AllocatorType>& other)
    : SuperClass(AllocatorType::allocate(other.size_), other.size_, other.size_)
  {
    if constexpr (std::is_trivially_copy_constructible_v<Type>)
    {
      std::memcpy(this->begin(), other.cbegin(), (char*)other.cend() - (char*)other.cbegin());
    }
    else
    {
      std::uninitialized_copy(other.cbegin(), other.cend(), this->begin());
    }
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
    Vector<Type, AllocatorType>(std::move(other)).Swap(*this);
    return *this;
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
    Vector<Type, AllocatorType>(other).Swap(*this);
    return *this;
  }

  ///
  /// Constructs the element in place at the back of the vector.
  ///
  /// @param[in] value Element to add and construct in place.
  ///
  template<typename... Args>
  void EmplaceBack(Args&&... args)
  {
    PrepareInsertion();
    new (this->end()) Type(std::forward<Args>(args)...);
    ++this->size_;
  }

  ///
  /// Push an element into to the back of the vector.
  ///
  /// @param[in] value Element to add
  ///
  void PushBack(const Type& value) noexcept
  {
    EmplaceBack(value);
  }

  ///
  /// Push an element into to the back of the vector.
  ///
  /// @param[in] value Element to add
  ///
  void PushBack(Type&& value) noexcept
  {
    EmplaceBack(std::move(value));
  }

  ///
  /// Remove the element at the back of the vector.
  ///
  void PopBack() noexcept
  {
    ASSERT(this->size_ > 0, "Vector is empty");

    --this->size_;
    if constexpr (!std::is_trivially_destructible_v<Type>) this->array_[this->size_].~Type();
  }

  ///
  /// Remove the element at the specified index. Uses swap and pop technique.
  ///
  /// @warning
  ///     This changes the order of elements by swapping the element at the end of
  ///     the vector with the one at the erased index.
  ///
  /// @param[in] value Iterator for element to erase
  ///
  void UnorderedErase(iterator it) noexcept
  {
    ASSERT(this->size_ > 0, "Vector is empty");

    if (--this->size_) [[likely]]
    {
      if constexpr (IsTriviallyRelocatable<Type>::value) std::memmove(it, this->end(), sizeof(Type));
      else
      {
        *it = std::move(*this->end());
      }
    }
    else
    {
      if constexpr (!std::is_trivially_destructible_v<Type>) it->~Type();
    }
  }

  ///
  /// Resizes the vector and default constructs all the new elements.
  ///
  /// Will grow the internal array is the new size if larger than the capacity.
  ///
  /// @param[in] new_size New size of the vector
  ///
  void Resize(const size_t new_size) noexcept
  {
    Reserve(new_size);

    if (static_cast<uint32_t>(new_size) > this->size_)
    {
      std::uninitialized_value_construct(this->end(), this->begin() + new_size);
    }
    else
    {
      std::destroy(this->begin() + new_size, this->end());
    }

    this->size_ = static_cast<uint32_t>(new_size);
  }

  ///
  /// Resizes the vector and copies the value into the new elements.
  ///
  /// Will grow the internal array is the new size if larger than the capacity.
  ///
  /// @param[in] new_size New size of the vector.
  /// @param[in] value The value to copy.
  ///
  void Resize(const size_t new_size, const Type& value) noexcept
  {
    Reserve(new_size);

    if (static_cast<uint32_t>(new_size) > this->size_)
    {
      std::uninitialized_fill(this->end(), this->begin() + new_size, value);
    }
    else
    {
      std::destroy(this->begin() + new_size, this->end());
    }

    this->size_ = static_cast<uint32_t>(new_size);
  }

  ///
  /// If necessary, increases the capacity of the vector to
  /// at least the specified amount.
  ///
  /// @param[in] min_capacity Minimum capacity the vector should have.
  ///
  void Reserve(const size_t min_capacity) noexcept
  {
    if (static_cast<uint32_t>(min_capacity) > this->capacity_) [[likely]]
      Grow(static_cast<uint32_t>(min_capacity));
  }

  ///
  /// Clears the vector of all its contents without deallocating the memory.
  ///
  /// Always O(n), where n is the size of the vector.
  ///
  void Clear() noexcept
  {
    std::destroy(this->begin(), this->end());

    this->size_ = 0;
  }

  ///
  /// Swaps this vector's contents with another vector.
  ///
  /// @param[in] other Other vector.
  ///
  void Swap(Vector<Type, AllocatorType>& other) noexcept
  {
    SuperClass::Swap(other);
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
    if (this->size_ != other.size_ || this->capacity_ != other.capacity_) return false;

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
    // Use allocate_at_least here for c++23.
    // It should yield major performance benefits and every container will use it.
    Type* new_array = AllocatorType::allocate(new_capacity);

    if (this->array_ == nullptr) goto Update;

    UninitializedRelocate<false>(this->begin(), this->end(), new_array);

    AllocatorType::deallocate(this->array_, this->capacity_);

Update:
    this->array_ = new_array;
    this->capacity_ = new_capacity;
  }

  ///
  /// Computes a good growth capacity.
  ///
  /// If the vector is empty, will return a fixed size based on size of Type, otherwise the growth rate is 1.5x.
  ///
  /// @return Computed capacity.
  ///
  [[nodiscard]] uint32_t ComputeGrowthCapacity() const noexcept
  {
    if (this->capacity_ == 0) [[unlikely]]
    {
      // First allocation is always at least 256 bytes and a capacity of 4
      return sizeof(Type) >= 64 ? 4 : 256 / sizeof(Type);
    }

    // Ideally we want to grow by 1.5x to maximize memory reallocation.

    return this->capacity_ + (this->capacity_ >> 1); // Same as 1.5x
  }

  ///
  /// Makes sure the vector has enough available space to add an new element to it.
  ///
  void PrepareInsertion() noexcept
  {
    if (this->size_ == this->capacity_) [[unlikely]]
    {
      // We may want to allow customization of growth policy in the future.
      // Depending on the allocator the default growth policy may not be ideal.

      Grow(ComputeGrowthCapacity());
    }
  }

  ///
  /// Deallocates the current internal array.
  ///
  void Deallocate() noexcept
  {
    AllocatorType::deallocate(this->array_, this->capacity_);
    this->array_ = nullptr;
  }
};

template<typename Type, typename Allocator>
struct IsTriviallyRelocatable<Vector<Type, Allocator>>
  : std::bool_constant<IsTriviallyRelocatable<Type>::value && IsTriviallyRelocatable<Allocator>::value>
{};

} // namespace genebits::engine

namespace std
{
template<typename T>
constexpr void swap(genebits::engine::Vector<T>& lhs, genebits::engine::Vector<T>& rhs) noexcept
{
  lhs.Swap(rhs);
}
} // namespace std

#endif
