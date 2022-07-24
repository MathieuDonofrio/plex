#include "genebits/engine/random/seed.h"

#include <benchmark/benchmark.h>

namespace plex::bench
{
static void Random_Seed(benchmark::State& state)
{
  for (auto _ : state)
  {
    benchmark::DoNotOptimize(Seed());
  }
}

BENCHMARK(Random_Seed);
} // namespace plex::bench