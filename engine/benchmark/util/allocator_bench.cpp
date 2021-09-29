#include "genebits/engine/util/allocator.h"

#include <string>

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
static void Allocator_STD_Allocator_Allocate(benchmark::State& state)
{
  std::allocator<char> allocator;

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    char* ptr = allocator.allocate(amount);

    state.PauseTiming();

    allocator.deallocate(ptr, amount);

    benchmark::DoNotOptimize(ptr);

    state.ResumeTiming();
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Allocator_STD_Allocator_Allocate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Allocator_STD_Allocator_Deallocate(benchmark::State& state)
{
  std::allocator<char> allocator;

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    char* ptr = allocator.allocate(amount);

    benchmark::DoNotOptimize(ptr);

    state.ResumeTiming();

    allocator.deallocate(ptr, amount);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Allocator_STD_Allocator_Deallocate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Allocator_Mallocator_Allocate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  Mallocator allocator;

  for (auto _ : state)
  {
    Block block = allocator.Allocate(amount);

    state.PauseTiming();

    benchmark::DoNotOptimize(block);

    allocator.Deallocate(block);

    state.ResumeTiming();
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Allocator_Mallocator_Allocate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Allocator_Mallocator_Deallocate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  Mallocator allocator;

  for (auto _ : state)
  {
    state.PauseTiming();

    Block block = allocator.Allocate(amount);

    benchmark::DoNotOptimize(block);

    state.ResumeTiming();

    allocator.Deallocate(block);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Allocator_Mallocator_Deallocate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

} // namespace genebits::engine::bench