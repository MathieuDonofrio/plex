#include "genebits/engine/os/allocator.h"

#include <map>
#include <vector>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(
  sizeof(Block) == sizeof(char*) + sizeof(size_t), "The  size of a block cannot excede that of a char* and size_t");

static_assert(cMaxAlignment > 1, "Max alignment should be bigger than 1");

static_assert((cMaxAlignment & (cMaxAlignment - 1)) == 0, "Max alignment should be a power of two");

template<typename Type>
using MallocatorVector = std::vector<Type, AllocatorAdapter<Type, Mallocator>>;

// Static assert to check if the MallocatorVector compiles
static_assert(std::is_same_v<MallocatorVector<int>, MallocatorVector<int>>);

// These maps are used in the mock allocator
static std::map<size_t, bool> allocate_call;
static std::map<size_t, bool> deallocate_call;
static std::map<size_t, bool> deallocateall_call;
static std::map<size_t, bool> reallocate_call;
static std::map<size_t, bool> owns_call;

template<size_t Id = 0>
class MockAllocator
{
public:
  Block Allocate(size_t size)
  {
    allocate_call[Id] = true;

    return Block { static_cast<char*>(std::malloc(size)), size };
  }

  void Deallocate(Block block)
  {
    deallocate_call[Id] = true;

    std::free(block.ptr);
  }

  void DeallocateAll() const
  {
    deallocateall_call[Id] = true;
  }

  bool Reallocate(Block& block, size_t size)
  {
    reallocate_call[Id] = true;

    block.ptr = static_cast<char*>(std::realloc(block.ptr, size));
    block.size = size;

    return true;
  }

  bool Owns(Block)
  {
    owns_call[Id] = true;

    return true;
  }
};

TEST(Allocator_Tests, RoundToAlign_Extremities_AlreadyAligned)
{
  EXPECT_EQ(RoundToAligned(0), 0u);
  EXPECT_EQ(RoundToAligned(cMaxAlignment), cMaxAlignment);
}

TEST(Allocator_Tests, RoundToAlign_LargeExtremities_AlreadyAligned)
{
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 10), cMaxAlignment * 10);
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 1000), cMaxAlignment * 1000);
}

TEST(Allocator_Tests, RoundToAlign_AllValuesInBetween_NotAligned)
{
  for (size_t i = 1; i <= cMaxAlignment; i++)
  {
    EXPECT_EQ(RoundToAligned(i), cMaxAlignment);
  }

  for (size_t i = cMaxAlignment + 1; i <= cMaxAlignment * 2; i++)
  {
    EXPECT_EQ(RoundToAligned(i), cMaxAlignment * 2);
  }
}

TEST(Allocator_Tests, RoundToAlign_LargeNumberInBetween_NotAligned)
{
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 10 + 1), cMaxAlignment * 11);
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 10 + cMaxAlignment - 1), cMaxAlignment * 11);
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 1000 + 1), cMaxAlignment * 1001);
  EXPECT_EQ(RoundToAligned(cMaxAlignment * 1000 + cMaxAlignment - 1), cMaxAlignment * 1001);
}

TEST(Mallocator_Tests, Allocate_MultipleAllocations_AlwaysValid)
{
  Mallocator allocator;

  Block block1 = allocator.Allocate(1000);

  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, 1000u);

  Block block2 = allocator.Allocate(2000);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, 2000u);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Mallocator_Tests, Reallocate_ValidBlock_Valid)
{
  Mallocator allocator;

  Block block = allocator.Allocate(1000);

  bool result = allocator.Reallocate(block, 2000);

  EXPECT_TRUE(result);

  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, 2000u);

  allocator.Deallocate(block);
}

TEST(Mallocator_Tests, Owns_AllocatedBlock_True)
{
  Mallocator allocator;

  Block block = allocator.Allocate(1000);

  EXPECT_TRUE(allocator.Owns(block));

  allocator.Deallocate(block);
}

TEST(StackAllocator_Tests, Allocate_AllocationUseAllSpace_Valid)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size);

  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size);
}

TEST(StackAllocator_Tests, Allocate_DoubleAllocationUseSpace_Valid)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block1 = allocator.Allocate(Size / 2);

  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, Size / 2);

  Block block2 = allocator.Allocate(Size / 2);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, Size / 2);
}

