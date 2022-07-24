#include "genebits/engine/async/thread_pool.h"

#include <future>

#include <benchmark/benchmark.h>

#include "common/fake_work.h"
#include "genebits/engine/async/sync_wait.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/async/when_all.h"
#include "genebits/engine/containers/vector.h"

namespace plex::bench
{
namespace
{
  Task<> CreateTask(ThreadPool& pool, size_t work)
  {
    co_await pool.Schedule();

    Work(work);
  }
} // namespace

static void ThreadPool_STD_Reference_ThreadCreation(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::thread thread { [&]() { Work(0); } };

    benchmark::DoNotOptimize(thread);
    benchmark::ClobberMemory();

    thread.join();
  }
}

BENCHMARK(ThreadPool_STD_Reference_ThreadCreation)
  ->Unit(benchmark::kNanosecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void ThreadPool_STD_Reference_Async_Wait_NoWork(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto future = std::async(std::launch::async, [&]() { Work(0); });

    future.wait();

    benchmark::DoNotOptimize(future);
  }
}

BENCHMARK(ThreadPool_STD_Reference_Async_Wait_NoWork)
  ->Unit(benchmark::kNanosecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

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

BENCHMARK(ThreadPool_Schedule_Wait_NoWork)->Unit(benchmark::kNanosecond)->MeasureProcessCPUTime()->UseRealTime();

static void ThreadPool_Reference_SingleThread_AllWorkInSingleTask(benchmark::State& state)
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

BENCHMARK(ThreadPool_Reference_SingleThread_AllWorkInSingleTask)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void ThreadPool_STD_Reference_Async_WorkDividedInLewLargeTasks(benchmark::State& state)
{
  // STD Async can use a thread pool under the hood.
  // MSVC does this but not GCC and CLANG
  // The goal is for our thread pool to be about the same performance as MSVC, this would mean
  // we would have an efficient cross-platform thread pool.

  const size_t amount = state.range(0);

  size_t threads = std::thread::hardware_concurrency();

  auto work_per_thread = static_cast<unsigned int>(amount / threads);

  for (auto _ : state)
  {
    Vector<std::future<void>> tasks;
    tasks.resize(threads);

    for (size_t i = 0; i < threads; i++)
    {
      tasks[i] = std::async(std::launch::async, [work_per_thread]() { Work(1000 * work_per_thread); });
    }

    benchmark::DoNotOptimize(tasks.data());
    benchmark::ClobberMemory();

    for (auto& task : tasks)
    {
      task.wait();
    }
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_STD_Reference_Async_WorkDividedInLewLargeTasks)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void ThreadPool_Schedule_WorkDividedInFewLargeTasks(benchmark::State& state)
{
  ThreadPool pool;

  const size_t amount = state.range(0);

  auto work_per_thread = static_cast<unsigned int>(amount / pool.ThreadCount());

  for (auto _ : state)
  {
    Vector<Task<>> tasks;
    tasks.reserve(amount);

    benchmark::DoNotOptimize(tasks.data());

    for (size_t i = 0; i < pool.ThreadCount(); i++)
    {
      tasks.push_back(CreateTask(pool, 1000 * work_per_thread));
    }

    benchmark::ClobberMemory();

    Task<> task = WhenAll(std::move(tasks));

    SyncWait(task);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_Schedule_WorkDividedInFewLargeTasks)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void ThreadPool_STD_Reference_Async_WorkDividedInManySmallTasks(benchmark::State& state)
{
  const size_t amount = state.range(0);

  auto executor = []() { Work(1000); };

  for (auto _ : state)
  {
    Vector<std::future<void>> tasks;
    tasks.resize(amount);

    for (size_t i = 0; i < amount; i++)
    {
      tasks[i] = std::async(std::launch::async, executor);
    }

    benchmark::DoNotOptimize(tasks.data());
    benchmark::ClobberMemory();

    for (auto& task : tasks)
    {
      task.wait();
    }
  }

  benchmark::DoNotOptimize(executor);

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_STD_Reference_Async_WorkDividedInManySmallTasks)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void ThreadPool_Schedule_WorkDividedInManySmallTasks(benchmark::State& state)
{
  ThreadPool pool;

  const size_t amount = state.range(0);

  for (auto _ : state)
  {
    Vector<Task<>> tasks;
    tasks.reserve(amount);

    benchmark::DoNotOptimize(tasks.data());

    for (size_t i = 0; i < amount; i++)
    {
      tasks.push_back(CreateTask(pool, 1000));
    }

    benchmark::ClobberMemory();

    Task<> task = WhenAll(std::move(tasks));

    SyncWait(task);
  }

  state.SetComplexityN(amount);
}

BENCHMARK(ThreadPool_Schedule_WorkDividedInManySmallTasks)
  ->Arg(100)
  ->Arg(1000)
  ->Arg(10000)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

} // namespace plex::bench