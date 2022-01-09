#include "genebits/engine/events/event_bus.h"

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
    void listen(const TestEvent& event)
    {
      sum += event.value * event.value;
    }

    size_t sum;
  };
} // namespace

static void EventBus_Subscribe(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  std::vector<EventHandler<TestEvent>> handlers;

  for (size_t i = 0; i < amount; i++)
  {
    TestListener listener;
    EventHandler<TestEvent> handler;

    handler.Bind<TestListener, &TestListener::listen>(&listener);

    handlers.push_back(handler);
  }

  for (auto _ : state)
  {
    state.PauseTiming();

    EventBus bus;

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      bus.Subscribe(handlers[i]);
    }

    benchmark::DoNotOptimize(bus);
  }

  for (size_t i = 0; i < amount; i++)
  {
    benchmark::DoNotOptimize(handlers[i]);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));
}

BENCHMARK(EventBus_Subscribe)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

static void EventBus_Unsubscribe(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  std::vector<EventHandler<TestEvent>> handlers;

  for (size_t i = 0; i < amount; i++)
  {
    TestListener listener;
    EventHandler<TestEvent> handler;

    handler.Bind<TestListener, &TestListener::listen>(&listener);

    handlers.push_back(handler);
  }

  for (auto _ : state)
  {
    state.PauseTiming();

    EventBus bus;

    for (size_t i = 0; i < amount; i++)
    {
      bus.Subscribe(handlers[i]);
    }

    state.ResumeTiming();

    for (size_t i = 0; i < amount; i++)
    {
      bus.Unsubscribe(handlers[i]);
    }

    benchmark::DoNotOptimize(bus);
  }

  for (size_t i = 0; i < amount; i++)
  {
    benchmark::DoNotOptimize(handlers[i]);
  }

  state.SetComplexityN(static_cast<int64_t>(amount));
}

BENCHMARK(EventBus_Unsubscribe)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

static void EventBus_Publish(benchmark::State& state)
{
  auto amount = static_cast<size_t>(state.range(0));

  EventBus bus;

  for (size_t i = 0; i < amount; i++)
  {
    TestListener listener;
    EventHandler<TestEvent> handler;

    handler.Bind<TestListener, &TestListener::listen>(&listener);

    bus.Subscribe(handler);
  }

  TestEvent event;

  for (auto _ : state)
  {
    bus.Publish(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(bus);

  state.SetComplexityN(static_cast<int64_t>(amount));
}

BENCHMARK(EventBus_Publish)->Arg(10)->Arg(100)->Arg(1000)->Complexity();

} // namespace genebits::engine::bench