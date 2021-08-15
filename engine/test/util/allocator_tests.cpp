#include "genebits/engine/util/allocator.h"

#include <map>
#include <vector>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(
  sizeof(Block) == sizeof(char*) + sizeof(size_t), "The  size of a block cannot excede that of a char* and size_t");

static_assert((cMaxAlignment & (cMaxAlignment - 1)) == 0, "Max alignment should be a power of two");

template<typename Type>
using MallocatorVector = std::vector<Type, AllocatorAdapter<Type, Mallocator>>;

// Static assert to check if the MallocatorVector compiles
static_assert(std::is_same_v<MallocatorVector<int>, MallocatorVector<int>>);

// These maps are used in the mock allocator
static std::map<size_t, bool> allocate_call;
static std::map<size_t, bool> deallocate_call;
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

TEST(Allocator_Tests, RoundToAlign_Aligned)
{
  ASSERT_EQ(RoundToAligned(0), 0u);
  ASSERT_EQ(RoundToAligned(8), 8u);
}

TEST(Allocator_Tests, RoundToAlign_NotAligned)
{
  for (size_t i = 1; i < 8; i++)
  {
    ASSERT_EQ(RoundToAligned(i), 8u);
  }

  for (size_t i = 9; i < 16; i++)
  {
    ASSERT_EQ(RoundToAligned(i), 16u);
  }
}

TEST(Allocator_Tests, RoundToAlign_VeryLargeNumberNotAligned)
{
  ASSERT_EQ(RoundToAligned(8001), 8008u);
  ASSERT_EQ(RoundToAligned(1600009), 1600016u);
}

TEST(Allocator_Tests, Mallocator_Allocate_AlwaysValid)
{
  Mallocator allocator;

  Block block1 = allocator.Allocate(1000);

  ASSERT_NE(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 1000u);

  Block block2 = allocator.Allocate(2000);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 2000u);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Allocator_Tests, Mallocator_Reallocate_Works)
{
  Mallocator allocator;

  Block block = allocator.Allocate(1000);

  bool result = allocator.Reallocate(block, 2000);

  ASSERT_TRUE(result);

  ASSERT_NE(block.ptr, nullptr);
  ASSERT_EQ(block.size, 2000u);

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, Mallocator_Owns)
{
  Mallocator allocator;

  Block block = allocator.Allocate(1000);

  ASSERT_TRUE(allocator.Owns(block));

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, StackAllocate_Allocate_SufficientSpace)
{
  StackAllocator<800> allocator;

  Block block1 = allocator.Allocate(400);

  ASSERT_NE(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 400u);

  Block block2 = allocator.Allocate(400);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 400u);
}

TEST(Allocator_Tests, StackAllocate_Allocate_InsufficientSpace)
{
  StackAllocator<800> allocator;

  Block block1 = allocator.Allocate(801);

  ASSERT_EQ(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 0u);
}

TEST(Allocator_Tests, StackAllocate_Deallocate_RegainSpace)
{
  StackAllocator<800> allocator;

  Block block = allocator.Allocate(800);

  allocator.Deallocate(block);

  block.ptr = nullptr;
  block.size = 0;

  block = allocator.Allocate(800);

  ASSERT_NE(block.ptr, nullptr);
  ASSERT_EQ(block.size, 800u);
}

TEST(Allocator_Tests, StackAllocate_Deallocate_NotLast)
{
  StackAllocator<800> allocator;

  Block block1 = allocator.Allocate(400);
  Block block2 = allocator.Allocate(400);

  (void)block2; // Suppress warning

  allocator.Deallocate(block1);

  Block block3 = allocator.Allocate(400);

  ASSERT_EQ(block3.ptr, nullptr);
  ASSERT_EQ(block3.size, 0u);
}

TEST(Allocator_Tests, StackAllocate_Reallocate_SufficientSpace)
{
  StackAllocator<800> allocator;

  Block block = allocator.Allocate(400);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, 800);

  ASSERT_TRUE(result);
  ASSERT_EQ(block.ptr, old_ptr);
  ASSERT_EQ(block.size, 800u);
}

TEST(Allocator_Tests, StackAllocate_Reallocate_InsufficientSpace)
{
  StackAllocator<800> allocator;

  Block block = allocator.Allocate(400);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, 801);

  ASSERT_FALSE(result);
  ASSERT_EQ(block.ptr, old_ptr);
  ASSERT_EQ(block.size, 400u);
}

