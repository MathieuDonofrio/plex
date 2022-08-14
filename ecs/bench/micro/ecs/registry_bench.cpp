#include "plex/ecs/registry.h"

#include <benchmark/benchmark.h>

#include "plex/math/vec4.h"

namespace plex::bench
{
namespace
{
  struct Position
  {
    float4 data;
  };

  struct Velocity
  {
    float4 data;
  };

  template<size_t ID>
  struct Component
  {
    uint64_t data1;
    uint64_t data2;
  };
} // namespace

static void Registry_Iterate_SimpleWork_ManualFor(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

  for (float f = 0; f < static_cast<float>(amount); f++)
  {
    registry.Create(Position { { f, f, f, f } }, Velocity { { f, f, f, f } });
  }

  for (auto _ : state)
  {
    for (auto sub_view : registry.ViewFor<Position, Velocity>())
    {
      auto last = sub_view.end<Position, Velocity>();

      for (auto first = sub_view.begin<Position, Velocity>(); first != last; ++first)
      {
        auto data = *first;

        std::get<Position>(data).data += std::get<Velocity>(data).data * std::get<Velocity>(data).data;

        benchmark::DoNotOptimize(std::get<Position>(data));
      }
    }
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_SimpleWork_ManualFor)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Arg(100000)
  ->Complexity(::benchmark::oN);

static void Registry_Iterate_SimpleWork_ForEach(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

  for (float f = 0; f < static_cast<float>(amount); f++)
  {
    registry.Create(Position { { f, f, f, f } }, Velocity { { f, f, f, f } });
  }

  for (auto _ : state)
  {
    EntityForEach(registry.ViewFor<Position, const Velocity>(),
      [](Position& position, const Velocity& velocity)
      {
        position.data += velocity.data * velocity.data;
        benchmark::DoNotOptimize(position.data);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_SimpleWork_ForEach)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Arg(100000)
  ->Complexity(::benchmark::oN);

static void Registry_Iterate_OneArchetype(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create();
  }

  for (auto _ : state)
  {
    EntityForEach(registry.ViewFor<>(), [](Entity entity) { benchmark::DoNotOptimize(entity); });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_OneArchetype)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Iterate_OneArchetype_Unpack1(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create(Component<0> { i, i });
  }

  for (auto _ : state)
  {
    EntityForEach(registry.ViewFor<Component<0>>(),
      [](Entity entity, Component<0> c1)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_OneArchetype_Unpack1)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Iterate_OneArchetype_Unpack2(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create(Component<0> { i, i }, Component<1> { i, i });
  }

  for (auto _ : state)
  {
    EntityForEach(registry.ViewFor<Component<0>, Component<1>>(),
      [](Entity entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_OneArchetype_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Iterate_TwoArchetypes_Unpack2(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

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
    EntityForEach(registry.ViewFor<Component<0>, Component<1>>(),
      [](Entity entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_TwoArchetypes_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Iterate_TenArchetypes_Unpack2(benchmark::State& state)
{
  Registry registry;

  size_t amount = state.range(0);

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
    EntityForEach(registry.ViewFor<Component<0>, Component<1>>(),
      [](Entity entity, Component<0> c1, Component<1> c2)
      {
        benchmark::DoNotOptimize(entity);
        benchmark::DoNotOptimize(c1);
        benchmark::DoNotOptimize(c2);
      });
  }

  benchmark::DoNotOptimize(registry);

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Iterate_TenArchetypes_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Create_NoComponents(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create();
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Create_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Registry_Create_OneComponent(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i });
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Create_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Create_TwoComponents(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i }, Component<1> { i, i });
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Create_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Destroy_NoComponents(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create();
    }

    state.ResumeTiming();

    for (Entity i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Destroy_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Destroy_OneComponent(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i });
    }

    state.ResumeTiming();

    for (Entity i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Destroy_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void Registry_Destroy_TwoComponents(benchmark::State& state)
{
  size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Registry registry;

    for (size_t i = 0; i < amount; i++)
    {
      registry.Create(Component<0> { i, i }, Component<1> { i, i });
    }

    state.ResumeTiming();

    for (Entity i = 0; i < amount; i++)
    {
      registry.Destroy(i);
    }

    benchmark::DoNotOptimize(registry);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Registry_Destroy_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);
} // namespace plex::bench