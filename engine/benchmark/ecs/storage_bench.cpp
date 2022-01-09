#include "genebits/engine/ecs/storage.h"

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

static void Storage_Unpack(benchmark::State& state)
{
  SharedSparseArray<size_t> sparse;
  Storage<size_t> storage(&sparse);
  storage.Initialize<Component<0>>();

  storage.Insert(999, Component<0> { 999, 999 });

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(storage.Unpack<Component<0>>(999));
  }
}

BENCHMARK(Storage_Unpack);

static void Storage_Contains(benchmark::State& state)
{
  SharedSparseArray<size_t> sparse;
  Storage<size_t> storage(&sparse);
  storage.Initialize<Component<0>>();

  storage.Insert(999, Component<0> { 999, 999 });

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(storage.Contains(999));
  }
}

BENCHMARK(Storage_Contains);

static void Storage_Iterate(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;
  Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Iterate_Unpack1(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;
  Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Iterate_Unpack1)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Iterate_Unpack2(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;
  Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Iterate_Unpack2)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Insert_NoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
    storage.Initialize<>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i);
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Insert_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Insert_OneComponent(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
    storage.Initialize<Component<0>>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Insert_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Insert_OneComponentNonTrivial(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
    storage.Initialize<std::string>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, std::string { "Test" });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Insert_OneComponentNonTrivial)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Insert_TwoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
    storage.Initialize<Component<0>, Component<1>>();

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Insert_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Erase_NoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Erase_NoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Erase_OneComponent(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Erase_OneComponent)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Erase_OneComponentNonTrivial(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));
}

BENCHMARK(Storage_Erase_OneComponentNonTrivial)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void Storage_Erase_TwoComponents(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  SharedSparseArray<size_t> sparse;

  for (auto _ : state)
  {
    state.PauseTiming();

    Storage<size_t> storage(&sparse);
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

  state.SetComplexityN(static_cast<int64_t>(amount));

}

BENCHMARK(Storage_Erase_TwoComponents)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();
} // namespace genebits::engine::bench