#include "genebits/engine/os/memory.h"

#include <benchmark/benchmark.h>

#include <new>

namespace genebits::engine::bench
{
static void Memory_STD_Memcpy(benchmark::State& state)
{
  const size_t amount = state.range(0);

  const size_t bytes = 32 * amount;

  void* src = ::operator new(bytes, std::align_val_t(32));
  void* dst = ::operator new(bytes, std::align_val_t(32));

  for (auto _ : state)
  {
    std::memcpy(dst, src, bytes);
  }

  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);

  ::operator delete(src, bytes, std::align_val_t(32));
  ::operator delete(dst, bytes, std::align_val_t(32));
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
    std::memmove(dst, src, bytes);
  }

  benchmark::DoNotOptimize(src);
  benchmark::DoNotOptimize(dst);

  ::operator delete(src, bytes, std::align_val_t(32));
  ::operator delete(dst, bytes, std::align_val_t(32));
}

BENCHMARK(Memory_STD_Memmove)->Arg(100)->Arg(1000)->Arg(10000);

} // namespace genebits::engine::bench