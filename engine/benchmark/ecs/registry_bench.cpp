#include "genebits/engine/ecs/registry.h"

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
namespace
{
  template<size_t ID>
  struct Component
  {
    uint64_t data1;
    uint64_t data2;
  };
} // namespace

static void Registry_Iterate_OneArchetype(benchmark::State& state)
{
  Registry<size_t> registry;

  auto amount = static_cast<size_t>(state.range(0));

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create();
  }

  for (auto _ : state)
  {
    registry.ForEach([](size_t entity) { benchmark::DoNotOptimize(entity); });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Iterate_OneArchetype)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Iterate_OneArchetype_Unpack1(benchmark::State& state)
{
  Registry<size_t> registry;

  auto amount = static_cast<size_t>(state.range(0));

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create(Component<0> { i, i });
  }

  for (auto _ : state)
  {
    registry.ForEach<Component<0>>(
      [](size_t entity, Component<0> c1)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Iterate_OneArchetype_Unpack1)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Iterate_OneArchetype_Unpack2(benchmark::State& state)
{
  Registry<size_t> registry;

  auto amount = static_cast<size_t>(state.range(0));

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i });
  }

  for (auto _ : state)
  {
    registry.ForEach<Component<0>, Component<1>>(
      [](size_t entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Iterate_OneArchetype_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Iterate_TwoArchetypes_Unpack2(benchmark::State& state)
{
  Registry<size_t> registry;

  auto amount = static_cast<size_t>(state.range(0));

  for (size_t i = 0; i < amount / 2; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<2> { i, i });
  }

  for (size_t i = 0; i < amount / 2; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i });
  }

  for (auto _ : state)
  {
    registry.ForEach<Component<0>, Component<1>>(
      [](size_t entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Iterate_TwoArchetypes_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Iterate_TenArchetypes_Unpack2(benchmark::State& state)
{
  Registry<size_t> registry;

  auto amount = static_cast<size_t>(state.range(0));

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<10> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<11> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<12> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<13> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<14> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<15> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<16> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<17> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<18> { i, i });
  }

  for (size_t i = 0; i < amount / 10; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i }, Component<19> { i, i });
  }

  for (auto _ : state)
  {
    registry.ForEach<Component<0>, Component<1>>(
      [](size_t entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Iterate_TenArchetypes_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Create_NoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create();
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Create_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Create_OneComponent(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i });
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Create_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Create_TwoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i }, Component<1> { i, i });
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Create_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Destroy_NoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create();
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Destroy_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Destroy_OneComponent(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i });
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));
}

BENCHMARK(Registry_Destroy_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Destroy_TwoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry<size_t> registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i }, Component<1> { i, i });
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Registry_Destroy_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();
} // namespace genebits::engine::bench