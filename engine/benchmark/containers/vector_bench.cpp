#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/ref.h"

#include <benchmark/benchmark.h>

#include <random>

namespace genebits::engine::bench
{
static void Vector_STD_Reference_Reserve(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::vector<size_t> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
  }
}

BENCHMARK(Vector_STD_Reference_Reserve);

static void Vector_Reserve(benchmark::State& state)
{
  for (auto _ : state)
  {
    Vector<size_t> vector;
    vector.Reserve(1);
    benchmark::DoNotOptimize(vector.data());
  }
}

BENCHMARK(Vector_Reserve);

static void Vector_STD_Reference_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    std::vector<size_t> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.push_back(value);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(Vector_STD_Reference_PushBack);

static void Vector_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    Vector<size_t> vector;
    vector.Reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.PushBack(value);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(Vector_PushBack);

static void Vector_STD_Reference_SmartPtr_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    std::vector<Ref<size_t>> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.push_back(MakeRef<size_t>(value));
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);
}

BENCHMARK(Vector_STD_Reference_SmartPtr_PushBack);

static void Vector_SmartPtr_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    Vector<Ref<size_t>> vector;
    vector.Reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.PushBack(MakeRef<size_t>(value));
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);
}

BENCHMARK(Vector_SmartPtr_PushBack);

static void Vector_STD_Reference_String_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    std::vector<std::string> vector;
    vector.reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.push_back("Testing123456789");
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);
}

BENCHMARK(Vector_STD_Reference_String_PushBack);

static void Vector_String_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  for (auto _ : state)
  {
    Vector<std::string> vector;
    vector.Reserve(1);
    benchmark::DoNotOptimize(vector.data());
    vector.PushBack("Testing123456789");
    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);
}

BENCHMARK(Vector_String_PushBack);

static void Vector_STD_Reference_Consecutive_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    std::vector<size_t> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(value);
      benchmark::DoNotOptimize(vector.data());
    }

    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Reference_Consecutive_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_Consecutive_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Vector<size_t> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.PushBack(value);
      benchmark::DoNotOptimize(vector.data());
    }

    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_Consecutive_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void Vector_STD_Reference_Consecutive_SmartPtr_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    std::vector<Ref<size_t>> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.push_back(MakeRef<size_t>(value));
      benchmark::DoNotOptimize(vector.data());
    }

    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_STD_Reference_Consecutive_SmartPtr_PushBack)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Complexity(benchmark::oN);

static void Vector_Consecutive_SmartPtr_PushBack(benchmark::State& state)
{
  std::random_device device;
  size_t value = device.operator()();

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Vector<Ref<size_t>> vector;

    for (size_t i = 0; i < amount; i++)
    {
      vector.PushBack(MakeRef<size_t>(value));
      benchmark::DoNotOptimize(vector.data());
    }

    benchmark::ClobberMemory();
  }

  benchmark::DoNotOptimize(value);

  state.SetComplexityN(amount);
}

BENCHMARK(Vector_Consecutive_SmartPtr_PushBack)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

} // namespace genebits::engine::bench