TEST(StackAllocator_Tests, Allocate_ToBig_InsufficientSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size + 1);

  EXPECT_EQ(block.ptr, nullptr);
  EXPECT_EQ(block.size, 0u);
}

TEST(StackAllocator_Tests, Allocate_AllocationUseSomeSpace_Aligned)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block1 = allocator.Allocate(Size / 2 + 1);

  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, Size / 2 + 1);

  Block block2 = allocator.Allocate(Size / 2 - 1);

  EXPECT_EQ(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, 0);
}

TEST(StackAllocator_Tests, Deallocate_LastAllocation_RegainSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size);

  allocator.Deallocate(block);

  block.ptr = nullptr;
  block.size = 0;

  block = allocator.Allocate(Size);

  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size);
}

TEST(StackAllocator_Tests, Deallocate_NotLastAllocation_DontRegainSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block1 = allocator.Allocate(Size / 2);
  Block block2 = allocator.Allocate(Size / 2);

  (void)block2; // Suppress warning

  allocator.Deallocate(block1);

  Block block3 = allocator.Allocate(Size / 2);

  EXPECT_EQ(block3.ptr, nullptr);
  EXPECT_EQ(block3.size, 0u);
}

TEST(StackAllocator_Tests, DeallocateAll_UsedMemory_RegainSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size);

  allocator.DeallocateAll();

  block = allocator.Allocate(Size);

  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size);
}

TEST(StackAllocator_Tests, Reallocate_LastAllocation_UseAllSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size / 2);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, Size);

  EXPECT_TRUE(result);
  EXPECT_EQ(block.ptr, old_ptr);
  EXPECT_EQ(block.size, Size);
}

TEST(StackAllocator_Tests, Reallocate_LastAllocationButToBig_InsufficientSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block = allocator.Allocate(Size / 2);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, Size + 1);

  EXPECT_FALSE(result);
  EXPECT_EQ(block.ptr, old_ptr);
  EXPECT_EQ(block.size, Size / 2);
}

TEST(StackAllocator_Tests, Reallocate_SufficientSpaceButNotLast_InsufficientSpace)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block1 = allocator.Allocate(Size / 4);
  Block block2 = allocator.Allocate(Size / 2);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, Size / 2);

  char* old_ptr = block1.ptr;

  bool result = allocator.Reallocate(block1, Size / 2 + 1);

  EXPECT_FALSE(result);
  EXPECT_EQ(block1.ptr, old_ptr);
  EXPECT_EQ(block1.size, Size / 4);

  Block block3 = allocator.Allocate(Size / 4);

  EXPECT_NE(block3.ptr, nullptr);
  EXPECT_EQ(block3.size, Size / 4);
}

TEST(StackAllocator_Tests, Owns_AllocatedBlock_Valid)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  Block block1 = allocator.Allocate(Size / 2);
  Block block2 = allocator.Allocate(Size / 2);

  EXPECT_TRUE(allocator.Owns(block1));
  EXPECT_TRUE(allocator.Owns(block2));
}

TEST(StackAllocator_Tests, Owns_NotFromAllocator_NotValid)
{
  constexpr size_t Size = cMaxAlignment * 100;

  StackAllocator<Size> allocator;

  char c = 'c';

  Block block { &c, 16 };

  EXPECT_FALSE(allocator.Owns(block));
}

TEST(LinearAllocator_Tests, Constructor_NoMemory_CallsParent)
{
  constexpr size_t Size = cMaxAlignment * 100;

  allocate_call[1] = false;

  LinearAllocator<MockAllocator<1>, Size> allocator;

  EXPECT_TRUE(allocate_call[1]);
}

TEST(LinearAllocator_Tests, Destructor_AllocatedMemory_CallsParent)
{
  constexpr size_t Size = cMaxAlignment * 100;

  deallocate_call[1] = false;

  {
    LinearAllocator<MockAllocator<1>, Size> allocator;
  }

  EXPECT_TRUE(deallocate_call[1]);
}

