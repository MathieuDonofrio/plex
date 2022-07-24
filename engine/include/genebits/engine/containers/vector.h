#ifndef GENEBITS_ENGINE_CONTAINERS_VECTOR_H
#define GENEBITS_ENGINE_CONTAINERS_VECTOR_H

#include <iterator>
#include <memory_resource>
#include <ranges>
#include <utility>

#include "genebits/engine/containers/array.h"
#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/utilities/memory.h"
#include "genebits/engine/utilities/type_traits.h"

namespace plex
{
///
/// General purpose vector implementation optimized for speed.
///
/// This implementation of vector is our internal alternative for std::vector. Is most cases this vector is faster than
/// std::vector for the following reason:
///
/// - Optimizations for relocatable types replacing move and destroy loops for bitwise memory copy.
/// - 16 byte size.
/// - Other minor optimizations.
///
/// @tparam Type Value type to contain.
/// @tparam AllocatorType Allocator type to allocate memory with.
///
template<typename Type, typename AllocatorType = std::allocator<Type>>
class Vector : private AllocatorType
{
public:
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

  using allocator_type = AllocatorType;
  using allocator_traits = std::allocator_traits<allocator_type>;

  // Forward iterators
  [[nodiscard]] constexpr iterator begin() { return array_; }
  [[nodiscard]] constexpr const_iterator begin() const { return array_; }
  [[nodiscard]] constexpr iterator end() { return array_ + size_; }
  [[nodiscard]] constexpr const_iterator end() const { return array_ + size_; }

  // Explicit const forward iterators
  [[nodiscard]] constexpr const_iterator cbegin() const { return array_; }
  [[nodiscard]] constexpr const_iterator cend() const { return array_ + size_; }

  // Reverse iterators
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
  [[nodiscard]] constexpr const_reference operator[](const size_type index) const noexcept
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
  [[nodiscard]] constexpr reference operator[](const size_type index) noexcept
  {
    ASSERT(index < size_, "Index out of bounds");
    return array_[index];
  }

  ///
  /// Returns the amount of elements currently stored in the vector.
  ///
  /// @return Size of the vector.
  ///
  [[nodiscard]] constexpr size_type size() const noexcept
  {
    return static_cast<size_type>(size_);
  }