TEST(Allocator_Tests, StackAllocate_Reallocate_NotLast)
{
  StackAllocator<800> allocator;

  Block block1 = allocator.Allocate(200);
  Block block2 = allocator.Allocate(400);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 400u);

  char* old_ptr = block1.ptr;

  bool result = allocator.Reallocate(block1, 401u);

  ASSERT_FALSE(result);
  ASSERT_EQ(block1.ptr, old_ptr);
  ASSERT_EQ(block1.size, 200u);

  Block block3 = allocator.Allocate(200);

  ASSERT_NE(block3.ptr, nullptr);
  ASSERT_EQ(block3.size, 200u);
}

TEST(Allocator_Tests, StackAllocate_Owns_Valid)
{
  StackAllocator<800> allocator;

  Block block1 = allocator.Allocate(400);
  Block block2 = allocator.Allocate(400);

  ASSERT_TRUE(allocator.Owns(block1));
  ASSERT_TRUE(allocator.Owns(block2));
}

TEST(Allocator_Tests, StackAllocate_Owns_NotValid)
{
  StackAllocator<800> allocator;

  char c = 'c';

  Block block { &c, 16 };

  ASSERT_FALSE(allocator.Owns(block));
}

TEST(Allocator_Tests, FallbackAllocator_Allocate_Primary)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(800);

  (void)block; // Suppress warning

  ASSERT_FALSE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, FallbackAllocator_Allocate_Fallback)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(801);

  (void)block; // Suppress warning

  ASSERT_TRUE(allocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, FallbackAllocator_Deallocate_Primary)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(800);

  allocator.Deallocate(block);

  ASSERT_FALSE(deallocate_call[1]);
}

TEST(Allocator_Tests, FallbackAllocator_Deallocate_Fallback)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(801);

  allocator.Deallocate(block);

  ASSERT_TRUE(deallocate_call[1]);
}

TEST(Allocator_Tests, FallbackAllocator_Reallocate_PrimaryOwns)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(400);

  bool result = allocator.Reallocate(block, 800);

  ASSERT_TRUE(result);
  ASSERT_FALSE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, FallbackAllocator_Reallocate_PrimaryOwnsButFails)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  allocate_call[1] = false;
  reallocate_call[1] = false;

  Block block1 = allocator.Allocate(400);

  char old_memory[400];

  std::memcpy(old_memory, block1.ptr, 400);

  bool result = allocator.Reallocate(block1, 801);

  ASSERT_TRUE(result);
  ASSERT_TRUE(allocate_call[1]);
  ASSERT_FALSE(reallocate_call[1]);
  ASSERT_NE(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 801u);

  for (auto i = 0; i < 400; i++)
  {
    ASSERT_EQ(block1.ptr[i], old_memory[i]);
  }

  allocate_call[1] = false;

  Block block2 = allocator.Allocate(800);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 800u);
  ASSERT_FALSE(allocate_call[1]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Allocator_Tests, FallbackAllocator_Reallocate_BothFail)
{
  FallbackAllocator<StackAllocator<800>, StackAllocator<200>> allocator;

  Block block = allocator.Allocate(800);

  bool result = allocator.Reallocate(block, 801);

  ASSERT_FALSE(result);
  ASSERT_NE(block.ptr, nullptr);
  ASSERT_EQ(block.size, 800u);
}

TEST(Allocator_Tests, FallbackAllocator_Reallocate_FallbackOwns)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = { nullptr, 801 };

  bool result = allocator.Reallocate(block, 2000);

  ASSERT_TRUE(result);
  ASSERT_TRUE(reallocate_call[1]);

  allocator.Deallocate(block);
}

TEST(Allocator_Tests, FallbackAllocator_Owns_None)
{
  FallbackAllocator<StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = { nullptr, 801 };

  bool result = allocator.Owns(block);

  ASSERT_FALSE(result);
}

TEST(Allocator_Tests, FallbackAllocator_Owns_Primary)
{
  FallbackAllocator<StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = allocator.Allocate(800);

  bool result = allocator.Owns(block);

  ASSERT_TRUE(result);
}

