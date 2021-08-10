#include "engine/util/type_map.h"

#include <map>
#include <unordered_map>

#include <benchmark/benchmark.h>

namespace genebits::engine
{
namespace
{
  template<size_t Tag = 0>
  struct TestType
  {
  };

} // namespace

static void TypeMap_STD_Map_Assure(benchmark::State& state)
{
  std::map<size_t, int> map;

  benchmark::DoNotOptimize(map[Meta<TestType<0>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<1>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<2>>::Hash()]);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map[Meta<TestType<9999>>::Hash()]);
  }
}

BENCHMARK(TypeMap_STD_Map_Assure);

static void TypeMap_STD_UnorderedMap_Assure(benchmark::State& state)
{
  std::unordered_map<size_t, int> map;

  benchmark::DoNotOptimize(map[Meta<TestType<0>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<1>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<2>>::Hash()]);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map[Meta<TestType<9999>>::Hash()]);
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
    benchmark::DoNotOptimize(map.Assure<TestType<9999>>());
  }
}

BENCHMARK(TypeMap_Assure);

} // namespace genebits::engine