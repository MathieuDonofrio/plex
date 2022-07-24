#include "plex/utilities/memory.h"

#include <benchmark/benchmark.h>

#include <new>

namespace plex::bench
{
static void Memory_STD_Memcpy(benchmark::State& state)
{
  const size_t amount = state.range(0);

  const size_t bytes = 32 * amount;

  void* src = ::operator new(bytes, std::align_val_t(32));
  void* dst = ::operator new(bytes, std::align_val_t(32));

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(std::memcpy(dst, src, bytes));
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);

  ::operator delete(src, bytes, std::align_val_t(32));
  ::operator delete(dst, bytes, std::align_val_t(32));

  state.SetBytesProcessed(bytes);
}

BENCHMARK(Memory_STD_Memcpy)->Arg(100)->Arg(1000)->Arg(10000);

static void Memory_STD_Memmove(benchmark::State& state)
{
  const size_t amount = state.range(0);

  const size_t bytes = 32 * amount;

  void* src = ::operator new(bytes, std::align_val_t(32));
  void* dst = ::operator new(bytes, std::align_val_t(32));

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(std::memmove(dst, src, bytes));
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);

  ::operator delete(src, bytes, std::align_val_t(32));
  ::operator delete(dst, bytes, std::align_val_t(32));

  state.SetBytesProcessed(bytes);
}

BENCHMARK(Memory_STD_Memmove)->Arg(100)->Arg(1000)->Arg(10000);

static void Memory_STD_UninitializedRelocate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  const size_t bytes = 32 * amount;

  void* src = ::operator new(bytes, std::align_val_t(32));
  void* dst = ::operator new(bytes, std::align_val_t(32));

  char* first = reinterpret_cast<char*>(src);
  char* last = first + bytes;
  char* result = reinterpret_cast<char*>(dst);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(UninitializedRelocate(first, last, result));
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);

  ::operator delete(src, bytes, std::align_val_t(32));
  ::operator delete(dst, bytes, std::align_val_t(32));

  state.SetBytesProcessed(bytes);
}

BENCHMARK(Memory_STD_UninitializedRelocate)->Arg(100)->Arg(1000)->Arg(10000);

} // namespace plex::bench