TEST(FallbackAllocator_Tests, Allocate_DoesNotFail_PrimaryCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(Size);

  (void)block; // Suppress warning

  EXPECT_FALSE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(FallbackAllocator_Tests, Allocate_Fails_FallbackCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(Size + 1);

  (void)block; // Suppress warning

  EXPECT_TRUE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(FallbackAllocator_Tests, Deallocate_BlockFromPrimary_PrimaryCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(Size);

  allocator.Deallocate(block);

  EXPECT_FALSE(deallocate_call[1]);
}

TEST(FallbackAllocator_Tests, Deallocate_BlockFromFallback_FallbackCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(Size + 1);

  allocator.Deallocate(block);

  EXPECT_TRUE(deallocate_call[1]);
}

TEST(FallbackAllocator_Tests, DeallocateAll_NoState_CallsBoth)
{
  FallbackAllocator<MockAllocator<1>, MockAllocator<2>> allocator;

  deallocateall_call[1] = false;
  deallocateall_call[2] = false;

  allocator.DeallocateAll();

  EXPECT_TRUE(deallocateall_call[1]);
  EXPECT_TRUE(deallocateall_call[2]);
}

TEST(FallbackAllocator_Tests, Reallocate_PrimaryBlockAndDontFail_PrimaryCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(Size / 2);

  bool result = allocator.Reallocate(block, Size);

  EXPECT_TRUE(result);
  EXPECT_FALSE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(FallbackAllocator_Tests, Reallocate_PrimaryBlockOwnsAndFails_FallbackCalled)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  allocate_call[1] = false;
  reallocate_call[1] = false;

  Block block1 = allocator.Allocate(Size / 2);

  bool result = allocator.Reallocate(block1, Size + 1);

  EXPECT_TRUE(result);
  EXPECT_TRUE(allocate_call[1]);
  EXPECT_FALSE(reallocate_call[1]);
  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, Size + 1);

  allocate_call[1] = false;

  Block block2 = allocator.Allocate(Size);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, Size);
  EXPECT_FALSE(allocate_call[1]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(FallbackAllocator_Tests, Reallocate_PrimaryOwnAndFallbackCalled_BytesCopied)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  Block block1 = allocator.Allocate(Size / 2);

  char old_memory[Size / 2];

  std::memcpy(old_memory, block1.ptr, Size / 2);

  allocator.Reallocate(block1, Size + 1);

  for (auto i = 0; i < 400; i++)
  {
    EXPECT_EQ(block1.ptr[i], old_memory[i]);
  }
}

TEST(FallbackAllocator_Tests, Reallocate_BothFail_Fails)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, StackAllocator<Size / 2>> allocator;

  Block block = allocator.Allocate(Size);

  bool result = allocator.Reallocate(block, Size + 1);

  EXPECT_FALSE(result);
  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size);
}

TEST(FallbackAllocator_Tests, Reallocate_FallbackOwns_CallsFallback)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = { nullptr, Size + 1 };

  bool result = allocator.Reallocate(block, Size * 2);

  EXPECT_TRUE(result);
  EXPECT_TRUE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(FallbackAllocator_Tests, Owns_NoneOwn_False)
{
  FallbackAllocator<StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = { nullptr, 801 };

  bool result = allocator.Owns(block);

  EXPECT_FALSE(result);
}

TEST(FallbackAllocator_Tests, Owns_PrimaryOwns_True)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, StackAllocator<400>> allocator;

  Block block = allocator.Allocate(Size);

  bool result = allocator.Owns(block);

  EXPECT_TRUE(result);
}

TEST(FallbackAllocator_Tests, Owns_FallbackOwns_True)
{
  constexpr size_t Size = cMaxAlignment * 100;

  FallbackAllocator<StackAllocator<Size>, MockAllocator<1>> allocator;

  owns_call[1] = false;

  Block block1 = allocator.Allocate(Size);
  Block block2 = allocator.Allocate(Size);

  bool result = allocator.Owns(block2);

  EXPECT_TRUE(result);
  EXPECT_TRUE(owns_call[1]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Segregator_Tests, Allocate_Small_CallsSmall)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, MockAllocator<1>, MockAllocator<2>> allocator;

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block1 = allocator.Allocate(Size / 4);

  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, Size / 4);
  EXPECT_TRUE(allocate_call[1]);
  EXPECT_FALSE(allocate_call[2]);

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block2 = allocator.Allocate(Size / 2);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, Size / 2);
  EXPECT_TRUE(allocate_call[1]);
  EXPECT_FALSE(allocate_call[2]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Segregator_Tests, Allocate_Large_CallsLarge)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, MockAllocator<1>, MockAllocator<2>> allocator;

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block1 = allocator.Allocate(Size + 1);

  EXPECT_NE(block1.ptr, nullptr);
  EXPECT_EQ(block1.size, Size + 1);
  EXPECT_FALSE(allocate_call[1]);
  EXPECT_TRUE(allocate_call[2]);

  allocate_call[1] = false;
  allocate_call[2] = false;
}

