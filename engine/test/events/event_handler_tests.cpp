#include "engine/events/event_handler.h"

#include <type_traits>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestEvent
  {
    volatile size_t value;
  };

  struct TestListener
  {
    void addValue1(const TestEvent& event)
    {
      sumValue1 += event.value;
    }

    void addValue2(const TestEvent& event)
    {
      sumValue2 += event.value;
    }

    void addValueConst1(const TestEvent& event) const
    {
      sumValue1 += event.value;
    }

    void addValueConst2(const TestEvent& event) const
    {
      sumValue2 += event.value;
    }

    mutable size_t sumValue1 = 0;
    mutable size_t sumValue2 = 0;
  };

  size_t sumValue1Global = 0u;
  size_t sumValue2Global = 0u;

  void addValue1Global(const TestEvent& event)
  {
    sumValue1Global += event.value;
  }

  void addValue2Global(const TestEvent& event)
  {
    sumValue2Global += event.value;
  }

} // namespace

static_assert(std::is_trivially_copyable_v<EventHandler<TestEvent>>, "EventHandler must be trivially copyable");
static_assert(std::is_trivially_destructible_v<EventHandler<TestEvent>>, "EventHandler must be trivially destructible");
static_assert(!std::is_polymorphic_v<EventHandler<TestEvent>>, "EventHandler cannot be polymorphic");

TEST(EventHandler_Tests, Constructor_Default_Null)
{
  EventHandler<TestEvent> handler;

  ASSERT_FALSE(handler);
}

TEST(EventHandler_Tests, Bind_FreeFunction_NotNull)
{
  EventHandler<TestEvent> handler;
  handler.Bind<&addValue1Global>();

  ASSERT_TRUE(handler);
}

TEST(EventHandler_Tests, Invoke_FreeFunction_DelegatesCall)
{
  sumValue1Global = 0;

  EventHandler<TestEvent> handler;
  handler.Bind<&addValue1Global>();

  handler.Invoke(TestEvent { 1u });

  ASSERT_EQ(sumValue1Global, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(sumValue1Global, 11u);
}

TEST(EventHandler_Tests, Invoke_MemberFunction_DelegatesCall)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  handler.Invoke({ 1u });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(listener.sumValue1, 11u);
}

TEST(EventHandler_Tests, Invoke_ConstMemberFunction_DelegatesCall)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  handler.Invoke({ 1u });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(listener.sumValue1, 11u);
}

TEST(EventHandler_Tests, Invoke_Lambda_DelegatesCall)
{
  size_t sum = 0;

  EventHandler<TestEvent> handler;
  handler.Bind([&sum](const TestEvent& event)
    { sum += event.value; });

  handler.Invoke({ 1u });

  ASSERT_EQ(sum, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(sum, 11u);
}

TEST(EventHandler_Tests, Equality_FreeFunction_Equal)
{
  EventHandler<TestEvent> handler;
  handler.Bind<&addValue1Global>();

  ASSERT_EQ(handler, handler);
}

TEST(EventHandler_Tests, Equality_FreeFunction_NotEqual)
{
  EventHandler<TestEvent> handler1;
  handler1.Bind<&addValue1Global>();

  EventHandler<TestEvent> handler2;
  handler2.Bind<&addValue2Global>();

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_MemberFunction_Equal)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  ASSERT_EQ(handler, handler);
}

TEST(EventHandler_Tests, Equality_MemberFunction_FunctionNotEqual)
{
  TestListener listener;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_MemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValue1>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_MemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_ConstMemberFunction_Equal)
{
  TestListener listener;

  EventHandler<TestEvent> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  ASSERT_EQ(handler, handler);
}

TEST(EventHandler_Tests, Equality_ConstMemberFunction_FunctionNotEqual)
{
  TestListener listener;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_ConstMemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener1);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst1>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_ConstMemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst2>(&listener1);

  EventHandler<TestEvent> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, Equality_Lambda_NotEqual)
{
  EventHandler<TestEvent> handler1;
  handler1.Bind([](const TestEvent& event)
    {
      static volatile size_t sum = 0;
      sum += event.value;
    });

  EventHandler<TestEvent> handler2;
  handler2.Bind([](const TestEvent& event)
    {
      static volatile size_t sum = 0;
      sum += event.value;
    });

  ASSERT_NE(handler1, handler2);
}

TEST(EventHandler_Tests, CopyAssignment_FreeFunction_Equal)
{
  EventHandler<TestEvent> handler1;
  handler1.Bind<&addValue1Global>();

  EventHandler<TestEvent> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(EventHandler_Tests, CopyAssignment_MemberFunction_Equal)
{
  TestListener listener;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  EventHandler<TestEvent> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(EventHandler_Tests, CopyAssignment_ConstMemberFunction_Equal)
{
  TestListener listener;

  EventHandler<TestEvent> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  EventHandler<TestEvent> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(EventHandler_Tests, CopyAssignment_Lambda_Equal)
{
  EventHandler<TestEvent> handler1;
  handler1.Bind([](const TestEvent& event)
    {
      static volatile size_t sum = 0;
      sum += event.value;
    });

  EventHandler<TestEvent> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}
} // namespace genebits::engine::tests