  ///
  /// Returns the current maximum amount of elements that can be stored
  /// in the vector without needing to grow the internal array.
  ///
  /// @return Capacity of the vector.
  ///
  [[nodiscard]] constexpr size_type capacity() const noexcept
  {
    return static_cast<size_type>(capacity_);
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

  ///
  /// Returns the allocator for this vector.
  ///
  /// @return Allocator.
  ///
  constexpr allocator_type get_allocator() const noexcept
  {
    return *this;
  }

public:
  ///
  /// Default constructor
  ///
  constexpr Vector() noexcept(std::is_nothrow_default_constructible_v<AllocatorType>)
    : array_(nullptr), size_(0), capacity_(0)
  {}

  ///
  /// Constructs vector using c-array style initializer list.
  ///
  /// @tparam Size CArray size.
  ///
  /// @param[in] source Array.
  ///
  template<size_t Size>
  Vector(CArray<Type, Size>&& source)
  {
    AssignToEmpty(std::move(source));
  }

  ///
  /// Empty c-array constructor. Default constructs the vector.
  ///
  template<std::same_as<EmptyCArray> Source = EmptyCArray>
  constexpr Vector(Source) noexcept(noexcept(Vector())) : Vector()
  {}

  ///
  /// Constructs vector using iterators.
  ///
  /// @tparam Iterator Iterator type.
  ///
  /// @param[in] first First iterator.
  /// @param[in] last Last iterator.
  ///
  template<std::input_iterator Iterator>
  Vector(Iterator first, Iterator last)
  {
    AssignToEmpty(first, last);
  }

  ///
  /// Constructs vector using a range
  ///
  /// @param source Range to copy from.
  ///
  template<std::ranges::input_range Range>
  Vector(const Range& source) : Vector(std::ranges::begin(source), std::ranges::end(source))
  {}

  ///
  /// Destructor
  ///
  ~Vector() noexcept
  {
    if (this->array_)
    {
      std::destroy(begin(), end());
      get_allocator().deallocate(array_, capacity_);
    }
  }

  ///
  /// Move constructor
  ///
  /// @param[in] other Vector to move into this one.
  ///
  constexpr Vector(Vector<Type, AllocatorType>&& other) noexcept
    : array_(other.array_), size_(other.size_), capacity_(other.capacity_)
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
  Vector(const Vector<Type, AllocatorType>& other) : size_(other.size_)
  {
    Block block = AllocateAtLeast(size_);

    array_ = block.ptr;
    capacity_ = block.count;

    std::uninitialized_copy(other.cbegin(), other.cend(), this->begin());
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
    Vector<Type, AllocatorType>(std::move(other)).swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Vector to copy from.
  ///
  /// @return Reference to this vector.
  ///
  Vector& operator=(const Vector<Type, AllocatorType>& other)
  {
    Vector<Type, AllocatorType>(other).swap(*this);
    return *this;
  }

  ///
  /// Constructs the element in place directly before the position.
  ///
  /// @param[in] pos Position to place element before.
  /// @param[in] value Element to add and construct in place.
  ///
  template<typename... Args>
  void emplace(iterator pos, Args&&... args)
  requires std::constructible_from<Type, Args...>
  {
    if (size_ < capacity_ && pos == end())
    {
      ConstructOneAtEnd(std::forward<Args>(args)...);
    }
    else // Slow path
    {
      SlowEmplace(pos, std::forward<Args>(args)...);
    }
  }

  ///
  /// Inserts the element directly before the position.
  ///
  /// @param[in] value Element to insert.
  ///
  void insert(iterator pos, const Type& value)
  requires std::constructible_from<Type, const Type&>
  {
    emplace(pos, value);
  }

  ///
  /// Inserts the element directly before the position.
  ///
  /// @param[in] value Element to insert.
  ///
  void insert(iterator pos, Type&& value)
  requires std::constructible_from<Type, Type&&>
  {
    emplace(pos, std::move(value));
  }

  ///
  /// Constructs the element in place at the back of the vector.
  ///
  /// @param[in] value Element to add and construct in place.
  ///
  template<typename... Args>
  void emplace_back(Args&&... args)
  requires std::constructible_from<Type, Args...>
  {
    if (size_ < capacity_) [[likely]]
    {
      ConstructOneAtEnd(std::forward<Args>(args)...);
    }
    else // Slow path
    {
      SlowEmplaceBack(std::forward<Args>(args)...);
    }
  }

  ///
  /// Push an element into to the back of the vector.
  ///
  /// @param[in] value Element to add
  ///
  void push_back(const Type& value)
  requires std::constructible_from<Type, const Type&>
  {
    emplace_back(value);
  }

  ///
  /// Push an element into to the back of the vector.
  ///
  /// @param[in] value Element to add
  ///
  void push_back(Type&& value)
  requires std::constructible_from<Type, Type&&>
  {
    emplace_back(std::move(value));
  }

  ///
  /// Remove the element at the back of the vector.
  ///
  void pop_back() noexcept
  {
    ASSERT(size_ > 0, "Vector is empty");

    --size_;
    end()->~Type();
  }

  ///
  /// Remove the element at the specified index. Shifts all elements after element.
  ///
  /// @param[in] it Iterator for element to erase
  ///
  void erase(iterator it)
  {
    ASSERT(size_ > 0, "Vector is empty");

    it->~Type();
    if (it + 1 != end()) UninitializedRelocate(it + 1, end(), it);
  }

  ///
  /// Remove the element at the specified index. Uses swap and pop technique.
  ///
  /// @warning
  ///     This changes the order of elements by swapping the element at the end of
  ///     the vector with the one at the erased index.
  ///
  /// @param[in] it Iterator for element to erase
  ///
  void SwapAndPop(iterator it)
  {
    ASSERT(size_ > 0, "Vector is empty");

    it->~Type();
    if (--size_) [[likely]]
    {
      RelocateAt(end(), it);
    }
  }

  ///
  /// Resizes the vector and default constructs all the new elements.
  ///
  /// @param[in] new_size New size of the vector
  ///
  void resize(const size_t new_size)
  requires std::default_initializable<Type>
  {
    reserve(new_size);

    if (static_cast<uint32_t>(new_size) > size_) std::uninitialized_value_construct(end(), begin() + new_size);
    else
      std::destroy(begin() + new_size, end());

    size_ = static_cast<uint32_t>(new_size);
  }

  ///
  /// Resizes the vector and copies the value into the new elements.
  ///
  /// @param[in] new_size New size of the vector.
  /// @param[in] value The value to copy.
  ///
  void resize(const size_t new_size, const Type& value)
  requires std::constructible_from<Type, const Type&>
  {
    reserve(new_size);

    if (static_cast<uint32_t>(new_size) > size_) std::uninitialized_fill(end(), begin() + new_size, value);
    else
      std::destroy(begin() + new_size, end());

    size_ = static_cast<uint32_t>(new_size);
  }

  ///
  /// If necessary, increases the capacity of the vector to
  /// at least the specified amount.
  ///
  /// @param[in] min_capacity Minimum capacity the vector should have.
  ///
  void reserve(const size_t min_capacity)
  {
    if (static_cast<uint32_t>(min_capacity) > capacity_) [[likely]]
    {
      const Block block = AllocateAtLeast(min_capacity);
      UninitializedRelocate(begin(), end(), block.ptr);

      SwapArrays(block.ptr, block.count);
    }
  }

  ///
  /// Clears the vector of all its contents without deallocating the memory.
  ///
  void clear() noexcept
  {
    std::destroy(begin(), end());

    this->size_ = 0;
  }

  ///
  /// Swaps this vector's contents with another vector.
  ///
  /// @param[in] other Other vector.
  ///
  void swap(Vector<Type, AllocatorType>& other) noexcept
  {
    std::swap(array_, other.array_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  ///
  /// Equality operator.
  ///
  /// @param[in] other Vector to compare.
  ///
  /// @return True if vectors are equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator==(const Vector<Type, AllocatorType>& other) const
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
  [[nodiscard]] constexpr bool operator!=(const Vector<Type, AllocatorType>& other) const
  {
    return !(*this == other);
  }

protected:
  ///
  /// Block returned by allocation. Returns a pointer and a count.
  ///
  struct Block
  {
    Type* ptr;
    uint32_t count;
  };

  ///
  /// Allocates a block of memory with least the required capacity.
  ///
  /// @param[in] capacity Minimum capacity.
  ///
  /// @return The allocated block.
  ///
  Block AllocateAtLeast(size_t capacity)
  {
    // C++ 23 allocate_at_least should yield major performance benefits over regular allocation for containers.
    // It is a suitable replacement for realloc.
    // Proposal: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0401r6.html
#ifdef __cpp_lib_allocate_at_least
    auto block = get_allocator().allocate_at_least(capacity);
    return { std::to_address(block.ptr), static_cast<uint32_t>(block.count) };
#else
    auto ptr = get_allocator().allocate(capacity);
    return { std::to_address(ptr), static_cast<uint32_t>(capacity) };
#endif
  }

  ///
  /// Computes a good growth capacity.
  ///
  /// If the vector is empty, will return a fixed size based on size of Type, otherwise the growth rate is 1.5x.
  ///
  /// @return Computed capacity.
  ///
  [[nodiscard]] constexpr uint32_t ComputeNextCapacity() const noexcept
  {
    if (capacity_ == 0) [[unlikely]]
    {
      // First allocation is always at least 256 bytes and a capacity of 4
      return sizeof(Type) >= 64 ? 4 : 256 / sizeof(Type);
    }

    // Ideally we want to grow by 1.5x to maximize memory reallocation.
    return capacity_ + (capacity_ / 2); // Same as 1.5x
  }

  ///
  /// Swaps the old array with the new array. Deallocates the old array if any.
  ///
  /// @param[in] new_array Array to swap to.
  /// @param[in] new_capacity New array capacity.
  ///
  void SwapArrays(Type* new_array, uint32_t new_capacity) noexcept
  {
    if (array_) get_allocator().deallocate(array_, capacity_);

    array_ = new_array;
    capacity_ = new_capacity;
  }

  ///
  /// Reallocates the array and constructs one element directly before the position.
  ///
  /// @tparam Args Argument types.
  ///
  /// @param[in] pos Position to emplace at.
  /// @param[in] args Arguments.
  ///
  template<typename... Args>
  void SlowEmplace(iterator pos, Args&&... args)
  {
    if (size_ < capacity_) [[likely]]
    {
      ShiftBackAt(pos);

      new (pos) Type(std::forward<Args>(args)...);
    }
    else
    {
      const Block block = AllocateAtLeast(ComputeNextCapacity());

      const size_t elements_before = static_cast<size_t>(pos - begin());
      const iterator new_pos = block.ptr + elements_before;

      new (new_pos) Type(std::forward<Args>(args)...);

      UninitializedRelocate(begin(), pos, block.ptr);
      UninitializedRelocate(pos, end(), new_pos + 1);

      SwapArrays(block.ptr, block.count);
    }

    ++size_;
  }

  ///
  /// Reallocates the array and constructs one element at the end of the vector.
  ///
  /// @tparam Args Argument types.
  ///
  /// @param[in] args Arguments.
  ///
  template<typename... Args>
  void SlowEmplaceBack(Args&&... args)
  {
    const Block block = AllocateAtLeast(ComputeNextCapacity());

    new (block.ptr + size_) Type(std::forward<Args>(args)...);
    UninitializedRelocate(begin(), end(), block.ptr);

    SwapArrays(block.ptr, block.count);

    ++size_;
  }

  ///
  /// Constructs one element at the end of the vector.
  ///
  /// @tparam Args Argument types.
  ///
  /// @param[in] args Arguments.
  ///
  template<typename... Args>
  void ConstructOneAtEnd(Args&&... args) noexcept(noexcept(Type(std::forward<Args>(args)...)))
  {
    new (end()) Type(std::forward<Args>(args)...);
    ++size_;
  }

  ///
  /// Moves all elements starting at the position one location back leaving the position uninitialized.
  ///
  void ShiftBackAt(iterator pos)
  {
    auto first = pos;
    auto last = end();
    auto dest = last + 1;

    if constexpr (IsTriviallyRelocatable<Type>::value)
    {
      const size_t bytes = reinterpret_cast<char*>(last) - reinterpret_cast<char*>(first);
      std::memmove(reinterpret_cast<char*>(dest) - bytes, first, bytes);
    }
    else
    {
      ::new (static_cast<void*>(--dest)) Type(std::move(*--last));
      std::move_backward(first, last, dest);
      first->~Type();
    }
  }

  ///
  /// Assigns a c-array to an empty vector.
  ///
  /// @tparam Size CArray size.
  ///
  /// @param[in] source CArray source.
  ///
  template<size_t Size>
  void AssignToEmpty(CArray<Type, Size>&& source)
  {
    Block block = AllocateAtLeast(Size);

    array_ = block.ptr;
    size_ = Size;
    capacity_ = block.count;

    UninitializedRelocate(source, static_cast<Type*>(source) + Size, array_);
  }

  ///
  /// Iterator contents to an empty vector.
  ///
  /// @param[in] first Iterator to first element.
  /// @param[in] last Iterator to last element.
  ///
  template<typename Iterator>
  void AssignToEmpty(Iterator first, Iterator last)
  {
    const auto size = static_cast<uint32_t>(std::distance(first, last));

    Block block = AllocateAtLeast(size);

    array_ = block.ptr;
    size_ = size;
    capacity_ = block.count;

    std::uninitialized_copy(first, last, array_);
  }

private:
  Type* array_;

  // We use 32 bit size and capacity to get a 16 byte vector on 64 bit.
  uint32_t size_;
  uint32_t capacity_;
};

template<std::input_iterator Iterator>
Vector(Iterator first, Iterator last) -> Vector<typename std::allocator_traits<Iterator>::value_type>;

template<typename Type, typename Allocator>
struct IsTriviallyRelocatable<Vector<Type, Allocator>>
  : public std::bool_constant<IsTriviallyRelocatable<Allocator>::value>
{};

namespace pmr
{
  template<typename Type>
  using Vector = Vector<Type, std::pmr::polymorphic_allocator<Type>>;
}
} // namespace plex

namespace std
{
template<typename T>
constexpr void swap(plex::Vector<T>& lhs, plex::Vector<T>& rhs) noexcept
{
  lhs.swap(rhs);
}
} // namespace std

#endif
