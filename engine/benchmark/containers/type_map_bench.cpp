#include "genebits/engine/containers/type_map.h"

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
namespace
{
  template<size_t Tag = 0>
  struct TestType
  {
    [[maybe_unused]] std::atomic_size_t i = Tag;
  };

} // namespace

static void TypeMap_Assure(benchmark::State& state)
{
  TypeMap<int> map;

  benchmark::DoNotOptimize(map.Assure<TestType<0>>());
  benchmark::DoNotOptimize(map.Assure<TestType<1>>());
  benchmark::DoNotOptimize(map.Assure<TestType<2>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Assure<TestType<1001>>());
  }
}

BENCHMARK(TypeMap_Assure);

static void TypeMap_Get(benchmark::State& state)
{
  TypeMap<int> map;

  benchmark::DoNotOptimize(map.Assure<TestType<0>>());
  benchmark::DoNotOptimize(map.Assure<TestType<1>>());
  benchmark::DoNotOptimize(map.Assure<TestType<2>>());

  benchmark::DoNotOptimize(map.Assure<TestType<1001>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<1001>>());
  }
}

BENCHMARK(TypeMap_Get);

} // namespace genebits::engine::bench