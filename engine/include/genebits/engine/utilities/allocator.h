#ifndef GENEBITS_ENGINE_UTILITIES_ALLOCATOR_H
#define GENEBITS_ENGINE_UTILITIES_ALLOCATOR_H

#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <type_traits>

namespace genebits::engine
{
///
/// Represents a block of of memory. Contains a pointer to memory and a size.
///
struct Block
{
  char* ptr;
  size_t size;
};

///
/// Uses alignment of std::max_align_t to get max alignment.
/// Pointers returned by allocation functions such as std::malloc are suitably aligned for any object,
/// which means they are aligned at least as strictly as std::max_align_t.
///
constexpr size_t cMaxAlignment = alignof(std::max_align_t);

///
/// Rounds a size to the aligned size.
///
/// Can be used for compulsive alignment for allocators.
///
/// @param[in] size The size to round with alignment
///
/// @return Aligned size.
///
constexpr size_t RoundToAligned(size_t size)
{
  return (size + cMaxAlignment - 1) & ~(cMaxAlignment - 1);
}

///
/// Concept that is used to verify that the type meets all the requirements of an allocator.
///
/// The following methods are required:
///     - Block Allocate(size_t)
///     - void Deallocate(Block)
///     - bool Reallocate(Block&, size_t)
///     - bool Owns(Block)
///
/// Inspired by a talk from Andrei Alexandrescu: https://www.youtube.com/watch?v=LIb3L4vKZ7U
///
/// @tparam AllocatorImpl The type of the allocator to verify implementation.
///
template<typename AllocatorImpl>
concept Allocator = requires(AllocatorImpl allocator, Block block)
{
  // Style Exception: STL
  // clang-format off

  // Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  // was not successful.
  { allocator.Allocate(size_t {}) } -> std::convertible_to<Block>;

  // Deallocates a block of memory
  allocator.Deallocate(Block {});

  // Deallocates all memory if necessary
  allocator.DeallocateAll();

  // Reallocates a block of memory for the size. If allocation failed, does not change old block and
  // returns false.
  { allocator.Reallocate(block, size_t {}) } -> std::convertible_to<bool>;

  // Checks if the allocator has ownership of the block.
  { allocator.Owns(Block {}) } -> std::convertible_to<bool>;

  // clang-format on
};

///
/// Allocator adapter for compatibility with standard allocators.
///
/// Wrap the desired allocator to use the allocator in standard containers.
///
/// @note Use this only when you need to.
///
/// @tparam Type The data type to allocate
/// @tparam AllocatorImpl The allocator implementation to use.
///
template<typename Type, Allocator AllocatorImpl>
class AllocatorAdapter : public AllocatorImpl
{
public:
  ///
  /// Allocates an array of Type for the size.
  ///
  /// @note Actual byte size = size * sizeof(Type).
  ///
  /// @param[in] size The amount of Type to allocate.
  ///
  /// @return Pointer to allocated memory.
  ///
  [[nodiscard]] Type* allocate(size_t size) noexcept
  {
    return reinterpret_cast<Type*>(AllocatorImpl::Allocate(size * sizeof(Type)).ptr);
  }

  ///
  /// Deallocates memory that was previously allocated by this allocator.
  ///
  /// @param[in] ptr The pointer to the allocated memory.
  /// @param[in] size The size of the allocation.
  ///
  void deallocate(Type* ptr, size_t size) noexcept
  {
    AllocatorImpl::Deallocate(Block { reinterpret_cast<char*>(ptr), size * sizeof(Type) });
  }
};

///
/// Allocator based on std::malloc family.
///
class Mallocator
{
public:
  ///
  /// Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  /// was not successful.
  ///
  /// @see std::malloc
  ///
  /// @param size[in] Size in bytes to allocate.
  ///
  /// @return Allocated block. If failed, block memory pointer will be nullptr.
  ///
  [[nodiscard]] Block Allocate(const size_t size) const noexcept
  {
    return { static_cast<char*>(std::malloc(size)), size };
  }

