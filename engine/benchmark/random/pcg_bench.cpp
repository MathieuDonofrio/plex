#include "genebits/engine/random/pcg.h"

#include <benchmark/benchmark.h>

#include "genebits/engine/random/seed.h"

namespace genebits::engine::bench
{
static void PCG_Next(benchmark::State& state)
{
  PCG random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random());
  }
}

BENCHMARK(PCG_Next);

static void PCG_NextBounded(benchmark::State& state)
{
  PCG random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random(50));
  }
}

BENCHMARK(PCG_NextBounded);
} // namespace genebits::engine::bench