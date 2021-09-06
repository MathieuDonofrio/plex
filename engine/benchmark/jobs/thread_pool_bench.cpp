#include "genebits/engine/jobs/thread_pool.h"

#include <array>
#include <cmath>

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

  for (auto _ : state)
  {
    std::array<Task, threads> tasks;

    for (auto& task : tasks)
    {
      task.Bind(
        [work_per_thread]()
        {
          for (size_t i = 0; i < work_per_thread; i++)
          {
            Work();
          }
        });

      pool.Execute(task);
    }

    for (const auto& task : tasks)
    {
      task.Wait();
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_4Threads_Execute)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

} // namespace genebits::engine