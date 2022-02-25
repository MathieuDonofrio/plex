#include "genebits/engine/ecs/phase.h"

#include <benchmark/benchmark.h>

#include "common/fake_work.h"
#include "genebits/engine/parallel/sync_wait.h"

namespace genebits::engine::bench
{
namespace
{
  template<typename... Components>
  class TestSystem : public System<Components...>
  {
  public:
    Task<> OnUpdate() override
    {
      benchmark::ClobberMemory();

      co_return;
    }
  };

  template<typename... Components>
  class TestSystemWithWork : public System<Components...>
  {
  public:
    TestSystemWithWork(ThreadPool& pool) : pool_(pool) {}

    Task<> WorkTask()
    {
      co_await pool_.Schedule();

      Work(1000);
    }

    Task<> OnUpdate() override
    {
      co_await WorkTask();

      co_return;
    }

  private:
    ThreadPool& pool_;
  };
} // namespace

static void Phase_RunOverheadNoDeps(benchmark::State& state)
{
  size_t amount = state.range(0);

  Ref<SystemGroup> group = MakeRef<SystemGroup>();

  for (size_t i = 0; i < amount; i++)
  {
    group->Add(MakeRef<TestSystem<>>());
  }

  Phase phase = Phase::Compile(group);

  for (auto _ : state)
  {
    SyncWait(phase.Run());
  }

  benchmark::DoNotOptimize(phase);
  benchmark::DoNotOptimize(group);

  state.SetComplexityN(amount);
}

BENCHMARK(Phase_RunOverheadNoDeps)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

static void Phase_RunOverheadWithDeps(benchmark::State& state)
{
  size_t amount = state.range(0);

  Ref<SystemGroup> group = MakeRef<SystemGroup>();

  for (size_t i = 0; i < amount; i++)
  {
    group->Add(MakeRef<TestSystem<int>>());
  }

  Phase phase = Phase::Compile(group);

  for (auto _ : state)
  {
    SyncWait(phase.Run());
  }

  benchmark::DoNotOptimize(phase);
  benchmark::DoNotOptimize(group);

  state.SetComplexityN(amount);
}

BENCHMARK(Phase_RunOverheadWithDeps)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

static void Phase_RunOverheadNoDepsWithWork(benchmark::State& state)
{
  size_t amount = state.range(0);

  ThreadPool pool;

  Ref<SystemGroup> group = MakeRef<SystemGroup>();

  for (size_t i = 0; i < amount; i++)
  {
    group->Add(MakeRef<TestSystemWithWork<>>(pool));
  }

  Phase phase = Phase::Compile(group);

  for (auto _ : state)
  {
    SyncWait(phase.Run());
  }

  benchmark::DoNotOptimize(phase);
  benchmark::DoNotOptimize(group);

  state.SetComplexityN(amount);
}

BENCHMARK(Phase_RunOverheadNoDepsWithWork)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

static void Phase_RunOverheadWithDepsWithWork(benchmark::State& state)
{
  size_t amount = state.range(0);

  ThreadPool pool;

  Ref<SystemGroup> group = MakeRef<SystemGroup>();

  for (size_t i = 0; i < amount; i++)
  {
    group->Add(MakeRef<TestSystemWithWork<int>>(pool));
  }

  Phase phase = Phase::Compile(group);

  for (auto _ : state)
  {
    SyncWait(phase.Run());
  }

  benchmark::DoNotOptimize(phase);
  benchmark::DoNotOptimize(group);

  state.SetComplexityN(amount);
}

BENCHMARK(Phase_RunOverheadWithDepsWithWork)->Arg(10)->Arg(100)->Arg(1000)->Complexity();
} // namespace genebits::engine::bench