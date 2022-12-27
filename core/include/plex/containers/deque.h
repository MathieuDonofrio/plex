#ifndef PLEX_CONTAINERS_DEQUEUE_H
#define PLEX_CONTAINERS_DEQUEUE_H

#include <memory_resource>

#include "plex/containers/carray.h"
#include "plex/debug/assertion.h"
#include "plex/utilities/memory.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
namespace details
{
  template<typename Type, typename Ref, typename Ptr>
  class DequeIterator
  {
  private:
    using Self = DequeIterator;

  public:
    using iterator_category = std::random_access_iterator_tag;

    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using value_type = Type;

    using reference = Ref;
    using pointer = Ptr;

    constexpr DequeIterator() noexcept = default;

    constexpr DequeIterator(value_type* array, uint32_t index, uint32_t mask) noexcept
      : array_(array), index_(index), mask_(mask)
    {}

    DequeIterator(const DequeIterator& other) noexcept = default;
    DequeIterator& operator=(const DequeIterator&) noexcept = default;

    // clang-format off

    Self& operator+=(difference_type amount) noexcept
    { index_ = (index_ + static_cast<uint32_t>(amount)) & mask_; return *this; }
    Self& operator-=(difference_type amount) noexcept
    { index_ = (mask_ + 1 + index_ - static_cast<uint32_t>(amount)) & mask_; return *this; }

    Self& operator++() noexcept { return index_ = (index_ + 1) & mask_, *this; }
    Self& operator--() noexcept { return index_ = (index_ + mask_) & mask_, *this; }

    Self operator++(int) noexcept { Self copy(*this); operator++(); return copy; }
    Self operator--(int) noexcept { Self copy(*this); operator--(); return copy; }

    [[nodiscard]] friend Self operator+(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp += amount; return tmp; }
    [[nodiscard]] friend Self operator-(const Self& it, difference_type amount) noexcept
    { Self tmp = it; tmp -= amount; return tmp; }
    [[nodiscard]] friend Self operator+(difference_type amount, const Self& it) noexcept
    { return it + amount; }

    template<typename OtherRef, typename OtherPtr>
    [[nodiscard]] friend difference_type
    operator-(const Self& lhs, const DequeIterator<Type, OtherRef, OtherPtr>& rhs) noexcept
    {
      ASSERT(lhs.array_ == rhs.array_, "Summing dequeue iterators from different dequeues");
      if (rhs.index_ >= lhs.index_) return rhs.index_ - lhs.index_;
      return lhs.index_ - rhs.index_;
    }

    [[nodiscard]] constexpr reference operator*() const noexcept { return array_[index_]; }
    [[nodiscard]] constexpr pointer operator->() const noexcept { return array_ + index_; }

    [[nodiscard]] constexpr reference operator[](difference_type index) const noexcept
    { return array_[(index_ + index) & mask_]; }

    [[nodiscard]] friend bool operator==(const DequeIterator& lhs, const DequeIterator& rhs) noexcept
    {
      ASSERT(lhs.array_ == rhs.array_, "Comparing dequeue iterators from different dequeues");
      return lhs.index_ == rhs.index_;
    }

    [[nodiscard]] friend bool operator!=(const DequeIterator& lhs, const DequeIterator& rhs) noexcept
    { return !(lhs == rhs); }

    [[nodiscard]]
    friend std::strong_ordering operator<=>(const Self& lhs, const Self& rhs) noexcept
    {
      ASSERT(lhs.array_ == rhs.array_, "Comparing dequeue iterators from different dequeues");
      return lhs.index_ <=> rhs.index_;
    }

    // clang-format on

  private:
    value_type* array_;
    uint32_t index_;
    uint32_t mask_;
  };

} // namespace details

///
/// General purpose deque implementation optimized for speed.
///
/// This implementation of deque is our internal alternative for std::deque. This deque is faster than std::deque for
/// the following reasons:
///
/// - Optimizations for relocatable types replacing move and destroy loops for bitwise memory copy.
/// - 16 byte size.
/// - Uses a single array, better cache locality and less indirections.
/// - Exponential growth rate.
/// - Other minor optimizations.
///
/// @warning This deque does not have pointer stability like std::deque, iterators can be invalidated if the container
/// is mutated.
///
/// @tparam Type Value type to contain.
/// @tparam AllocatorType Allocator type to allocate memory with.
///
template<typename Type, typename AllocatorType = std::allocator<Type>>
class Deque : private AllocatorType
{
public:
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = Type;

