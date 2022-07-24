#include "genebits/engine/containers/deque.h"

#include <benchmark/benchmark.h>

#include <deque>
#include <random>

namespace genebits::engine::bench
{
template<template<class> class Container>
static void Deque_PushPop(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Container<size_t> deque;

    for (size_t i = 0; i < amount; i++)
    {
      deque.push_back(value);
    }

    benchmark::ClobberMemory();

    for (size_t i = 0; i < amount; i++)
    {
      deque.pop_front();
    }

    benchmark::ClobberMemory();

    benchmark::DoNotOptimize(deque);
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK_TEMPLATE(Deque_PushPop, std::deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE(Deque_PushPop, Deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

template<template<class> class Container>
static void Deque_Relocatable_PushPop(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Container<std::tuple<size_t, size_t>> deque;

    for (size_t i = 0; i < amount; i++)
    {
      deque.push_back(std::make_tuple(value, value));
    }

    benchmark::ClobberMemory();

    for (size_t i = 0; i < amount; i++)
    {
      deque.pop_front();
    }

    benchmark::ClobberMemory();

    benchmark::DoNotOptimize(deque);
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK_TEMPLATE(Deque_Relocatable_PushPop, std::deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE(Deque_Relocatable_PushPop, Deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

template<template<class> class Container>
static void Deque_Iterate(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  Container<size_t> deque;

  for (size_t i = 0; i < amount; i++)
  {
    deque.push_back(value + i);
  }

  benchmark::ClobberMemory();

  for (auto _ : state)
  {
    for (auto it = deque.begin(); it != deque.end(); ++it)
    {
      benchmark::DoNotOptimize(*it);
    }
  }

  benchmark::DoNotOptimize(deque);

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK_TEMPLATE(Deque_Iterate, std::deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE(Deque_Iterate, Deque)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);
} // namespace genebits::engine::bench