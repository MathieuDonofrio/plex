#include "genebits/engine/parallel/thread_pool.h"

#include <cmath>
#include <future>

#include <benchmark/benchmark.h>

#include "common/fake_work.h"
#include "genebits/engine/parallel/sync_wait.h"
#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/when_all.h"
#include "genebits/engine/util/fast_vector.h"

namespace genebits::engine::bench
{
namespace
{
  Task<> CreateTask(ThreadPool& pool, size_t work)
  {
    co_await pool.Schedule();

    Work(work);
  }
} // namespace

static void ThreadPool_Schedule_Wait_NoWork(benchmark::State& state)
{
  ThreadPool pool;

  for (auto _ : state)
  {
    auto task = CreateTask(pool, 0);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }
}

BENCHMARK(ThreadPool_Schedule_Wait_NoWork);

static void ThreadPool_STD_ThreadCreation(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::thread thread { [&]() { Work(0); } };

    thread.join();

    benchmark::DoNotOptimize(thread);
  }
}

BENCHMARK(ThreadPool_STD_ThreadCreation);

static void ThreadPool_STD_Async_NoWork(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto future = std::async(std::launch::async, [&]() { Work(0); });

    future.wait();

    benchmark::DoNotOptimize(future);
  }
}

BENCHMARK(ThreadPool_STD_Async_NoWork);

static void ThreadPool_NoSchedule_SingleThread_Reference(benchmark::State& state)
{
  // This is to be used as a reference of single thread performance with no schedule overhead
  // Conclusion: For small loads it is better to not use the thread pool because scheduling is expensive.

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Work(1000 * amount);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_NoSchedule_SingleThread_Reference)->Arg(100)->Arg(1000)->Arg(10000)->Complexity();

static void ThreadPool_Schedule_FewLargeTasks(benchmark::State& state)
{
  constexpr size_t threads = 4;

  ThreadPool pool(threads, true);

  const size_t amount = state.range(0);

  auto work_per_thread = static_cast<unsigned int>(amount / threads);

  for (auto _ : state)
  {
    auto task = [&]() -> Task<> {
      co_await WhenAll(CreateTask(pool, 1000 * work_per_thread),
        CreateTask(pool, 1000 * work_per_thread),
        CreateTask(pool, 1000 * work_per_thread),
        CreateTask(pool, 1000 * work_per_thread));
    }();

    SyncWait(task);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_Schedule_FewLargeTasks)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void ThreadPool_Schedule_ManySmallTasks(benchmark::State& state)
{
  ThreadPool pool;

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    state.PauseTiming();

    FastVector<Task<>> tasks;
    tasks.Reserve(amount);

    benchmark::DoNotOptimize(tasks);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      tasks.PushBack(CreateTask(pool, 1000));
    }

    Task<> task = WhenAll(std::move(tasks));

    SyncWait(task);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_Schedule_ManySmallTasks)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void ThreadPool_STD_Async_LewLargeTasks(benchmark::State& state)
{
  // STD Async can use a thread pool under the hood.
  // MSVC does this but not GCC and CLANG
  // The goal is for our thread pool to be about the same performance as MSVC, this would mean
  // we would have an efficient cross-platform thread pool.

  constexpr size_t threads = 4;

  const size_t amount = state.range(0);

  auto work_per_thread = static_cast<unsigned int>(amount / threads);

  auto executor = [work_per_thread]() { Work(1000 * work_per_thread); };

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

  benchmark::DoNotOptimize(executor);

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_STD_Async_LewLargeTasks)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

static void ThreadPool_STD_Async_ManySmallTasks(benchmark::State& state)
{
  const size_t amount = state.range(0);

  auto executor = []() { Work(1000); };

  for (auto _ : state)
  {
    state.PauseTiming();

    FastVector<std::future<void>> tasks;
    tasks.Resize(amount);

    benchmark::DoNotOptimize(tasks);

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      tasks[i] = std::async(std::launch::async, executor);
    }

    for (size_t i = 0; i < amount; i++)
    {
      tasks[i].get();
    }
  }

  benchmark::DoNotOptimize(executor);

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_STD_Async_ManySmallTasks)->Arg(100)->Arg(1000)->Arg(10000)->Complexity(benchmark::oN);

} // namespace genebits::engine::bench