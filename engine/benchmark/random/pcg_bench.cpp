#include "plex/random/pcg.h"

#include <benchmark/benchmark.h>

#include "plex/random/seed.h"

namespace plex::bench
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
} // namespace plex::bench