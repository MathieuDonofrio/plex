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

static void Concurrency_SpinLock_LockUnlock(benchmark::State& state)
{
  SpinLock spin_lock;

  float sum = 0;

  for (auto _ : state)
  {
    spin_lock.lock();

    sum = sum * 2 + sum + 4;

    spin_lock.unlock();
  }

  benchmark::DoNotOptimize(spin_lock);
}

BENCHMARK(Concurrency_SpinLock_LockUnlock)->Threads(16);

} // namespace genebits::engine