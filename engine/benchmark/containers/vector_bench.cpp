#include "genebits/engine/containers/vector.h"

#include <benchmark/benchmark.h>

#include <random>
#include <vector>

namespace plex::bench
{
template<template<class> class Container>
static void Vector_Reserve(benchmark::State& state)
{
  for (auto _ : state)
  {
    Container<size_t> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
  }
}

BENCHMARK_TEMPLATE(Vector_Reserve, std::vector);
BENCHMARK_TEMPLATE(Vector_Reserve, Vector);

template<template<class> class Container>
static void Vector_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    Container<size_t> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.push_back(value);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_TEMPLATE(Vector_PushBack, std::vector);
BENCHMARK_TEMPLATE(Vector_PushBack, Vector);

template<template<class> class Container>
static void Vector_Consecutive_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Container<size_t> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(value);
    }

    benchmark::ClobberMemory();

    benchmark::DoNotOptimize(vector.data());
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK_TEMPLATE(Vector_Consecutive_PushBack, std::vector)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(Vector_Consecutive_PushBack, Vector)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

template<template<class> class Container>
static void Vector_Relocatable_Consecutive_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Container<std::tuple<size_t, size_t>> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(std::make_tuple(value, value));
    }

    benchmark::ClobberMemory();

    benchmark::DoNotOptimize(vector.data());
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK_TEMPLATE(Vector_Relocatable_Consecutive_PushBack, std::vector)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE(Vector_Relocatable_Consecutive_PushBack, Vector)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Complexity(benchmark::oN);

} // namespace plex::bench