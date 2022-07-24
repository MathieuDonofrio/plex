#include "plex/containers/type_map.h"

#include <unordered_map>

#include <benchmark/benchmark.h>

#include "common/repeat.h"

namespace plex::bench
{
namespace
{
  template<size_t Tag = 0>
  struct TestType
  {
    [[maybe_unused]] std::atomic_size_t i = Tag;
  };

} // namespace

static void TypeMap_STD_UnorderedMap_Subscript(benchmark::State& state)
{
  std::unordered_map<std::string_view, int> map;

  benchmark::DoNotOptimize(map[TypeName<TestType<0>>()]);
  benchmark::DoNotOptimize(map[TypeName<TestType<1>>()]);
  benchmark::DoNotOptimize(map[TypeName<TestType<2>>()]);

  for (auto _ : state)
  {
    REPEAT8(benchmark::DoNotOptimize(map[TypeName<TestType<1001>>()]))
  }

  state.SetItemsProcessed(state.iterations() * 8);
}

BENCHMARK(TypeMap_STD_UnorderedMap_Subscript);

static void TypeMap_Assure(benchmark::State& state)
{
  TypeMap<int> map;

  benchmark::DoNotOptimize(map.Assure<TestType<0>>());
  benchmark::DoNotOptimize(map.Assure<TestType<1>>());
  benchmark::DoNotOptimize(map.Assure<TestType<2>>());

  for (auto _ : state)
  {
    REPEAT8(benchmark::DoNotOptimize(map.Assure<TestType<1001>>()));
  }

  state.SetItemsProcessed(state.iterations() * 8);
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
    REPEAT8(benchmark::DoNotOptimize(map.Get<TestType<1001>>()));
  }

  state.SetItemsProcessed(state.iterations() * 8);
}

BENCHMARK(TypeMap_Get);

} // namespace plex::bench