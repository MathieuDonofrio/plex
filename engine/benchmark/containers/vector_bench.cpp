#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/ref.h"

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
static void Vector_STD_Vector_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  std::vector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.push_back(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Vector_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_STD_Vector_NonTrivial_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  std::vector<std::string> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Vector_NonTrivial_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_STD_Vector_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    std::vector<size_t> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(10);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Vector_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_STD_Vector_Ref_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    std::vector<Ref<size_t>> items;

    for (size_t i = 0; i < amount; i++)
    {
      items.push_back(MakeRef<size_t>(i));
    }

    std::vector<Ref<size_t>> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(items[i]);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Vector_Ref_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_STD_Vector_NonTrivial_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    std::vector<std::string> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back("Testing123");
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Vector_NonTrivial_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  Vector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.PushBack(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_NonTrivial_Iterate(benchmark::State& state)
{
  const size_t amount = state.range(0);

  Vector<size_t> vector;

  for (size_t i = 0; i != amount; i++)
  {
    vector.PushBack(i);
  }

  for (auto _ : state)
  {
    for (auto value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_NonTrivial_Iterate)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Vector<size_t> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i != amount; i++)
    {
      vector.PushBack(i);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_Ref_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Vector<Ref<size_t>> items;

    for (size_t i = 0; i != amount; i++)
    {
      items.PushBack(MakeRef<size_t>(i));
    }

    Vector<Ref<size_t>> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i != amount; i++)
    {
      vector.PushBack(items[i]);
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_Ref_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_NonTrivial_PushBack(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    Vector<std::string> vector;

    benchmark::DoNotOptimize(vector);

    state.ResumeTiming();

    for (size_t i = 0; i != amount; i++)
    {
      vector.PushBack("Testing123");
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_NonTrivial_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

} // namespace genebits::engine::bench