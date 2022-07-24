#include "genebits/engine/events/listener.h"

#include <gtest/gtest.h>

namespace plex::tests
{
namespace
{
  struct TestEvent
  {
    size_t value;
  };

  struct TestEvent2
  {
    size_t value;
  };

  struct TestListener : public Listener<TestListener, TestEvent>
  {
    TestListener(EventBus& bus) : Listener<TestListener, TestEvent>(bus), sum(0) {}

    void Listen(const TestEvent& event)
    {
      sum += event.value;
    }

    size_t sum;
  };

  struct TestListener2 : public Listener<TestListener2, TestEvent, TestEvent2>
  {
    // Used to make sure it compiles

    TestListener2(EventBus& bus) : Listener<TestListener2, TestEvent, TestEvent2>(bus), sum(0) {}

    void Listen(const TestEvent& event)
    {
      sum += event.value;
    }

    void Listen(const TestEvent2& event)
    {
      sum += event.value;
    }

    size_t sum;
  };

}; // namespace

TEST(Listener_Tests, Constructor_SingleListener_SubscribeCountIncrease)
{
  EventBus bus;

  TestListener listener(bus);

  EXPECT_EQ(bus.Count<TestEvent>(), 1u);
}

TEST(Listener_Tests, Constructor_MultipleListeners_SubscribeCountIncrease)
{
  EventBus bus;

  TestListener listener1(bus);
  TestListener listener2(bus);
  TestListener listener3(bus);

  EXPECT_EQ(bus.Count<TestEvent>(), 3u);
}

TEST(Listener_Tests, Destructor_SingleListener_SubscribeCountDecrease)
{
  EventBus bus;

  {
    TestListener listener(bus);
    EXPECT_EQ(bus.Count<TestEvent>(), 1u);
  }

  EXPECT_EQ(bus.Count<TestEvent>(), 0u);
}

TEST(Listener_Tests, Destructor_MultipleListeners_SubscribeCountDecrease)
{
  EventBus bus;

  {
    TestListener listener1(bus);
    {
      TestListener listener2(bus);
      {
        TestListener listener3(bus);
        EXPECT_EQ(bus.Count<TestEvent>(), 3u);
      }
      EXPECT_EQ(bus.Count<TestEvent>(), 2u);
    }
    EXPECT_EQ(bus.Count<TestEvent>(), 1u);
  }

  EXPECT_EQ(bus.Count<TestEvent>(), 0u);
}

TEST(Listener_Tests, Listen_SinglePublish_ReceivedEvent)
{
  EventBus bus;

  TestListener listener(bus);

  TestEvent event { 10 };

  bus.Publish(event);

  EXPECT_EQ(listener.sum, 10u);
}

TEST(Listener_Tests, Listen_MultiPublish_ReceivedEvent)
{
  EventBus bus;

  TestListener listener(bus);

  TestEvent event { 10 };

  bus.Publish(event);
  bus.Publish(event);
  bus.Publish(event);

  EXPECT_EQ(listener.sum, 30u);
}

} // namespace plex::tests