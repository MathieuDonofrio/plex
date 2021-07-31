#include "engine/events/event_handler.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestEvent
  {
    size_t value1;
    double value2;
  };

  struct TestListener
  {
    void addValue1(const TestEvent& event)
    {
      sumValue1 += event.value1;
    }

    void addValueConst1(const TestEvent& event)
    {
      sumValue1 += event.value1;
    }

    mutable size_t sumValue1 = 0;
  };

  size_t sumValue1Global = 0u;

  void addValue1Global(const TestEvent& event)
  {
    sumValue1Global += event.value1;
  }

} // namespace

TEST(EventHandler_Tests, Invoke_FreeFunction_DelegatesCall)
{
  sumValue1Global = 0;

  EventHandler<TestEvent> handler(addValue1Global);

  handler.Invoke({ 1u, 0 });

  ASSERT_EQ(sumValue1Global, 1u);

  handler.Invoke({ 10u, 0 });

  ASSERT_EQ(sumValue1Global, 11u);
}

TEST(EventHandler_Tests, Invoke_MemberFunction_DelegatesCall)
{
  TestListener listener;

  EventHandler<TestEvent> handler(&TestListener::addValue1, &listener);

  handler.Invoke({ 1u, 0 });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u, 0 });

  ASSERT_EQ(listener.sumValue1, 11u);
}

TEST(EventHandler_Tests, Invoke_ConstMemberFunction_DelegatesCall)
{
  TestListener listener;

  EventHandler<TestEvent> handler(&TestListener::addValueConst1, &listener);

  handler.Invoke({ 1u, 0 });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u, 0 });

  ASSERT_EQ(listener.sumValue1, 11u);
}
} // namespace genebits::engine::tests