  using iterator = details::DequeIterator<Type, Type&, Type*>;
  using const_iterator = details::DequeIterator<Type, const Type&, const Type*>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using reference = Type&;
  using const_reference = const Type&;
  using pointer = Type*;
  using const_pointer = const Type*;

  using allocator_type = AllocatorType;
  using allocator_traits = std::allocator_traits<allocator_type>;

  // clang-format off

  // Forward iterators
  [[nodiscard]] constexpr iterator begin() { return { array_, front_, mask_ }; }
  [[nodiscard]] constexpr const_iterator begin() const { return {array_, front_, mask_}; }
  [[nodiscard]] constexpr iterator end() { return { array_, rear_, mask_ }; }
  [[nodiscard]] constexpr const_iterator end() const { return { array_, rear_, mask_ }; }

  // Explicit const forward iterators
  [[nodiscard]] constexpr const_iterator cbegin() const { return  { array_, front_, mask_ }; }
  [[nodiscard]] constexpr const_iterator cend() const { return { array_, rear_, mask_ }; }

  // Reverse iterators
  [[nodiscard]] constexpr reverse_iterator rbegin() { return reverse_iterator(end()); }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  [[nodiscard]] constexpr reverse_iterator rend() { return reverse_iterator(begin()); }
  [[nodiscard]] constexpr const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Internal array accessors
  [[nodiscard]] constexpr reference front() { return array_[front_]; }
  [[nodiscard]] constexpr const_reference front() const { return array_[front_]; }
  [[nodiscard]] constexpr reference back() { return array_[(rear_ + mask_) & mask_]; }
  [[nodiscard]] constexpr const_reference back() const { return array_[(rear_ + mask_) & mask_]; }

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
    ASSERT(index < size(), "Index out of bounds");
    return array_[(index + front_) & mask_];
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
    ASSERT(index < size(), "Index out of bounds");
    return array_[(index + front_) & mask_];
  }

  ///
  /// Returns the amount of elements currently stored in the vector.
  ///
  /// @return Size of the vector.
  ///
  [[nodiscard]] constexpr size_type size() const noexcept
  {
    if (rear_ >= front_)
    {
      return rear_ - front_;
    }

    return static_cast<size_type>(MaskToCapacity(mask_) - front_ + rear_);
  }

  ///
  /// Returns the current maximum amount of elements that can be stored
  /// in the dequeue without needing to grow the internal array.
  ///
  /// @return Capacity of the vector.
  ///
  [[nodiscard]] constexpr size_type capacity() const noexcept
  {
    if (array_)
    {
      return static_cast<size_type>(MaskToCapacity(mask_));
    }

    return 0;
  }

  ///
  /// Returns whether or not the vector is empty.
  ///
  /// @return True if the vector is empty, false otherwise.
  ///
  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return front_ == rear_;
  }

  ///
  /// Returns the allocator for this dequeue.
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
  constexpr Deque() noexcept(std::is_nothrow_default_constructible_v<AllocatorType>)
    : array_(nullptr), front_(0), rear_(0), mask_(0)
  {}

  ///
  /// Constructs deque using c-array style initializer list.
  ///
  /// @tparam Size CArray size.
  ///
  /// @param[in] source Array.
  ///
  template<size_t Size>
  Deque(CArray<Type, Size>&& source)
  {
    AssignToEmpty(std::move(source));
  }

  ///
  /// Empty c-array constructor. Default constructs the deque.
  ///
  template<std::same_as<EmptyCArray> Source = EmptyCArray>
  constexpr Deque(Source) noexcept(noexcept(Deque())) : Deque()
  {}

  ///
  /// Constructs deque using iterators.
  ///
  /// @tparam Iterator Iterator type.
  ///
  /// @param[in] first First iterator.
  /// @param[in] last Last iterator.
  ///
  template<std::input_iterator Iterator>
  Deque(Iterator first, Iterator last)
  {
    AssignToEmpty(first, last);
  }

  ///
  /// Constructs vector using a range
  ///
  /// @param source Range to copy from.
  ///
  template<std::ranges::input_range Range>
  Deque(const Range& source) : Deque(std::ranges::begin(source), std::ranges::end(source))
  {}

  ///
  /// Destructor
  ///
  ~Deque() noexcept
  {
    if (array_)
    {
      DestroyAll();
      get_allocator().deallocate(array_, capacity());
    }
  }

  ///
  /// Move constructor
  ///
  /// @param[in] other Vector to move into this one.
  ///
  constexpr Deque(Deque<Type, AllocatorType>&& other) noexcept
    : array_(other.array_), front_(other.front_), rear_(other.rear_), mask_(other.mask_)
  {
    other.array_ = nullptr;
    other.front_ = 0;
    other.rear_ = 0;
    other.mask_ = 0;
  }

  ///
  /// Copy constructor.
  ///
  /// @param[in] other Vector to copy from.
  ///
  Deque(const Deque<Type, AllocatorType>& other) : front_(0), rear_(static_cast<uint32_t>(other.size()))
  {
    if (rear_ > 0)
    {
      const auto capacity = ComputeCapacity(rear_);

      mask_ = CapacityToMask(capacity);
      array_ = Allocate(capacity);

      other.CopyTo(array_);
    }
    else
    {
      array_ = nullptr;
      mask_ = 0;
    }
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Dequeue to move into this one.
  ///
  /// @return Reference to this deque.
  ///
  constexpr Deque& operator=(Deque<Type, AllocatorType>&& other) noexcept
  {
    Deque<Type, AllocatorType>(std::move(other)).swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Dequeue to copy from.
  ///
  /// @return Reference to this deque.
  ///
  Deque& operator=(const Deque<Type, AllocatorType>& other)
  {
    Deque<Type, AllocatorType>(other).swap(*this);
    return *this;
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
    const auto next_rear = (rear_ + 1) & mask_;

    if (next_rear != front_) [[likely]]
    {
      new (array_ + rear_) Type(std::forward<Args>(args)...);
      rear_ = next_rear;
    }
    else // Slow path
    {
      SlowEmplaceBack(std::forward<Args>(args)...);
    }
  }

  ///
  /// Constructs the element in place at the front of the vector.
  ///
  /// @param[in] value Element to add and construct in place.
  ///
  template<typename... Args>
  void emplace_front(Args&&... args)
  requires std::constructible_from<Type, Args...>
  {
    const auto next_front = (front_ + mask_) & mask_;

    if (next_front != rear_) [[likely]]
    {
      front_ = next_front;
      new (array_ + next_front) Type(std::forward<Args>(args)...);
    }
    else // Slow path
    {
      SlowEmplaceFront(std::forward<Args>(args)...);
    }
  }

  ///
  /// Push an element into to the back of the deque.
  ///
  /// @param[in] value Element to add
  ///
  void push_back(const Type& value)
  requires std::constructible_from<Type, const Type&>
  {
    emplace_back(value);
  }

  ///
  /// Push an element into to the back of the deque.
  ///
  /// @param[in] value Element to add
  ///
  void push_back(Type&& value)
  requires std::constructible_from<Type, Type&&>
  {
    emplace_back(std::move(value));
  }

  ///
  /// Push an element into to the front of the deque.
  ///
  /// @param[in] value Element to add
  ///
  void push_front(const Type& value)
  requires std::constructible_from<Type, const Type&>
  {
    emplace_front(value);
  }

  ///
  /// Push an element into to the front of the deque.
  ///
  /// @param[in] value Element to add
  ///
  void push_front(Type&& value)
  requires std::constructible_from<Type, Type&&>
  {
    emplace_front(std::move(value));
  }

  ///
  /// Remove the element at the back of the deque.
  ///
  void pop_back() noexcept
  {
    ASSERT(!empty(), "Dequeue is empty");

    rear_ = (rear_ + mask_) & mask_;
    (array_ + rear_)->~Type();
  }

  ///
  /// Remove the element at the front of the deque.
  ///
  void pop_front() noexcept
  {
    ASSERT(!empty(), "Dequeue is empty");

    (array_ + front_)->~Type();
    front_ = (front_ + 1) & mask_;
  }

  ///
  /// If necessary, increases the capacity of the deque to at least the specified amount.
  ///
  /// If a reallocation is triggered, the front of the deque is placed at the beginning of the array.
  ///
  /// @param[in] min_capacity Minimum capacity the vector should have.
  ///
  void reserve(const size_t min_capacity)
  {
    if (min_capacity > capacity()) [[likely]]
    {
      const uint32_t new_capacity = ComputeCapacity(static_cast<uint32_t>(min_capacity));
      Type* new_array = Allocate(new_capacity);

      RelocateTo(new_array);

      SwapArrays(new_array, new_capacity);
    }
  }

  ///
  /// Clears the deque of all its contents without deallocating the memory.
  ///
  void clear() noexcept
  {
    DestroyAll();

    front_ = rear_ = 0;
  }

  ///
  /// Swaps this deque's contents with another deque.
  ///
  /// @param[in] other Other vector.
  ///
  void swap(Deque<Type, AllocatorType>& other) noexcept
  {
    std::swap(array_, other.array_);
    std::swap(front_, other.front_);
    std::swap(rear_, other.rear_);
    std::swap(mask_, other.mask_);
  }

  ///
  /// Equality operator.
  ///
  /// @param[in] other Dequeue to compare.
  ///
  /// @return True if deque are equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator==(const Deque<Type, AllocatorType>& other) const
  {
    if (this->array_ == other.array_) return true; // Checks for same instance or two empty deques.

    if (size() != other.size()) return false;

    if (front_ <= rear_ && other.front_ <= other.rear_)
    {
      return std::equal(array_ + front_, array_ + rear_, other.array_ + other.front_);
    }
    else
    {
      return std::equal(begin(), end(), other.begin());
    }
  }

  ///
  /// Inequality operator.
  ///
  /// @param[in] other Dequeue to compare.
  ///
  /// @return True if deque are not equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator!=(const Deque<Type, AllocatorType>& other) const
  {
    return !(*this == other);
  }

private:
  ///
  /// Allocates a block of memory with exactly the given capacity
  ///
  /// @param[in] capacity The requested capacity.
  ///
  /// @return Pointer to the allocated block of memory
  ///
  Type* Allocate(size_t capacity)
  {
    return std::to_address(get_allocator().allocate(capacity));
  }

  ///
  /// Computes the next capacity.
  ///
  /// First allocation capacity is constant based on size of type. All other allocations scale the capacity by a
  /// approximately a factor of two.
  ///
  /// @return Computed capacity.
  ///
  [[nodiscard]] constexpr uint32_t ComputeNextCapacity() const noexcept
  {
    if (mask_ == 0) [[unlikely]]
    {
      // First allocation is always at least 256 bytes and a capacity of 4
      return ComputeCapacity(sizeof(Type) >= 64 ? 4 : 256 / sizeof(Type));
    }

    return ComputeCapacity(MaskToCapacity(mask_) << 1); // Double capacity
  }

  ///
  /// Computes the capacity at least as big as the given capacity.
  ///
  /// Capacity is always a power of two. This allows us to use fast bitwise operations.
  ///
  /// @return Computed capacity.
  ///
  [[nodiscard]] static constexpr uint32_t ComputeCapacity(uint32_t min_capacity) noexcept
  {
    ASSERT(min_capacity > 0, "Capacity should never be zero.");

    return std::bit_ceil(min_capacity + 1);
  }

  ///
  /// Returns the mask obtained from the capacity.
  ///
  /// @param[in] capacity The capacity.
  ///
  /// @return The mask.
  ///
  [[nodiscard]] static constexpr uint32_t CapacityToMask(uint32_t capacity) noexcept
  {
    ASSERT(std::has_single_bit(capacity), "Capacity must be a power of two");

    return capacity - 1;
  }

  ///
  /// Returns the capacity obtained from the mask.
  ///
  /// @param[in] mask The mask;
  ///
  /// @return The capacity.
  ///
  [[nodiscard]] static constexpr uint32_t MaskToCapacity(uint32_t mask) noexcept
  {
    return mask + 1;
  }

  ///
  /// Swaps the old array with the new array. Deallocates the old array if any.
  ///
  /// @param[in] new_array Array to swap to.
  /// @param[in] new_capacity New array capacity.
  ///
  void SwapArrays(Type* new_array, uint32_t new_capacity) noexcept
  {
    if (array_) get_allocator().deallocate(array_, MaskToCapacity(mask_));

    array_ = new_array;
    mask_ = CapacityToMask(new_capacity);
  }

  ///
  /// Destroys all elements in the deque.
  ///
  void DestroyAll()
  {
    if (front_ <= rear_)
    {
      std::destroy(array_ + front_, array_ + rear_);
    }
    else
    {
      std::destroy(array_ + front_, array_ + MaskToCapacity(mask_));
      std::destroy(array_, array_ + rear_);
    }
  }

  ///
  /// Relocates the current array onto the given array.
  ///
  /// The front is always relocated at the beginning of the array.
  ///
  /// @param[in] array The array to relocate to.
  ///
  /// @return The amount of elements relocated.
  ///
  uint32_t RelocateTo(Type* array)
  {
    if (front_ <= rear_)
    {
      UninitializedRelocate(array_ + front_, array_ + rear_, array);

      return rear_ - front_;
    }
    else
    {
      const auto capacity = MaskToCapacity(mask_);

      auto last = UninitializedRelocate(array_ + front_, array_ + capacity, array);
      UninitializedRelocate(array_, array_ + rear_, last);

      return (capacity - front_) + rear_;
    }
  }

  ///
  /// Copies the current array onto the given array.
  ///
  /// The front is always relocated at the beginning of the array.
  ///
  /// @param[in] array The array to copy to.
  ///
  void CopyTo(Type* array) const
  {
    if (front_ <= rear_)
    {
      std::uninitialized_copy(array_ + front_, array_ + rear_, array);
    }
    else
    {
      auto last = std::uninitialized_copy(array_ + front_, array_ + MaskToCapacity(mask_), array);
      std::uninitialized_copy(array_, array_ + rear_, last);
    }
  }

  ///
  /// Reallocates the array and constructs one element at the front of the deque.
  ///
  /// @tparam Args Argument types.
  ///
  /// @param[in] args Arguments.
  ///
  template<typename... Args>
  NO_INLINE void SlowEmplaceFront(Args&&... args)
  {
    const uint32_t new_capacity = ComputeNextCapacity();
    Type* new_array = Allocate(new_capacity);

    new (new_array) Type(std::forward<Args>(args)...);

    const uint32_t size = RelocateTo(new_array + 1);

    rear_ = size + 1;
    front_ = 0;

    SwapArrays(new_array, new_capacity);
  }

  ///
  /// Reallocates the array and constructs one element at the end of the deque.
  ///
  /// @tparam Args Argument types.
  ///
  /// @param[in] args Arguments.
  ///
  template<typename... Args>
  NO_INLINE void SlowEmplaceBack(Args&&... args)
  {
    const uint32_t new_capacity = ComputeNextCapacity();
    Type* new_array = Allocate(new_capacity);

    const uint32_t size = RelocateTo(new_array);

    new (new_array + size) Type(std::forward<Args>(args)...);

    rear_ = size + 1;
    front_ = 0;

    SwapArrays(new_array, new_capacity);
  }

  ///
  /// Assigns a c-array to an empty deque.
  ///
  /// @tparam Size CArray size.
  ///
  /// @param[in] source CArray source.
  ///
  template<size_t Size>
  void AssignToEmpty(CArray<Type, Size>&& source)
  {
    const auto capacity = ComputeCapacity(Size);

    mask_ = CapacityToMask(capacity);
    array_ = Allocate(capacity);
    front_ = 0;
    rear_ = Size;

    UninitializedRelocate(static_cast<Type*>(source), static_cast<Type*>(source) + Size, array_);
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
    const auto capacity = ComputeCapacity(size);

    mask_ = CapacityToMask(capacity);
    array_ = Allocate(capacity);
    front_ = 0;
    rear_ = size;

    std::uninitialized_copy(first, last, array_);
  }

private:
  Type* array_;
  uint32_t front_;
  uint32_t rear_;
  uint32_t mask_;
};

template<std::input_iterator Iterator>
Deque(Iterator first, Iterator last) -> Deque<typename std::allocator_traits<Iterator>::value_type>;

template<typename Type, typename Allocator>
struct IsTriviallyRelocatable<Deque<Type, Allocator>>
  : public std::bool_constant<IsTriviallyRelocatable<Allocator>::value>
{};

namespace pmr
{
  template<typename Type>
  using Dequeue = Deque<Type, std::pmr::polymorphic_allocator<Type>>;
}
} // namespace plex

namespace std
{
template<typename T>
constexpr void swap(plex::Deque<T>& lhs, plex::Deque<T>& rhs) noexcept
{
  lhs.swap(rhs);
}
} // namespace std

#endif
