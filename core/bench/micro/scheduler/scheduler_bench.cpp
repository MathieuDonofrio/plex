#include "plex/scheduler/scheduler.h"

#include <benchmark/benchmark.h>

#include "micro/common/fake_work.h"
#include "plex/async/sync_wait.h"
#include "plex/async/thread_pool.h"

namespace plex::bench
{
namespace
{
  ThreadPool thread_pool;

  template<size_t id>
  struct MockData
  {
    size_t value;
  };

  template<size_t id>
  struct MockStage
  {};

  template<typename... Components>
  struct MockQuery
  {
    static MockQuery Fetch(void*, Context&, Context&)
    {
      return MockQuery();
    }

    static consteval std::array<QueryDataAccess, sizeof...(Components)> GetDataAccess() noexcept
    {
      return { QueryDataAccess {
        "mock", TypeName<Components>(), std::is_const_v<Components>, IsThreadSafe<Components>::value }... };
    }
  };

  template<size_t id, Query... queries>
  void SystemMock([[maybe_unused]] queries... q)
  {
    Work(10000);
  }

  template<size_t id, Query... queries>
  Task<void> AsyncSystemMock([[maybe_unused]] queries... q)
  {
    co_await thread_pool.Schedule();
    Work(10000);
  }

  template<size_t StageId, size_t Systems, bool Async, size_t I = Systems>
  struct SystemGenerator
  {
    static void AddSystems(Scheduler& scheduler)
    {
      if constexpr (Async)
        scheduler.template AddSystem<MockStage<StageId>>(
          AsyncSystemMock<StageId * Systems + I, MockQuery<MockData<I>>>);
      else
        scheduler.template AddSystem<MockStage<StageId>>(SystemMock<StageId * Systems + I, MockQuery<MockData<I>>>);
      if constexpr (I > 1) SystemGenerator<StageId, Systems, Async, I - 1>::AddSystems(scheduler);
    }

    static void AddSystems(Stage& stage)
    {
      if constexpr (Async) stage.AddSystem(AsyncSystemMock<StageId * Systems + I, MockQuery<MockData<I>>>);
      else
        stage.AddSystem(SystemMock<StageId * Systems + I, MockQuery<MockData<I>>>);
      if constexpr (I > 1) SystemGenerator<StageId, Systems, Async, I - 1>::AddSystems(stage);
    }
  };

  template<size_t Stages, size_t SystemsPerStage, bool Async, size_t I = Stages>
  struct StageGenerator
  {
    static void AddSystems(Scheduler& scheduler)
    {
      SystemGenerator<I, SystemsPerStage, Async>::AddSystems(scheduler);
      if constexpr (I > 1) StageGenerator<Stages, SystemsPerStage, Async, I - 1>::AddSystems(scheduler);
    }

    static void MakeStages(Vector<std::unique_ptr<Stage>>& stages)
    {
      stages.push_back(std::make_unique<Stage>());
      SystemGenerator<I, SystemsPerStage, Async>::AddSystems(*stages.back().get());
      if constexpr (I > 1) StageGenerator<Stages, SystemsPerStage, Async, I - 1>::MakeStages(stages);
    }
  };

  template<size_t Stages, size_t I = Stages>
  struct StageScheduler
  {
    static void ScheduleStages(Scheduler& scheduler)
    {
      scheduler.template Schedule<MockStage<I>>();
      if constexpr (I > 1) StageScheduler<Stages, I - 1>::ScheduleStages(scheduler);
    }
  };

} // namespace

static void Scheduler_ComputeSchedulerSteps_5Stages4SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 5;
  static constexpr size_t SystemsPerStage = 4;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  Vector<std::unique_ptr<Stage>> stages;

  StageGenerator<Stages, SystemsPerStage, Async>::MakeStages(stages);

  Vector<Stage*> raw_ptr_stages;

  for (auto& stage : stages)
  {
    raw_ptr_stages.push_back(stage.get());
  }