  ///
  /// Deallocates a block of memory previously allocated by this allocator.
  ///
  /// @see std::free
  ///
  /// @param[in] block Allocated block to deallocate
  ///
  void Deallocate(const Block block) const noexcept
  {
    std::free(block.ptr);
  }

  ///
  /// Does nothing. Mallocator has not state.
  ///
  void DeallocateAll() const noexcept
  {
    // Does nothing
  }

  ///
  /// Reallocates a block of memory for the size. If allocation failed, does not change old block and
  /// returns false.
  ///
  /// @see std::realloc
  ///
  /// @param[in, out] block Allocated block to reallocate
  /// @param[in]      size Size to reallocate to.
  ///
  /// @return True if reallocation is successful, false otherwise.
  ///
  bool Reallocate(Block& block, const size_t size) const noexcept
  {
    block.ptr = static_cast<char*>(std::realloc(block.ptr, size));
    block.size = size;

    return block.ptr != nullptr;
  }

  ///
  /// Returns whether or not the allocator has ownership on this block of memory.
  ///
  /// @warning Always returns true, because it is impossible to know if malloc was used.
  ///
  /// @return True if allocator owns block, false otherwise.
  ///
  [[nodiscard]] constexpr bool Owns(const Block) const noexcept
  {
    return true;
  }
};

///
/// Allocator that pre-allocates a single contiguous block of memory on the stack.
///
/// Has a fixed amount of memory and leaks memory depending on allocation order. Good for temporary allocation, for
/// instance, allocator for a single frame.
///
/// Should be used in combination with a fallback allocator to avoid running out of memory.
///
/// Fastest allocation strategy.
///
/// @tparam Parent The parent allocator to allocate block of memory from.
/// @tparam Size The size in bytes of the memory.
///
template<size_t Size>
class StackAllocator
{
public:
  ///
  /// Default constructor.
  ///
  constexpr StackAllocator() : ptr_(data_) {};

  ///
  /// Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  /// was not successful.
  ///
  /// @param size[in] Size in bytes to allocate.
  ///
  /// @return Allocated block. If failed, block memory pointer will be nullptr.
  ///
  [[nodiscard]] constexpr Block Allocate(const size_t size) noexcept
  {
    const size_t aligned_size = RoundToAligned(size);

    if (aligned_size > static_cast<size_t>((data_ + Size) - ptr_)) { return { nullptr, 0u }; }

    Block block { ptr_, size };
    ptr_ += aligned_size;

    return block;
  }

  ///
  /// Conditionally deallocates a block of memory previously allocated by this allocator. The
  /// memory can be regained if the block was the last to be allocated. If not, that memory
  /// cannot be reused again.
  ///
  /// @warning Watch allocate/deallocate order to not leak stack memory.
  ///
  /// @param[in] block Allocated block to deallocate
  ///
  constexpr void Deallocate(const Block block) noexcept
  {
    if (LastAllocation(block)) { ptr_ = block.ptr; }
  }

  ///
  /// Free's all space at once.
  ///
  /// Cheap O(1) operation.
  ///
  void DeallocateAll() noexcept
  {
    ptr_ = data_;
  }

  ///
  /// Reallocates a block of memory for the size. If allocation failed, does not change old block and
  /// returns false.
  ///
  /// @note Same memory leak rules as Deallocate apply.
  ///
  /// @see StackAllocator::Deallocate
  ///
  /// @param[in, out] block   Allocated block to reallocate
  /// @param[in] size         Size to reallocate to.
  ///
  /// @return True if reallocation is successful, false otherwise.
  ///
  constexpr bool Reallocate(Block& block, const size_t size) noexcept
  {
    char* ptr = LastAllocation(block) ? block.ptr : ptr_;

    const size_t aligned_size = RoundToAligned(size);

    if (aligned_size > static_cast<size_t>((data_ + Size) - ptr)) { return false; }

    block.ptr = ptr;
    block.size = aligned_size;

    ptr_ = ptr + aligned_size;

    return true;
  }

