#include "genebits/engine/jobs/thread_pool.h"

#include <array>
#include <cmath>
#include <future>

#include <benchmark/benchmark.h>

namespace genebits::engine
{

void Work()
{
  uint64_t seed = static_cast<uint64_t>(time(nullptr));

  benchmark::DoNotOptimize(seed);

  benchmark::ClobberMemory();

  uint64_t state = seed;

  for (size_t i = 0; i < 1000; i++)
  {
    auto old = state;

    state = old * 1664525 + 1013904223;

    state ^= old;

    benchmark::DoNotOptimize(state);
  }

  benchmark::DoNotOptimize(state);

  benchmark::ClobberMemory();
}

static void ThreadPool_SingleThread_Execute(benchmark::State& state)
{
  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    for (size_t i = 0; i < amount; i++)
    {
      Work();
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_SingleThread_Execute)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

static void ThreadPool_4Threads_Execute(benchmark::State& state)
{
  constexpr size_t threads = 4;

  ThreadPool pool(threads);

  const size_t amount = state.range(0);

  auto work_per_thread = static_cast<unsigned int>(amount / threads);

  auto executor = [work_per_thread]()
  {
    for (size_t i = 0; i < work_per_thread; i++)
    {
      Work();
    }
  };

  for (auto _ : state)
  {
    Task task1;
    task1.Bind(executor);
    pool.Execute(&task1);
    Task task2;
    task2.Bind(executor);
    pool.Execute(&task2);
    Task task3;
    task3.Bind(executor);
    pool.Execute(&task3);
    Task task4;
    task4.Bind(executor);
    pool.Execute(&task4);

    task1.Wait();
    task2.Wait();
    task3.Wait();
    task4.Wait();

    benchmark::DoNotOptimize(task1);
    benchmark::DoNotOptimize(task2);
    benchmark::DoNotOptimize(task3);
    benchmark::DoNotOptimize(task4);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_4Threads_Execute)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

static void ThreadPool_STD_Async4_Execute(benchmark::State& state)
{
  // STD Async can use a thread pool under the hood.
  // MSVC does this but not GCC and CLANG
  // The goal is for our thread pool to be about the same performance as MSVC, this would mean
  // we would have an efficient cross-platform thread pool.

  constexpr size_t threads = 4;

  const size_t amount = state.range(0);

  auto work_per_thread = static_cast<unsigned int>(amount / threads);

  auto executor = [work_per_thread]()
  {
    for (size_t i = 0; i < work_per_thread; i++)
    {
      Work();
    }
  };

  for (auto _ : state)
  {
    auto future1 = std::async(std::launch::async, executor);
    auto future2 = std::async(std::launch::async, executor);
    auto future3 = std::async(std::launch::async, executor);
    auto future4 = std::async(std::launch::async, executor);

    future1.wait();
    future2.wait();
    future3.wait();
    future4.wait();

    benchmark::DoNotOptimize(future1);
    benchmark::DoNotOptimize(future2);
    benchmark::DoNotOptimize(future3);
    benchmark::DoNotOptimize(future4);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_STD_Async4_Execute)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

} // namespace genebits::engine