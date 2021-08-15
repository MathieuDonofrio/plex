#include "genebits/engine/util/fast_vector.h"

#include <benchmark/benchmark.h>

namespace genebits::engine
{
static void FastVector_STD_Vector_Iterate(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  std::vector<size_t> vector;

  for (size_t i = 0; i != cAmount; i++)
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
}

BENCHMARK(FastVector_STD_Vector_Iterate);

static void FastVector_STD_Vector_NonTrivial_Iterate(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  std::vector<std::string> vector;

  for (size_t i = 0; i != cAmount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  for (auto _ : state)
  {
    for (auto& value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);
}

BENCHMARK(FastVector_STD_Vector_NonTrivial_Iterate);

static void FastVector_STD_Vector_PushBack(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::vector<size_t> vector;

    vector.push_back(10);

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_STD_Vector_PushBack);

static void FastVector_STD_Vector_PushBack_10(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  for (auto _ : state)
  {
    std::vector<size_t> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.push_back(i);
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_STD_Vector_PushBack_10);

static void FastVector_STD_Vector_PushBack_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  for (auto _ : state)
  {
    std::vector<size_t> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.push_back(i);
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_STD_Vector_PushBack_100);

static void FastVector_STD_Vector_PushBack_NonTrivial_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  for (auto _ : state)
  {
    std::vector<std::string> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.push_back("TestingString123123");
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_STD_Vector_PushBack_NonTrivial_100);

static void FastVector_Iterate(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  FastVector<size_t> vector;

  for (size_t i = 0; i != cAmount; i++)
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
}

BENCHMARK(FastVector_Iterate);

static void FastVector_NonTrivial_Iterate(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  FastVector<std::string> vector;

  for (size_t i = 0; i != cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  for (auto _ : state)
  {
    for (auto& value : vector)
    {
      benchmark::DoNotOptimize(value);
    }
  }

  benchmark::DoNotOptimize(vector);
}

BENCHMARK(FastVector_NonTrivial_Iterate);

static void FastVector_PushBack(benchmark::State& state)
{
  for (auto _ : state)
  {
    FastVector<size_t> vector;

    vector.PushBack(10);

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_PushBack);

static void FastVector_PushBack_10(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  for (auto _ : state)
  {
    FastVector<size_t> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.PushBack(i);
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_PushBack_10);

static void FastVector_PushBack_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  for (auto _ : state)
  {
    FastVector<size_t> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.PushBack(i);
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_PushBack_100);

static void FastVector_PushBack_NonTrivial_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  for (auto _ : state)
  {
    FastVector<std::string> vector;

    for (size_t i = 0; i != cAmount; i++)
    {
      vector.PushBack("TestingString123123");
    }

    benchmark::DoNotOptimize(vector);
  }
}

BENCHMARK(FastVector_PushBack_NonTrivial_100);

} // namespace genebits::engine