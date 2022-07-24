#include "genebits/engine/events/event_bus.h"

#include <gtest/gtest.h>

namespace plex::tests
{
namespace
{
  struct TestEvent1
  {
    size_t value;
  };

  size_t sum1_1 = 0;

  void listen1_1(const TestEvent1& event)
  {
    sum1_1 = event.value;
  }

  void listen1_2(const TestEvent1&) {}
} // namespace

TEST(EventBus_Tests, Count_OnConstruction_Zero)
{
  EventBus bus;

  EXPECT_EQ(bus.Count<TestEvent1>(), 0u);
}

TEST(EventBus_Tests, Subscribe_SingleEventType_IncreaseCount)
{
  EventBus bus;

  EventHandler<TestEvent1> handler1_1;
  handler1_1.Bind<&listen1_1>();

  bus.Subscribe<TestEvent1>(handler1_1);

  EXPECT_EQ(bus.Count<TestEvent1>(), 1u);

  EventHandler<TestEvent1> handler1_2;
  handler1_2.Bind<&listen1_2>();

  bus.Subscribe<TestEvent1>(handler1_2);

  EXPECT_EQ(bus.Count<TestEvent1>(), 2u);
}

TEST(EventBus_Tests, Unsubscribe_SingleEventType_DecreaseCount)
{
  EventBus bus;

  EventHandler<TestEvent1> handler1_1;
  handler1_1.Bind<&listen1_1>();

  bus.Subscribe<TestEvent1>(handler1_1);
  bus.Unsubscribe<TestEvent1>(handler1_1);

  EXPECT_EQ(bus.Count<TestEvent1>(), 0u);
}

TEST(EventBus_Tests, Publish_SingleSubscribe_CallsSubscribe)
{
  EventBus bus;

  EventHandler<TestEvent1> handler1_1;
  handler1_1.Bind<&listen1_1>();

  bus.Subscribe<TestEvent1>(handler1_1);

  sum1_1 = 0;

  bus.Publish(TestEvent1 { 10u });

  EXPECT_EQ(sum1_1, 10u);
}

} // namespace plex::tests