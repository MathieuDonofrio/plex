#include "engine/events/event_handler.h"

#include <functional>

#include <benchmark/benchmark.h>

namespace genebits::engine
{
namespace
{
  struct TestEvent
  {
    volatile size_t value;
  };

  struct TestListener
  {
    void listen(const TestEvent&)
    {
    }
  };
} // namespace

static void EventHandler_STD_FunctionBind_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    std::function<void(TestEvent&)> handler = std::bind(&TestListener::listen, &listener, _1);

    benchmark::DoNotOptimize(handler);
  }
}

BENCHMARK(EventHandler_STD_FunctionBind_Construct);

static void EventHandler_STD_FunctionBind_Invoke(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  std::function<void(TestEvent&)> handler = std::bind(&TestListener::listen, &listener, _1);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(EventHandler_STD_FunctionBind_Invoke);

static void EventHandler_Construct(benchmark::State& state)
{
  using namespace std::placeholders;

  TestListener listener;

  for (auto _ : state)
  {
    EventHandler<TestEvent> handler;
    handler.Bind<&TestListener::listen>(&listener);

    benchmark::DoNotOptimize(handler);
  }

  benchmark::DoNotOptimize(listener);
}

BENCHMARK(EventHandler_Construct);

static void EventHandler_Invoke(benchmark::State& state)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<&TestListener::listen>(&listener);

  TestEvent event { static_cast<size_t>(std::rand()) };

  for (auto _ : state)
  {
    handler.Invoke(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(listener);
}

BENCHMARK(EventHandler_Invoke);

} // namespace genebits::engine