TEST(Segregator_Tests, Deallocate_Small_CallsSmall)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block1 = allocator.Allocate(Size);

  allocator.Deallocate(block1);

  Block block2 = allocator.Allocate(Size);

  EXPECT_NE(block2.ptr, nullptr);
  EXPECT_EQ(block2.size, Size);
  EXPECT_FALSE(deallocate_call[1]);
}

TEST(Segregator_Tests, Deallocate_Large_CallsLarge)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(Size + 1);

  allocator.Deallocate(block);

  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size + 1);
  EXPECT_TRUE(deallocate_call[1]);
}

TEST(Segregator_Tests, DeallocateAll_NoState_CallsBoth)
{
  Segregator<1000, MockAllocator<1>, MockAllocator<2>> allocator;

  deallocateall_call[1] = false;
  deallocateall_call[2] = false;

  allocator.DeallocateAll();

  EXPECT_TRUE(deallocateall_call[1]);
  EXPECT_TRUE(deallocateall_call[2]);
}

TEST(Segregator_Tests, Reallocate_Small_CallsSmall)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(Size / 2);

  bool result = allocator.Reallocate(block, Size);

  EXPECT_TRUE(result);
  EXPECT_NE(block.ptr, nullptr);
  EXPECT_EQ(block.size, Size);
  EXPECT_FALSE(reallocate_call[1]);
}

TEST(Segregator_Tests, Reallocate_SmallButFails_Fails)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block1 = allocator.Allocate(Size / 4);

  (void)block1;

  Block block2 = allocator.Allocate(Size / 2);

  char* old_ptr = block2.ptr;

  bool result = allocator.Reallocate(block2, Size);

  EXPECT_FALSE(result);
  EXPECT_EQ(block2.ptr, old_ptr);
  EXPECT_EQ(block2.size, Size / 2);
  EXPECT_FALSE(reallocate_call[1]);
}

TEST(Segregator_Tests, Reallocate_PreviouslySmallButNowLarge_CallsLarge)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(Size / 2);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, Size + 1);

  EXPECT_TRUE(result);
  EXPECT_NE(block.ptr, old_ptr);
  EXPECT_EQ(block.size, Size + 1);
  EXPECT_TRUE(allocate_call[1]);
}

TEST(Segregator_Tests, Reallocate_PreviouslySmallButNowLargeButFails_Fails)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, StackAllocator<Size / 2>> allocator;

  Block block = allocator.Allocate(Size / 2);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, Size + 1);

  EXPECT_FALSE(result);
  EXPECT_EQ(block.ptr, old_ptr);
  EXPECT_EQ(block.size, Size / 2);
}

TEST(Segregator_Tests, Reallocate_LargeAndLarger_CallsLarge)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(Size + 1);

  EXPECT_EQ(block.size, Size + 1);

  bool result = allocator.Reallocate(block, Size * 2);

  EXPECT_TRUE(result);
  EXPECT_EQ(block.size, Size * 2);
  EXPECT_TRUE(reallocate_call[1]);
}

TEST(Segregator_Tests, Owns_None)
{
  Segregator<800, StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = { nullptr, 801 };

  bool result = allocator.Owns(block);

  EXPECT_FALSE(result);
}

TEST(Segregator_Tests, Owns_SmallOwns_True)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, StackAllocator<Size / 2>> allocator;

  Block block = allocator.Allocate(Size);

  bool result = allocator.Owns(block);

  EXPECT_TRUE(result);
}

TEST(Segregator_Tests, Owns_LargeOwns_True)
{
  constexpr size_t Size = cMaxAlignment * 100;

  Segregator<Size, StackAllocator<Size>, MockAllocator<1>> allocator;

  owns_call[1] = false;

  Block block = allocator.Allocate(Size + 1);

  bool result = allocator.Owns(block);

  EXPECT_TRUE(result);
  EXPECT_TRUE(owns_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Allocate_NothingRecycled_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(256);

  EXPECT_TRUE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Deallocate_MaxSize_UsesFreelist)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(256);

  allocator.Deallocate(block);

  EXPECT_FALSE(deallocate_call[1]);
}

