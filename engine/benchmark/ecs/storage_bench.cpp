#include "genebits/engine/ecs/storage.h"

#include <benchmark/benchmark.h>

namespace genebits::engine
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
  Storage<size_t> storage;
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
  Storage<size_t> storage;
  storage.Initialize<Component<0>>();

  storage.Insert(999, Component<0> { 999, 999 });

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(storage.Contains(999));
  }
}

BENCHMARK(Storage_Contains);

static void Storage_Iterate_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  Storage<size_t> storage;
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
}

BENCHMARK(Storage_Iterate_100);

static void Storage_Iterate_Unpack1_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  Storage<size_t> storage;
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
}

BENCHMARK(Storage_Iterate_Unpack1_100);

static void Storage_Iterate_Unpack2_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  Storage<size_t> storage;
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
}

BENCHMARK(Storage_Iterate_Unpack2_100);

static void Storage_Insert_NoComponents_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i);
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_Insert_NoComponents_100);

static void Storage_InsertErase_NoComponents_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i);
    }

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_InsertErase_NoComponents_100);

static void Storage_Insert_OneComponent_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<Component<0>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_Insert_OneComponent_100);

static void Storage_InsertErase_OneComponent_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<Component<0>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i });
    }

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_InsertErase_OneComponent_100);

static void Storage_Insert_OneComponentNonTrivial_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<std::string>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, std::string { "Test" });
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_Insert_OneComponentNonTrivial_100);

static void Storage_InsertErase_OneComponentNonTrivial_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<std::string>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, std::string { "Test" });
    }

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_InsertErase_OneComponentNonTrivial_100);

static void Storage_Insert_TwoComponents_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<Component<0>, Component<1>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_Insert_TwoComponents_100);

static void Storage_InsertErase_TwoComponents_100(benchmark::State& state)
{
  constexpr size_t amount = 100;

  for (auto _ : state)
  {
    Storage<size_t> storage;
    storage.Initialize<Component<0>, Component<1>>();

    for (size_t i = 0; i < amount; i++)
    {
      storage.Insert(i, Component<0> { i, i }, Component<1> { i, i });
    }

    for (size_t i = 0; i < amount; i++)
    {
      storage.Erase(i);
    }

    benchmark::DoNotOptimize(storage);
  }
}

BENCHMARK(Storage_InsertErase_TwoComponents_100);
} // namespace genebits::engine