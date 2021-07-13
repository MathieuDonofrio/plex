#include "engine/util/concurrency.h"

#include <mutex>

#include <benchmark/benchmark.h>

namespace genebits::engine
{
static void Concurrency_STD_Mutex_LockUnlock(benchmark::State& state)
{
  std::mutex mutex;

  float sum = 0;

  for (auto _ : state)
  {
    mutex.lock();

    sum = sum * 2 + sum + 4;

    mutex.unlock();
  }

  benchmark::DoNotOptimize(mutex);
}

BENCHMARK(Concurrency_STD_Mutex_LockUnlock)->Threads(16);

static void Concurrency_SpinMutex_LockUnlock(benchmark::State& state)
{
  SpinMutex mutex;

  float sum = 0;

  for (auto _ : state)
  {
    mutex.lock();

    sum = sum * 2 + sum + 4;

    mutex.unlock();
  }

  benchmark::DoNotOptimize(mutex);
}

BENCHMARK(Concurrency_SpinMutex_LockUnlock)->Threads(16);

} // namespace genebits::engine