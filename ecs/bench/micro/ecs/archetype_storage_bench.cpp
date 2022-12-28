#include "plex/ecs/archetype_storage.h"

#include <benchmark/benchmark.h>

namespace plex::bench
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

static void ArchetypeStorage_Unpack(benchmark::State& state)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<Component<0>>();

  storage.Insert(999, Component<0> { 999, 999 });

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(storage.Unpack<Component<0>>(999));
  }
}

BENCHMARK(ArchetypeStorage_Unpack);

static void ArchetypeStorage_Contains(benchmark::State& state)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<Component<0>>();

  storage.Insert(999, Component<0> { 999, 999 });

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(storage.Contains(999));
  }
}

BENCHMARK(ArchetypeStorage_Contains);

static void ArchetypeStorage_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  for (size_t i = 0; i < amount; i++)
  {
    storage.Insert(i);
  }

  for (auto _ : state)
  {
    for (size_t i = 0; i < amount; i++)
    {
      benchmark::DoNotOptimize(storage[i]);
    }
  }

  benchmark::DoNotOptimize(storage);

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Iterate_Unpack1(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<Component<0>>();

  for (size_t i = 0; i < amount; i++)
  {
    storage.Insert(i, Component<0> { i, i });
  }

  for (auto _ : state)
  {
    auto& array1 = storage.Access<Component<0>>();

    for (size_t i = 0; i < amount; i++)
    {
      benchmark::DoNotOptimize(storage[i]);
      benchmark::DoNotOptimize(array1[i]);
    }
  }

  benchmark::DoNotOptimize(storage);

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Iterate_Unpack1)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Iterate_Unpack2(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<Component<0>, Component<1>>();

  for (size_t i = 0; i < amount; i++)
  {
    storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
  }

  for (auto _ : state)
  {
    auto& array1 = storage.Access<Component<0>>();
    auto& array2 = storage.Access<Component<1>>();

    for (size_t i = 0; i < amount; i++)
    {
      benchmark::DoNotOptimize(storage[i]);
      benchmark::DoNotOptimize(array1[i]);
      benchmark::DoNotOptimize(array2[i]);
    }
  }

  benchmark::DoNotOptimize(storage);

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Iterate_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Insert_NoComponents(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Insert_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Insert_OneComponent(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<Component<0>>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Insert_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Insert_OneComponentNonTrivial(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<std::string>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, std::string { "Test" });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Insert_OneComponentNonTrivial)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Insert_TwoComponents(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<Component<0>, Component<1>>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Insert_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Erase_NoComponents(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i);
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Erase_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Erase_OneComponent(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<Component<0>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i });
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Erase_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Erase_OneComponentNonTrivial(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<std::string>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, std::string { "Test" });
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Erase_OneComponentNonTrivial)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);

static void ArchetypeStorage_Erase_TwoComponents(benchmark::State& state)
{
  const size_t amount = state.range(0);

  ArchetypeStorageSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    ArchetypeStorage<size_t> storage(&sparse);
    storage.Initialize<Component<0>, Component<1>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ArchetypeStorage_Erase_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(::benchmark::oN);
} // namespace plex::bench