TEST(Freelist_Tests, Deallocate_MinSize_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(128);

  allocator.Deallocate(block);

  EXPECT_TRUE(deallocate_call[1]);
}

TEST(Freelist_Tests, Deallocate_BlockAllocatedFromParent_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(512);

  allocator.Deallocate(block);

  EXPECT_TRUE(deallocate_call[1]);
}

TEST(Freelist_Tests, DeallocateAll_NoBlocks_DoesNothing)
{
  Freelist<MockAllocator<1>, 256, 256> allocator;

  deallocate_call[1] = false;

  allocator.DeallocateAll();

  EXPECT_FALSE(deallocate_call[1]);
}

TEST(Freelist_Tests, DeallocateAll_OneBlock_CallsParent)
{
  Freelist<MockAllocator<1>, 256, 256> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(256);

  allocator.Deallocate(block);

  allocator.DeallocateAll();

  EXPECT_TRUE(deallocate_call[1]);
}

TEST(Freelist_Tests, Allocate_SomethingRecycledAndGoodSize_UseFreelist)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  allocator.Deallocate(allocator.Allocate(256));

  allocate_call[1] = false;

  Block block = allocator.Allocate(256);

  EXPECT_FALSE(allocate_call[1]);
}

TEST(Freelist_Tests, Allocate_SomethingRecycledAndMinSize_UsesFreelist)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  allocator.Deallocate(allocator.Allocate(256));

  allocate_call[1] = false;

  Block block = allocator.Allocate(128);

  EXPECT_FALSE(allocate_call[1]);
}

TEST(Freelist_Tests, Allocate_SomethingRecycledAndBigSize_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  allocator.Deallocate(allocator.Allocate(256));

  allocate_call[1] = false;

  Block block = allocator.Allocate(512);

  EXPECT_TRUE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Reallocate_NothingRecycled_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(128);
  allocator.Reallocate(block, 256);

  EXPECT_TRUE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Reallocate_SomethingRecycledGoodSize_UsesFreelist)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  Block t1 = allocator.Allocate(256);
  Block t2 = allocator.Allocate(256);

  allocator.Deallocate(t1);
  allocator.Deallocate(t2);

  reallocate_call[1] = false;

  Block block = allocator.Allocate(128);
  allocator.Reallocate(block, 256);

  EXPECT_FALSE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Reallocate_SomethingRecycledLargeSize_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  Block t1 = allocator.Allocate(256);
  Block t2 = allocator.Allocate(256);

  allocator.Deallocate(t1);
  allocator.Deallocate(t2);

  reallocate_call[1] = false;

  Block block = allocator.Allocate(128);
  allocator.Reallocate(block, 512);

  EXPECT_TRUE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Reallocate_SomethingRecycledSmallSize_CallsParent)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  Block t1 = allocator.Allocate(256);
  Block t2 = allocator.Allocate(256);

  allocator.Deallocate(t1);
  allocator.Deallocate(t2);

  reallocate_call[1] = false;

  Block block = allocator.Allocate(128);
  allocator.Reallocate(block, 64);

  EXPECT_TRUE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Owns_MaxSize_True)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  owns_call[1] = false;

  Block block = allocator.Allocate(256);

  EXPECT_TRUE(allocator.Owns(block));
  EXPECT_FALSE(owns_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Owns_MinSize_True)
{
  Freelist<MockAllocator<1>, 128, 256> allocator;

  owns_call[1] = false;

  Block block = allocator.Allocate(128);

  EXPECT_TRUE(allocator.Owns(block));
  EXPECT_TRUE(owns_call[1]);

  allocator.Deallocate(block);
}

TEST(Freelist_Tests, Destructor_WithBlock_DeallocatesAll)
{
  {
    Freelist<MockAllocator<1>, 256, 256> allocator;

    deallocate_call[1] = false;

    Block block = allocator.Allocate(256);

    allocator.Deallocate(block);
  }

  EXPECT_TRUE(deallocate_call[1]);
}
} // namespace genebits::engine::tests