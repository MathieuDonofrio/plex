#include "plex/math/funcs_common.h"

#include <benchmark/benchmark.h>

#include "plex/random/pcg.h"
#include "plex/random/seed.h"

namespace plex::bench
{
static void Pow_STD_Reference_FloatBaseIntExp(benchmark::State& state)
{
  int amount = static_cast<int>(state.range(0));

  for (auto _ : state)
  {
    float result = std::powf(0.78f, static_cast<float>(amount));

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(amount);
}

BENCHMARK(Pow_STD_Reference_FloatBaseIntExp)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Pow_FloatBaseIntExp(benchmark::State& state)
{
  int amount = static_cast<int>(state.range(0));

  for (auto _ : state)
  {
    float result = Pow(0.78f, amount);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(amount);
}

BENCHMARK(Pow_FloatBaseIntExp)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Sqrt_STD_Reference_Float(benchmark::State& state)
{
  PCG pcg { Seed() };
  float x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    float result = std::sqrtf(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(Sqrt_STD_Reference_Float);

static void Sqrt_Float(benchmark::State& state)
{
  PCG pcg { Seed() };
  float x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    float result = Sqrt(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(Sqrt_Float);

static void RSqrt_STD_Reference_Float(benchmark::State& state)
{
  PCG pcg { Seed() };
  float x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    float result = 1.0f / std::sqrtf(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(RSqrt_STD_Reference_Float);

static void RSqrt_Float(benchmark::State& state)
{
  PCG pcg { Seed() };
  float x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    float result = RSqrt(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(RSqrt_Float);

static void RSqrt_STD_Reference_Double(benchmark::State& state)
{
  PCG pcg { Seed() };
  double x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    double result = 1.0f / std::sqrt(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(RSqrt_STD_Reference_Double);

static void RSqrt_Double(benchmark::State& state)
{
  PCG pcg { Seed() };
  double x = static_cast<float>(pcg()) / (static_cast<float>(RAND_MAX / 10000.0f));

  for (auto _ : state)
  {
    double result = RSqrt(x);

    benchmark::DoNotOptimize(result);
  }

  benchmark::DoNotOptimize(x);
}

BENCHMARK(RSqrt_Double);

} // namespace plex::bench