  ///
  /// Returns whether or not the allocator has ownership on this block of memory.
  ///
  /// @return True if allocator owns block, false otherwise.
  ///
  [[nodiscard]] constexpr bool Owns(const Block block) const noexcept
  {
    return block.ptr >= data_ && block.ptr < data_ + Size;
  }

protected:
  ///
  /// Returns whether or not the block was the previous allocation of the allocator.
  ///
  /// @param block Block to check.
  ///
  /// @return True if the block was the last allocation of the allocator.
  ///
  [[nodiscard]] constexpr bool LastAllocation(const Block block) const noexcept
  {
    return block.ptr + RoundToAligned(block.size) == ptr_;
  }

protected:
  char* ptr_;
  std::conditional_t<Size == 0, char*, char[Size]> data_;
};

///
/// Allocator that pre-allocates a single contiguous block of memory.
///
/// Has a fixed amount of memory and leaks memory depending on allocation order. Good for temporary allocation, for
/// instance, allocator for a single frame.
///
/// Should be used in combination with a fallback allocator to avoid running out of memory.
///
/// Fastest allocation strategy.
///
/// @tparam Parent The parent allocator to allocate block of memory from.
/// @tparam Size The size in bytes of the memory.
///
template<Allocator Parent, size_t Size>
class LinearAllocator : public StackAllocator<0>, private Parent
{
public:
  ///
  /// Default constructor.
  ///
  LinearAllocator()
  {
    Block block = Parent::Allocate(Size);

    this->data_ = block.ptr;
  }

  ///
  /// Destructor.
  ///
  ~LinearAllocator()
  {
    Parent::Deallocate({ this->data_, Size });
  }
};

///
/// Allocator uses primary allocator but has a fallback allocation in case the
/// primary allocator fails.
///
/// @tparam Primary     Allocator to use first.
/// @tparam Fallback    Allocator to use when the primary allocator fails.
///
template<Allocator Primary, Allocator Fallback>
class FallbackAllocator : private Primary, private Fallback
{
public:
  ///
  /// Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  /// was not successful.
  ///
  /// If the primary allocator fails, fallback allocator will be used.
  ///
  /// @param size[in] Size in bytes to allocate.
  ///
  /// @return Allocated block. If failed, block memory pointer will be nullptr.
  ///
  [[nodiscard]] Block Allocate(const size_t size)
  {
    Block block = Primary::Allocate(size);

    if (!block.ptr) block = Fallback::Allocate(size);

    return block;
  }

  ///
  /// Deallocates a block of memory previously allocated by this allocator.
  ///
  /// Selects allocator to deallocate to based on ownership.
  ///
  /// @param[in] block Allocated block to deallocate
  ///
  void Deallocate(const Block block)
  {
    if (Primary::Owns(block)) Primary::Deallocate(block);
    else
    {
      Fallback::Deallocate(block);
    }
  }

  ///
  /// Delegates calls to primary and fallback allocators.
  ///
  void DeallocateAll()
  {
    Primary::DeallocateAll();
    Fallback::DeallocateAll();
  }

  ///
  /// Reallocates a block of memory for the size. If allocation failed, does not change old block and
  /// returns false.
  ///
  /// Selects allocator based on ownership, but if primary allocator is used and the allocation fails,
  /// a new block will be allocated using fallback allocator.
  ///
  /// @param[in, out] block Allocated block to reallocate
  /// @param[in]      size Size to reallocate to.
  ///
  /// @return True if reallocation is successful, false otherwise.
  ///
  bool Reallocate(Block& block, const size_t size)
  {
    if (Primary::Owns(block))
    {
      if (!Primary::Reallocate(block, size))
      {
        Block fallback_block = Fallback::Allocate(size);

        if (!fallback_block.ptr) return false;

        std::memcpy(fallback_block.ptr, block.ptr, block.size);

        Primary::Deallocate(block);

        block = fallback_block;
      }

      return true;
    }
    else
    {
      return Fallback::Reallocate(block, size);
    }
  }

