#include "genebits/engine/parallel/job.h"

#include <benchmark/benchmark.h>

#include "fake_work.h"

#include <functional>

namespace genebits::engine::bench
{
namespace
{
  class BenchBasicJob : public BasicJob<BenchBasicJob>
  {
  public:
    BenchBasicJob(std::function<void()> function) : function_(function) {}

    void operator()()
    {
      function_();
    }

  private:
    std::function<void()> function_;
  };

  class BenchParallelForJob : public ParallelForJob<BenchParallelForJob>
  {
  public:
    BenchParallelForJob(std::function<void(size_t)> function, size_t amount)
      : ParallelForJob<BenchParallelForJob>(amount), function_(function)
    {}

    void operator()(size_t index)
    {
      function_(index);
    }

  private:
    std::function<void(size_t index)> function_;
  };
} // namespace

void BasicJob_ScheduleAndComplete_NoWork(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  for (auto _ : state)
  {
    auto job = MakeRef<BenchBasicJob>(
      [&t]()
      {
        t++;
        benchmark::ClobberMemory();
      });

    JobHandle handle = scheduler.Schedule(job);

    scheduler.Complete(handle);

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);
}

BENCHMARK(BasicJob_ScheduleAndComplete_NoWork);

void BasicJob_ScheduleAndComplete_Work(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  const size_t amount = state.range(0);

  const size_t iterations = 1;
  const size_t work_per_iteration = amount / iterations;

  for (auto _ : state)
  {
    auto job = MakeRef<BenchBasicJob>(
      [&t, work_per_iteration]()
      {
        Work(work_per_iteration);
        t++;
        benchmark::ClobberMemory();
      });

    JobHandle handle = scheduler.Schedule(job);

    scheduler.Complete(handle);

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);

  state.SetComplexityN(amount);
}

BENCHMARK(BasicJob_ScheduleAndComplete_Work)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

void ParallelForJob_ScheduleAndComplete_NoWork(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  for (auto _ : state)
  {
    auto job = MakeRef<BenchParallelForJob>(
      [&t](size_t index)
      {
        t += index;
        benchmark::ClobberMemory();
      },
      16);

    JobHandle handle = scheduler.Schedule(job);

    scheduler.Complete(handle);

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);
}

BENCHMARK(ParallelForJob_ScheduleAndComplete_NoWork);

void ParallelForJob_ScheduleAndComplete_Work(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  const size_t amount = state.range(0);

  const size_t iterations = 50;
  const size_t work_per_iteration = amount / iterations;

  for (auto _ : state)
  {
    auto job = MakeRef<BenchParallelForJob>(
      [&t, work_per_iteration](size_t index)
      {
        Work(work_per_iteration);
        t += index;
        benchmark::ClobberMemory();
      },
      iterations);

    JobHandle handle = scheduler.Schedule(job);

    scheduler.Complete(handle);

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);

  state.SetComplexityN(amount);
}

BENCHMARK(ParallelForJob_ScheduleAndComplete_Work)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

} // namespace genebits::engine::bench