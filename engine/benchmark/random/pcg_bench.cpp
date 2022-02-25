#include "genebits/engine/random/pcg.h"

#include <random>

#include <benchmark/benchmark.h>

#include "genebits/engine/random/seed.h"

namespace genebits::engine::bench
{
static void PCG_Next(benchmark::State& state)
{
  Random random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random.NextUInt());
  }
}

BENCHMARK(PCG_Next);

static void PCG_Next_Bounded(benchmark::State& state)
{
  Random random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random.NextUInt(50));
  }
}

BENCHMARK(PCG_Next_Bounded);
} // namespace genebits::engine::bench