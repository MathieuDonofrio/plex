#include "genebits/engine/util/random.h"

#include <random>

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
static void Random_STD_Rand(benchmark::State& state)
{
  std::srand(static_cast<unsigned int>(Seed()));

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(std::rand());
  }
}

BENCHMARK(Random_STD_Rand);

static void Random_STD_mt19937_Rand(benchmark::State& state)
{
  std::mt19937 random(static_cast<unsigned int>(Seed()));

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random());
  }
}

BENCHMARK(Random_STD_mt19937_Rand);

static void Random_NextUInt(benchmark::State& state)
{
  Random random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random.NextUInt());
  }
}

BENCHMARK(Random_NextUInt);

static void Random_NextUInt_Bounded(benchmark::State& state)
{
  Random random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random.NextUInt(50));
  }
}

BENCHMARK(Random_NextUInt_Bounded);

static void Random_NextFloat(benchmark::State& state)
{
  Random random(Seed()); // Use runtime seed.

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(random.NextFloat());
  }
}

BENCHMARK(Random_NextFloat);

static void Random_Seed(benchmark::State& state)
{
  for (auto _ : state)
  {
    benchmark::DoNotOptimize(Seed());
  }
}

BENCHMARK(Random_Seed);
} // namespace genebits::engine::bench