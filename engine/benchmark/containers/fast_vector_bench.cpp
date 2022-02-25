#include "genebits/engine/containers/fast_vector.h"

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
static void FastVector_STD_Vector_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  std::vector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.push_back(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_STD_Vector_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_STD_Vector_NonTrivial_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  std::vector<std::string> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_STD_Vector_NonTrivial_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_STD_Vector_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    std::vector<size_t> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(10);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_STD_Vector_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_STD_Vector_NonTrivial_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    std::vector<std::string> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back("Testing123");
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_STD_Vector_NonTrivial_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  FastVector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.PushBack(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_NonTrivial_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  FastVector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.PushBack(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_NonTrivial_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    FastVector<size_t> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i != amount; i++)
    {
      vector.PushBack(i);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void FastVector_NonTrivial_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    FastVector<std::string> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i != amount; i++)
    {
      vector.PushBack("Testing123");
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(FastVector_NonTrivial_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

} // namespace genebits::engine::bench