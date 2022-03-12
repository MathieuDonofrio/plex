#include "genebits/engine/utilities/delegate.h"

#include <functional>

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
namespace
{
  struct TestEvent
  {
    volatile size_t value;
  };

  struct TestListener
  {
    void listenOverhead(const TestEvent& event)
    {
      sum += event.value * event.value;
    }

    void listenZeroOverhead(const TestEvent&) {}

    size_t sum;
  };
} // namespace

static void Delegate_STD_FunctionBind_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    std::function<void(TestEvent&)> handler = std::bind(&TestListener::listenOverhead, &listener, _1);

    benchmark::DoNotOptimize(handler);
  }
}

BENCHMARK(Delegate_STD_FunctionBind_Construct);

static void Delegate_STD_FunctionBind_Invoke_Overhead(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  std::function<void(TestEvent&)> handler = std::bind(&TestListener::listenOverhead, &listener, _1);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(Delegate_STD_FunctionBind_Invoke_Overhead);

static void Delegate_STD_FunctionBind_Invoke_ZeroOverhead(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  std::function<void(TestEvent&)> handler = std::bind(&TestListener::listenZeroOverhead, &listener, _1);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(Delegate_STD_FunctionBind_Invoke_ZeroOverhead);

static void Delegate_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    Delegate<void(const TestEvent&)> handler;
    handler.Bind<TestListener, &TestListener::listenOverhead>(&listener);

    benchmark::DoNotOptimize(handler);
  }

  benchmark::DoNotOptimize(listener);
}

BENCHMARK(Delegate_Construct);

static void Delegate_Invoke_Overhead(benchmark::State& state)
{
  TestListener listener;

  Delegate<void(const TestEvent&)> handler;
  handler.Bind<TestListener, &TestListener::listenOverhead>(&listener);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler.Invoke(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(Delegate_Invoke_Overhead);

static void Delegate_Invoke_ZeroOverhead(benchmark::State& state)
{
  TestListener listener;

  Delegate<void(const TestEvent&)> handler;
  handler.Bind<TestListener, &TestListener::listenZeroOverhead>(&listener);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler.Invoke(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(Delegate_Invoke_ZeroOverhead);

} // namespace genebits::engine::bench