TEST(Allocator_Tests, FallbackAllocator_Owns_Fallback)
{
  FallbackAllocator<StackAllocator<800>, MockAllocator<1>> allocator;

  owns_call[1] = false;

  Block block1 = allocator.Allocate(800);
  Block block2 = allocator.Allocate(800);

  bool result = allocator.Owns(block2);

  ASSERT_TRUE(result);
  ASSERT_TRUE(owns_call[1]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Allocator_Tests, Segregator_Allocate_Small)
{
  Segregator<400, MockAllocator<1>, MockAllocator<2>> allocator;

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block1 = allocator.Allocate(200);

  ASSERT_NE(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 200u);
  ASSERT_TRUE(allocate_call[1]);
  ASSERT_FALSE(allocate_call[2]);

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block2 = allocator.Allocate(400);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 400u);
  ASSERT_TRUE(allocate_call[1]);
  ASSERT_FALSE(allocate_call[2]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Allocator_Tests, Segregator_Allocate_Large)
{
  Segregator<400, MockAllocator<1>, MockAllocator<2>> allocator;

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block1 = allocator.Allocate(401);

  ASSERT_NE(block1.ptr, nullptr);
  ASSERT_EQ(block1.size, 401u);
  ASSERT_FALSE(allocate_call[1]);
  ASSERT_TRUE(allocate_call[2]);

  allocate_call[1] = false;
  allocate_call[2] = false;

  Block block2 = allocator.Allocate(1000);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 1000u);
  ASSERT_FALSE(allocate_call[1]);
  ASSERT_TRUE(allocate_call[2]);

  allocator.Deallocate(block1);
  allocator.Deallocate(block2);
}

TEST(Allocator_Tests, Segregator_Deallocate_Small)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block1 = allocator.Allocate(400);

  allocator.Deallocate(block1);

  Block block2 = allocator.Allocate(400);

  ASSERT_NE(block2.ptr, nullptr);
  ASSERT_EQ(block2.size, 400u);
  ASSERT_FALSE(deallocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Deallocate_Large)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  deallocate_call[1] = false;

  Block block = allocator.Allocate(401);

  allocator.Deallocate(block);

  ASSERT_NE(block.ptr, nullptr);
  ASSERT_EQ(block.size, 401u);
  ASSERT_TRUE(deallocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Reallocate_Small)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(200);

  bool result = allocator.Reallocate(block, 400);

  ASSERT_TRUE(result);
  ASSERT_NE(block.ptr, nullptr);
  ASSERT_EQ(block.size, 400u);
  ASSERT_FALSE(reallocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Reallocate_SmallButFails)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block1 = allocator.Allocate(100);

  (void)block1;

  Block block2 = allocator.Allocate(200);

  char* old_ptr = block2.ptr;

  bool result = allocator.Reallocate(block2, 400);

  ASSERT_FALSE(result);
  ASSERT_EQ(block2.ptr, old_ptr);
  ASSERT_EQ(block2.size, 200u);
  ASSERT_FALSE(reallocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Reallocate_PreviouslySmallButNowLarge)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  allocate_call[1] = false;

  Block block = allocator.Allocate(200);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, 401);

  ASSERT_TRUE(result);
  ASSERT_NE(block.ptr, old_ptr);
  ASSERT_EQ(block.size, 401u);
  ASSERT_TRUE(allocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Reallocate_PreviouslySmallButNowLargeButFails)
{
  Segregator<400, StackAllocator<400>, StackAllocator<200>> allocator;

  Block block = allocator.Allocate(200);

  char* old_ptr = block.ptr;

  bool result = allocator.Reallocate(block, 401);

  ASSERT_FALSE(result);
  ASSERT_EQ(block.ptr, old_ptr);
  ASSERT_EQ(block.size, 200u);
}

TEST(Allocator_Tests, Segregator_Reallocate_Large)
{
  Segregator<400, StackAllocator<400>, MockAllocator<1>> allocator;

  reallocate_call[1] = false;

  Block block = allocator.Allocate(401);

  ASSERT_EQ(block.size, 401u);

  bool result = allocator.Reallocate(block, 800);

  ASSERT_TRUE(result);
  ASSERT_EQ(block.size, 800u);
  ASSERT_TRUE(reallocate_call[1]);
}

TEST(Allocator_Tests, Segregator_Owns_None)
{
  Segregator<800, StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = { nullptr, 801 };

  bool result = allocator.Owns(block);

  ASSERT_FALSE(result);
}

TEST(Allocator_Tests, Segregator_Owns_Small)
{
  Segregator<800, StackAllocator<800>, StackAllocator<400>> allocator;

  Block block = allocator.Allocate(800);

  bool result = allocator.Owns(block);

  ASSERT_TRUE(result);
}

TEST(Allocator_Tests, Segregator_Owns_Large)
{
  Segregator<800, StackAllocator<800>, MockAllocator<1>> allocator;

  owns_call[1] = false;

  Block block = allocator.Allocate(801);

  bool result = allocator.Owns(block);

  ASSERT_TRUE(result);
  ASSERT_TRUE(owns_call[1]);

  allocator.Deallocate(block);
}
} // namespace genebits::engine::tests