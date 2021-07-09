#include "engine/util/allocator.h"

#include <string>

#include <benchmark/benchmark.h>

namespace genebits::engine
{
static void Allocator_STD_Allocator_AllocateDeallocate(benchmark::State& state)
{
  std::allocator<char> allocator;

  constexpr size_t size = 16000;

  for (auto _ : state)
  {
    char* ptr = allocator.allocate(size);

    benchmark::DoNotOptimize(ptr);

    allocator.deallocate(ptr, size);
  }
}

BENCHMARK(Allocator_STD_Allocator_AllocateDeallocate);

static void Allocator_StackAllocator_AllocateDeallocate(benchmark::State& state)
{
  constexpr size_t size = 16000;

  AllocatorAdapter<char, StackAllocator<size>> allocator;

  for (auto _ : state)
  {
    char* ptr = allocator.allocate(size);

    benchmark::DoNotOptimize(ptr);

    allocator.deallocate(ptr, size);
  }
}

BENCHMARK(Allocator_StackAllocator_AllocateDeallocate);

static void Allocator_Mallocator_AllocateDeallocate(benchmark::State& state)
{
  constexpr size_t size = 16000;

  AllocatorAdapter<char, Mallocator> allocator;

  for (auto _ : state)
  {
    char* ptr = allocator.allocate(size);

    benchmark::DoNotOptimize(ptr);

    allocator.deallocate(ptr, size);
  }
}

BENCHMARK(Allocator_Mallocator_AllocateDeallocate);

} // namespace genebits::engine