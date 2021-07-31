#include "engine/events/event_handler.h"

#include <functional>

#include <benchmark/benchmark.h>

namespace genebits::engine
{
namespace
{
  struct TestEvent
  {
    double value;
  };

  struct TestListener
  {
    void addValue(const TestEvent&)
    {
      // sum += event.value;
    }

    double sum = 0;
  };
} // namespace

static void EventHandler_STD_FunctionBind_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    std::function<void(TestEvent&)> handler = std::bind(&TestListener::addValue, &listener, _1);

    benchmark::DoNotOptimize(handler);
  }
}

BENCHMARK(EventHandler_STD_FunctionBind_Construct);

static void EventHandler_STD_FunctionBind_Invoke(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  std::function<void(TestEvent&)> handler = std::bind(&TestListener::addValue, &listener, _1);

  for (auto _ : state)
  {
    TestEvent event { 1234.56789 };

    handler(event);
  }

  benchmark::DoNotOptimize(listener.sum);
}

BENCHMARK(EventHandler_STD_FunctionBind_Invoke);

static void EventHandler_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    EventHandler<TestEvent> handler(&TestListener::addValue, &listener);

    benchmark::DoNotOptimize(handler);
  }
}

BENCHMARK(EventHandler_Construct);

static void EventHandler_Invoke(benchmark::State& state)
{
  TestListener listener;

  EventHandler<TestEvent> handler(&TestListener::addValue, &listener);

  for (auto _ : state)
  {
    TestEvent event { 1234.56789 };

    handler.Invoke(event);
  }

  benchmark::DoNotOptimize(listener.sum);
}

BENCHMARK(EventHandler_Invoke);

} // namespace genebits::engine