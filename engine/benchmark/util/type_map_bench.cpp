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

static void TypeMap_STD_Map_Get(benchmark::State& state)
{
  std::map<size_t, int> map;

  benchmark::DoNotOptimize(map[Meta<TestType<0>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<1>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<2>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<3>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<4>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<5>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<6>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<7>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<8>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<9>>::Hash()]);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 0>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 1>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 2>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 3>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 4>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 5>>::Hash()]);
  }
}

BENCHMARK(TypeMap_STD_Map_Get);

static void TypeMap_STD_UnorderedMap_Get(benchmark::State& state)
{
  std::unordered_map<size_t, int> map;

  benchmark::DoNotOptimize(map[Meta<TestType<0>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<1>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<2>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<3>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<4>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<5>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<6>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<7>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<8>>::Hash()]);
  benchmark::DoNotOptimize(map[Meta<TestType<9>>::Hash()]);

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 0>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 1>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 2>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 3>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 4>>::Hash()]);
    benchmark::DoNotOptimize(map[Meta<TestType<9999 + 5>>::Hash()]);
  }
}

BENCHMARK(TypeMap_STD_UnorderedMap_Get);

static void TypeMap_Get_Load012(benchmark::State& state)
{
  TypeMap<int, 128> map;

  benchmark::DoNotOptimize(map.Get<TestType<0>>());
  benchmark::DoNotOptimize(map.Get<TestType<1>>());
  benchmark::DoNotOptimize(map.Get<TestType<2>>());
  benchmark::DoNotOptimize(map.Get<TestType<3>>());
  benchmark::DoNotOptimize(map.Get<TestType<4>>());
  benchmark::DoNotOptimize(map.Get<TestType<5>>());
  benchmark::DoNotOptimize(map.Get<TestType<6>>());
  benchmark::DoNotOptimize(map.Get<TestType<7>>());
  benchmark::DoNotOptimize(map.Get<TestType<8>>());
  benchmark::DoNotOptimize(map.Get<TestType<9>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 0>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 1>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 2>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 3>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 4>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 5>>());
  }
}

BENCHMARK(TypeMap_Get_Load012);

static void TypeMap_Get_Load025(benchmark::State& state)
{
  TypeMap<int, 64> map;

  benchmark::DoNotOptimize(map.Get<TestType<0>>());
  benchmark::DoNotOptimize(map.Get<TestType<1>>());
  benchmark::DoNotOptimize(map.Get<TestType<2>>());
  benchmark::DoNotOptimize(map.Get<TestType<3>>());
  benchmark::DoNotOptimize(map.Get<TestType<4>>());
  benchmark::DoNotOptimize(map.Get<TestType<5>>());
  benchmark::DoNotOptimize(map.Get<TestType<6>>());
  benchmark::DoNotOptimize(map.Get<TestType<7>>());
  benchmark::DoNotOptimize(map.Get<TestType<8>>());
  benchmark::DoNotOptimize(map.Get<TestType<9>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 0>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 1>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 2>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 3>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 4>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 5>>());
  }
}

BENCHMARK(TypeMap_Get_Load025);

static void TypeMap_Get_Load050(benchmark::State& state)
{
  TypeMap<int, 32> map;

  benchmark::DoNotOptimize(map.Get<TestType<0>>());
  benchmark::DoNotOptimize(map.Get<TestType<1>>());
  benchmark::DoNotOptimize(map.Get<TestType<2>>());
  benchmark::DoNotOptimize(map.Get<TestType<3>>());
  benchmark::DoNotOptimize(map.Get<TestType<4>>());
  benchmark::DoNotOptimize(map.Get<TestType<5>>());
  benchmark::DoNotOptimize(map.Get<TestType<6>>());
  benchmark::DoNotOptimize(map.Get<TestType<7>>());
  benchmark::DoNotOptimize(map.Get<TestType<8>>());
  benchmark::DoNotOptimize(map.Get<TestType<9>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 0>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 1>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 2>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 3>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 4>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 5>>());
  }
}

BENCHMARK(TypeMap_Get_Load050);

static void TypeMap_Get_Load075(benchmark::State& state)
{
  TypeMap<int, 24> map;

  benchmark::DoNotOptimize(map.Get<TestType<0>>());
  benchmark::DoNotOptimize(map.Get<TestType<1>>());
  benchmark::DoNotOptimize(map.Get<TestType<2>>());
  benchmark::DoNotOptimize(map.Get<TestType<3>>());
  benchmark::DoNotOptimize(map.Get<TestType<4>>());
  benchmark::DoNotOptimize(map.Get<TestType<5>>());
  benchmark::DoNotOptimize(map.Get<TestType<6>>());
  benchmark::DoNotOptimize(map.Get<TestType<7>>());
  benchmark::DoNotOptimize(map.Get<TestType<8>>());
  benchmark::DoNotOptimize(map.Get<TestType<9>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 0>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 1>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 2>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 3>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 4>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 5>>());
  }
}

BENCHMARK(TypeMap_Get_Load075);

static void TypeMap_Get_Load100(benchmark::State& state)
{
  TypeMap<int, 16> map;

  benchmark::DoNotOptimize(map.Get<TestType<0>>());
  benchmark::DoNotOptimize(map.Get<TestType<1>>());
  benchmark::DoNotOptimize(map.Get<TestType<2>>());
  benchmark::DoNotOptimize(map.Get<TestType<3>>());
  benchmark::DoNotOptimize(map.Get<TestType<4>>());
  benchmark::DoNotOptimize(map.Get<TestType<5>>());
  benchmark::DoNotOptimize(map.Get<TestType<6>>());
  benchmark::DoNotOptimize(map.Get<TestType<7>>());
  benchmark::DoNotOptimize(map.Get<TestType<8>>());
  benchmark::DoNotOptimize(map.Get<TestType<9>>());

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 0>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 1>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 2>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 3>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 4>>());
    benchmark::DoNotOptimize(map.Get<TestType<9999 + 5>>());
  }
}

BENCHMARK(TypeMap_Get_Load100);

} // namespace genebits::engine