  for (auto _ : state)
  {
    auto steps = ComputeSchedulerData(raw_ptr_stages);

    benchmark::DoNotOptimize(steps);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_ComputeSchedulerSteps_5Stages4SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

// static void Scheduler_ComputeSchedulerSteps_10Stages8SystemsEach(benchmark::State& state)
//{
//   static constexpr size_t Stages = 10;
//   static constexpr size_t SystemsPerStage = 8;
//   static constexpr bool Async = false;
//
//   Context context;
//
//   Scheduler scheduler;
//
//   Vector<std::unique_ptr<Stage>> stages;
//
//   StageGenerator<Stages, SystemsPerStage, Async>::MakeStages(stages);
//
//   Vector<Stage*> raw_ptr_stages;
//
//   for (auto& stage : stages)
//   {
//     raw_ptr_stages.push_back(stage.get());
//   }
//
//   for (auto _ : state)
//   {
//     auto steps = ComputeSchedulerData(raw_ptr_stages);
//
//     benchmark::DoNotOptimize(steps);
//   }
//
//   benchmark::DoNotOptimize(scheduler);
// }
//
// BENCHMARK(Scheduler_ComputeSchedulerSteps_10Stages8SystemsEach)
//   ->Unit(benchmark::kMillisecond)
//   ->MeasureProcessCPUTime()
//   ->UseRealTime();

static void Scheduler_Reference_5Stages4SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 5;
  static constexpr size_t SystemsPerStage = 4;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  Vector<std::unique_ptr<Stage>> stages;

  StageGenerator<Stages, SystemsPerStage, Async>::MakeStages(stages);

  Vector<Stage*> raw_ptr_stages;

  for (auto& stage : stages)
  {
    raw_ptr_stages.push_back(stage.get());
  }

  auto steps = ComputeSchedulerData(raw_ptr_stages);

  for (auto _ : state)
  {
    for (auto& step : steps)
    {
      auto task = (*step.system)(context);

      task.Eject();

      benchmark::DoNotOptimize(task);
    }
  }

  benchmark::DoNotOptimize(steps);

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_Reference_5Stages4SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_SyncWork_5Stages4SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 5;
  static constexpr size_t SystemsPerStage = 4;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_SyncWork_5Stages4SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_AsyncWork_5Stages4SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 5;
  static constexpr size_t SystemsPerStage = 4;
  static constexpr bool Async = true;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_AsyncWork_5Stages4SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_Reference_10Stages8SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 8;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  Vector<std::unique_ptr<Stage>> stages;

  StageGenerator<Stages, SystemsPerStage, Async>::MakeStages(stages);

  Vector<Stage*> raw_ptr_stages;

  for (auto& stage : stages)
  {
    raw_ptr_stages.push_back(stage.get());
  }

  auto steps = ComputeSchedulerData(raw_ptr_stages);

  for (auto _ : state)
  {
    for (auto& step : steps)
    {
      auto task = (*step.system)(context);

      task.Eject();

      benchmark::DoNotOptimize(task);
    }
  }

  benchmark::DoNotOptimize(steps);

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_Reference_10Stages8SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_SyncWork_10Stages8SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 8;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_SyncWork_10Stages8SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_AsyncWork_10Stages8SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 8;
  static constexpr bool Async = true;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_AsyncWork_10Stages8SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_Reference_10Stages16SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 16;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  Vector<std::unique_ptr<Stage>> stages;

  StageGenerator<Stages, SystemsPerStage, Async>::MakeStages(stages);

  Vector<Stage*> raw_ptr_stages;

  for (auto& stage : stages)
  {
    raw_ptr_stages.push_back(stage.get());
  }

  auto steps = ComputeSchedulerData(raw_ptr_stages);

  for (auto _ : state)
  {
    for (auto& step : steps)
    {
      auto task = (*step.system)(context);

      task.Eject();

      benchmark::DoNotOptimize(task);
    }
  }

  benchmark::DoNotOptimize(steps);

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_Reference_10Stages16SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_SyncWork_10Stages16SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 16;
  static constexpr bool Async = false;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_SyncWork_10Stages16SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();

static void Scheduler_AsyncWork_10Stages16SystemsEach(benchmark::State& state)
{
  static constexpr size_t Stages = 10;
  static constexpr size_t SystemsPerStage = 16;
  static constexpr bool Async = true;

  Context context;

  Scheduler scheduler;

  StageGenerator<Stages, SystemsPerStage, Async>::AddSystems(scheduler);

  // Run once to cache results
  StageScheduler<Stages>::ScheduleStages(scheduler);
  SyncWait(scheduler.RunAll(context));

  for (auto _ : state)
  {
    StageScheduler<Stages>::ScheduleStages(scheduler);

    auto task = scheduler.RunAll(context);

    benchmark::DoNotOptimize(task);

    SyncWait(task);
  }

  benchmark::DoNotOptimize(scheduler);
}

BENCHMARK(Scheduler_AsyncWork_10Stages16SystemsEach)
  ->Unit(benchmark::kMillisecond)
  ->MeasureProcessCPUTime()
  ->UseRealTime();
} // namespace plex::bench