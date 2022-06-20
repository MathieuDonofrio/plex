#include "genebits/engine/containers/type_map.h"

#include <map>
#include <unordered_map>

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

static void TypeMap_STD_UnorderedMap_Assure(benchmark::State& state)
{
  std::unordered_map<size_t, int> map;

  benchmark::DoNotOptimize(map[TypeHash<TestType<0>>()]);
  benchmark::DoNotOptimize(map[TypeHash<TestType<1>>()]);
  benchmark::DoNotOptimize(map[TypeHash<TestType<2>>()]);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map[TypeHash<TestType<1001>>()]);

    benchmark::ClobberMemory();
  }
}

BENCHMARK(TypeMap_STD_UnorderedMap_Assure);

static void TypeMap_Assure(benchmark::State& state)
{
  TypeMap<int> map;

  benchmark::DoNotOptimize(map.Assure<TestType<0>>());
  benchmark::DoNotOptimize(map.Assure<TestType<1>>());
  benchmark::DoNotOptimize(map.Assure<TestType<2>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Assure<TestType<1001>>());

    benchmark::ClobberMemory();
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

    benchmark::ClobberMemory();
  }
}

BENCHMARK(TypeMap_Get);

} // namespace genebits::engine::bench