  ///
  /// Returns whether or not the allocator has ownership on this block of memory. Checks
  /// if any of the two allocators owns this block.
  ///
  /// @return True if allocator owns block, false otherwise.
  ///
  [[nodiscard]] bool Owns(const Block block)
  {
    return Primary::Owns(block) || Fallback::Owns(block);
  }
};

///
/// Allocator that selects between a small allocator and large allocation depending
/// on a threshold.
///
/// @tparam Threshold       The threshold for allocator selection.
/// @tparam SmallAllocator  Allocator that is used for sizes below or equal to the threshold.
/// @tparam LargeAllocator  Allocator is used for sizes larger than the threshold.
///
template<size_t Threshold, Allocator SmallAllocator, Allocator LargeAllocator>
class Segregator : private SmallAllocator, private LargeAllocator
{
public:
  ///
  /// Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  /// was not successful.
  ///
  /// If the size is smaller or equal to the threshold, the small allocator will be used, otherwise
  /// the large allocator will be used.
  ///
  /// @param size[in] Size in bytes to allocate.
  ///
  /// @return Allocated block. If failed, block memory pointer will be nullptr.
  ///
  [[nodiscard]] Block Allocate(const size_t size)
  {
    if (size <= Threshold) return SmallAllocator::Allocate(size);

    return LargeAllocator::Allocate(size);
  }

  ///
  /// Deallocates a block of memory previously allocated by this allocator.
  ///
  /// If the block size is smaller than the threshold, the small allocator will be used, otherwise
  /// the large allocator will be used.
  ///
  /// @param[in] block Allocated block to deallocate
  ///
  void Deallocate(const Block block)
  {
    if (block.size <= Threshold) return SmallAllocator::Deallocate(block);

    return LargeAllocator::Deallocate(block);
  }

  ///
  /// Delegates calls to small and large allocators.
  ///
  void DeallocateAll()
  {
    SmallAllocator::DeallocateAll();
    LargeAllocator::DeallocateAll();
  }

  ///
  /// Reallocates a block of memory for the size. If allocation failed, does not change old block and
  /// returns false.
  ///
  /// If the block size is smaller than the threshold, the small allocator will be used, otherwise
  /// the large allocator will be used. However, if the block size is smaller than the threshold
  /// but the new size is bigger than the threshold, a new block will be allocated using the large
  /// allocator.
  ///
  /// @param[in, out] block Allocated block to reallocate
  /// @param[in]      size Size to reallocate to.
  ///
  /// @return True if reallocation is successful, false otherwise.
  ///
  bool Reallocate(Block& block, const size_t size)
  {
    if (block.size <= Threshold)
    {
      if (size <= Threshold) return SmallAllocator::Reallocate(block, size);

      Block large_block = LargeAllocator::Allocate(size);

      if (!large_block.ptr) return false;

      std::memcpy(large_block.ptr, block.ptr, block.size);

      SmallAllocator::Deallocate(block);

      block = large_block;

      return true;
    }

    return LargeAllocator::Reallocate(block, size);
  }

  ///
  /// Returns whether or not the allocator has ownership on this block of memory. Checks
  /// if the parent correct parent allocator owns this block.
  ///
  /// @return True if allocator owns block, false otherwise.
  ///
  [[nodiscard]] constexpr bool Owns(const Block block)
  {
    if (block.size <= Threshold) return SmallAllocator::Owns(block);
    else
    {
      return LargeAllocator::Owns(block);
    }
  }
};

///
/// Allocator that constructs a list of blocks of memory for reuse. If the block of memory does not fall
/// within the size range, this allocator will fallback to parent allocator.
///
/// @warning The first 8 bytes of objects are overwritten, be careful when using uninitialized memory.
///
/// @tparam Parent  The parent allocator
/// @tparam MinSize The minimum size of allocation for this allocator.
/// @tparam MaxSize The maximum size of allocation for this allocator.
///
template<Allocator Parent, size_t MinSize, size_t MaxSize>
requires(MaxSize != 0 && MinSize <= MaxSize) class Freelist : private Parent
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Freelist() : root_(nullptr) {};

