#include "engine/events/event_bus.h"

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
    void listen(const TestEvent& event)
    {
      sum += event.value * event.value;
    }

    size_t sum;
  };
} // namespace

static void EventBus_SubscribeUnsubscribe(benchmark::State& state)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<&TestListener::listen>(&listener);

  for (auto _ : state)
  {
    EventBus bus;

    bus.Subscribe(handler);
    bus.Unsubscribe(handler);

    benchmark::DoNotOptimize(bus);
  }

  benchmark::DoNotOptimize(listener);
  benchmark::DoNotOptimize(handler);
}

BENCHMARK(EventBus_SubscribeUnsubscribe);

static void EventBus_SubscribeUnsubscribe_10(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  TestListener listeners[cAmount];
  EventHandler<TestEvent> handlers[cAmount];

  for (size_t i = 0; i < cAmount; i++)
  {
    handlers[i].Bind<&TestListener::listen>(&listeners[i]);
  }

  for (auto _ : state)
  {
    EventBus bus;

    for (size_t i = 0; i < cAmount; i++)
    {
      bus.Subscribe(handlers[i]);
    }

    for (size_t i = 0; i < cAmount; i++)
    {
      bus.Unsubscribe(handlers[i]);
    }

    benchmark::DoNotOptimize(bus);
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    benchmark::DoNotOptimize(listeners[i]);
    benchmark::DoNotOptimize(handlers[i]);
  }
}

BENCHMARK(EventBus_SubscribeUnsubscribe_10);

static void EventBus_SubscribeUnsubscribe_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  TestListener listeners[cAmount];
  EventHandler<TestEvent> handlers[cAmount];

  for (size_t i = 0; i < cAmount; i++)
  {
    handlers[i].Bind<&TestListener::listen>(&listeners[i]);
  }

  for (auto _ : state)
  {
    EventBus bus;

    for (size_t i = 0; i < cAmount; i++)
    {
      bus.Subscribe(handlers[i]);
    }

    for (size_t i = 0; i < cAmount; i++)
    {
      bus.Unsubscribe(handlers[i]);
    }

    benchmark::DoNotOptimize(bus);
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    benchmark::DoNotOptimize(listeners[i]);
    benchmark::DoNotOptimize(handlers[i]);
  }
}

BENCHMARK(EventBus_SubscribeUnsubscribe_100);

static void EventBus_Publish(benchmark::State& state)
{
  EventBus bus;

  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<&TestListener::listen>(&listener);

  bus.Subscribe(handler);

  TestEvent event;

  for (auto _ : state)
  {
    bus.Publish(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(bus);
  benchmark::DoNotOptimize(listener);
  benchmark::DoNotOptimize(handler);
}

BENCHMARK(EventBus_Publish);

static void EventBus_Publish_10(benchmark::State& state)
{
  constexpr size_t cAmount = 10;

  EventBus bus;

  TestListener listeners[cAmount];
  EventHandler<TestEvent> handlers[cAmount];

  for (size_t i = 0; i < cAmount; i++)
  {
    handlers[i].Bind<&TestListener::listen>(&listeners[i]);
    bus.Subscribe(handlers[i]);
  }

  TestEvent event;

  for (auto _ : state)
  {
    bus.Publish(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(bus);

  for (size_t i = 0; i < cAmount; i++)
  {
    benchmark::DoNotOptimize(listeners[i]);
    benchmark::DoNotOptimize(handlers[i]);
  }
}

BENCHMARK(EventBus_Publish_10);

static void EventBus_Publish_100(benchmark::State& state)
{
  constexpr size_t cAmount = 100;

  EventBus bus;

  TestListener listeners[cAmount];
  EventHandler<TestEvent> handlers[cAmount];

  for (size_t i = 0; i < cAmount; i++)
  {
    handlers[i].Bind<&TestListener::listen>(&listeners[i]);
    bus.Subscribe(handlers[i]);
  }

  TestEvent event;

  for (auto _ : state)
  {
    bus.Publish(event);
  }

  benchmark::DoNotOptimize(event);
  benchmark::DoNotOptimize(bus);

  for (size_t i = 0; i < cAmount; i++)
  {
    benchmark::DoNotOptimize(listeners[i]);
    benchmark::DoNotOptimize(handlers[i]);
  }
}

BENCHMARK(EventBus_Publish_100);

} // namespace genebits::engine