  ~Freelist()
  {
    DeallocateAll();
  }

  ///
  /// Allocates and returns a block of memory for the size. Returns nullptr in block if allocation
  /// was not successful.
  ///
  /// If the size is smaller or equal to the threshold, the small allocator will be used, otherwise
  /// the large allocator will be used.
  ///
  /// @param size[in] Size in bytes to allocate.
  ///
  /// @return Allocated block. If failed, block memory pointer will be nullptr.
  ///
  [[nodiscard]] Block Allocate(const size_t size)
  {
    if (root_ && IsSizeTolerated(size)) return Obtain(size);

    return Parent::Allocate(size);
  }

  ///
  /// Deallocates a block of memory previously allocated by this allocator.
  ///
  /// If the block size is smaller than the threshold, the small allocator will be used, otherwise
  /// the large allocator will be used.
  ///
  /// @param[in] block Allocated block to deallocate
  ///
  void Deallocate(const Block block)
  {
    if (block.size == MaxSize) Recycle(block);
    else
    {
      Parent::Deallocate(block);
    }
  }

  ///
  /// Deallocates all blocks of memory stored in the freelist.
  ///
  void DeallocateAll()
  {
    while (root_)
    {
      Parent::Deallocate(Obtain(MaxSize));
    }
  }

  ///
  /// Reallocates a block of memory for the size. If allocation failed, does not change old block and
  /// returns false.
  ///
  /// If the size is tolerated will try to use a block from the freelist, otherwise will use the parent reallocate.
  ///
  /// @param[in, out] block Allocated block to reallocate
  /// @param[in]      size Size to reallocate to.
  ///
  /// @return True if reallocation is successful, false otherwise.
  ///
  bool Reallocate(Block& block, const size_t size)
  {
    if (root_ && IsSizeTolerated(size))
    {
      Block new_block = Obtain(size);

      const size_t copy_size = block.size < size ? block.size : size;

      std::memcpy(new_block.ptr, block.ptr, copy_size);

      Deallocate(block);

      block = new_block;

      return true;
    }

    return Parent::Reallocate(block, size);
  }

  ///
  /// Returns whether or not the allocator has ownership on this block of memory. Checks
  /// if any of the two allocators owns this block.
  ///
  /// @return True if allocator owns block, false otherwise.
  ///
  [[nodiscard]] bool Owns(const Block block)
  {
    return block.size == MaxSize || Parent::Owns(block);
  }

private:
  ///
  /// Returns whether or not the size is tolerated for this freelist.
  ///
  /// @param[in] size Size to check.
  ///
  /// @return True if size is tolerated, false otherwise.
  ///
  [[nodiscard]] constexpr bool IsSizeTolerated(const size_t size) noexcept
  {
    if constexpr (MinSize == MaxSize) return size == MaxSize;
    else
    {
      return size >= MinSize && size <= MaxSize;
    }
  }

  ///
  /// Pop's the first node in the freelist and creates a block.
  ///
  /// Freelist must not be empty and size must be within tolerance.
  ///
  /// @param[in] size Size of the block.
  ///
  /// @return Block with last node and specified size.
  ///
  [[nodiscard]] constexpr Block Obtain(const size_t size) noexcept
  {
    Block block { reinterpret_cast<char*>(root_), size };
    root_ = root_->next;
    return block;
  }

  ///
  /// Add the the block of memory to the beginning of the freelist.
  ///
  /// Block size must be same at MaxSize.
  ///
  /// @param[in] block Block to recyle.
  ///
  [[nodiscard]] constexpr void Recycle(const Block block) noexcept
  {
    auto node = reinterpret_cast<Node*>(block.ptr);
    node->next = root_;
    root_ = node;
  }

private:
  struct Node
  {
    Node* next;
  };

  Node* root_;
};
} // namespace genebits::engine

#endif