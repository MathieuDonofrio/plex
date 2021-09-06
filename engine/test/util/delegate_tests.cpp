#include "genebits/engine/util/delegate.h"

#include <type_traits>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestArg
  {
    volatile size_t value;
  };

  struct TestListener
  {
    void addValue1(const TestArg& arg)
    {
      sumValue1 += arg.value;
    }

    void addValue2(const TestArg& arg)
    {
      sumValue2 += arg.value;
    }

    void addValueConst1(const TestArg& arg) const
    {
      sumValue1 += arg.value;
    }

    void addValueConst2(const TestArg& arg) const
    {
      sumValue2 += arg.value;
    }

    mutable size_t sumValue1 = 0;
    mutable size_t sumValue2 = 0;
  };

  size_t sumValue1Global = 0u;
  size_t sumValue2Global = 0u;

  void addValue1Global(const TestArg& arg)
  {
    sumValue1Global += arg.value;
  }

  void addValue2Global(const TestArg& arg)
  {
    sumValue2Global += arg.value;
  }

} // namespace

static_assert(std::is_trivially_copyable_v<Delegate<const TestArg&>>, "Delegate must be trivially copyable");
static_assert(std::is_trivially_destructible_v<Delegate<const TestArg&>>, "Delegate must be trivially destructible");
static_assert(!std::is_polymorphic_v<Delegate<const TestArg&>>, "Delegate cannot be polymorphic");

TEST(Delegate_Tests, Constructor_Default_Null)
{
  Delegate<const TestArg&> handler;

  ASSERT_FALSE(handler);
}

TEST(Delegate_Tests, Bind_FreeFunction_NotNull)
{
  Delegate<const TestArg&> handler;
  handler.Bind<&addValue1Global>();

  ASSERT_TRUE(handler);
}

TEST(Delegate_Tests, Invoke_FreeFunction_DelegatesCall)
{
  sumValue1Global = 0;

  Delegate<const TestArg&> handler;
  handler.Bind<&addValue1Global>();

  handler.Invoke(TestArg { 1u });

  ASSERT_EQ(sumValue1Global, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(sumValue1Global, 11u);
}

TEST(Delegate_Tests, Invoke_MemberFunction_DelegatesCall)
{
  TestListener listener;

  Delegate<const TestArg&> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  handler.Invoke({ 1u });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(listener.sumValue1, 11u);
}

TEST(Delegate_Tests, Invoke_ConstMemberFunction_DelegatesCall)
{
  TestListener listener;

  Delegate<const TestArg&> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  handler.Invoke({ 1u });

  ASSERT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(listener.sumValue1, 11u);
}

TEST(Delegate_Tests, Invoke_Lambda_DelegatesCall)
{
  size_t sum = 0;

  Delegate<const TestArg&> handler;
  handler.Bind([&sum](const TestArg& arg) { sum += arg.value; });

  handler.Invoke({ 1u });

  ASSERT_EQ(sum, 1u);

  handler.Invoke({ 10u });

  ASSERT_EQ(sum, 11u);
}

TEST(Delegate_Tests, Equality_FreeFunction_Equal)
{
  Delegate<const TestArg&> handler;
  handler.Bind<&addValue1Global>();

  ASSERT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_FreeFunction_NotEqual)
{
  Delegate<const TestArg&> handler1;
  handler1.Bind<&addValue1Global>();

  Delegate<const TestArg&> handler2;
  handler2.Bind<&addValue2Global>();

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_Equal)
{
  TestListener listener;

  Delegate<const TestArg&> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  ASSERT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_MemberFunction_FunctionNotEqual)
{
  TestListener listener;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValue1>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_Equal)
{
  TestListener listener;

  Delegate<const TestArg&> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  ASSERT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_FunctionNotEqual)
{
  TestListener listener;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener1);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst1>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst2>(&listener1);

  Delegate<const TestArg&> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener2);

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_Lambda_NotEqual)
{
  Delegate<const TestArg&> handler1;
  handler1.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  Delegate<const TestArg&> handler2;
  handler2.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  ASSERT_NE(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_FreeFunction_Equal)
{
  Delegate<const TestArg&> handler1;
  handler1.Bind<&addValue1Global>();

  Delegate<const TestArg&> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_MemberFunction_Equal)
{
  TestListener listener;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  Delegate<const TestArg&> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_ConstMemberFunction_Equal)
{
  TestListener listener;

  Delegate<const TestArg&> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  Delegate<const TestArg&> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_Lambda_Equal)
{
  Delegate<const TestArg&> handler1;
  handler1.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  Delegate<const TestArg&> handler2 = handler1;

  ASSERT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, Invoke_MultipleArgs_CorrectValues)
{
  Delegate<int, int, int> handler;

  handler.Bind(
    [](int arg1, int arg2, int arg3)
    {
      ASSERT_EQ(arg1, 1);
      ASSERT_EQ(arg2, 2);
      ASSERT_EQ(arg3, 3);
    });

  handler.Invoke(1, 2, 3);
}

TEST(Delegate_Tests, Invoke_MultipleArgs_CorrectForwarding)
{
  Delegate<int&&, const int&, int&, int> handler;

  handler.Bind(
    [](int&& arg1, const int& arg2, int& arg3, int arg4)
    {
      ASSERT_EQ(arg1, 1);
      ASSERT_EQ(arg2, 2);
      ASSERT_EQ(arg3, 3);
      ASSERT_EQ(arg4, 4);

      arg3 = 30;
    });

  int arg3 = 3;

  handler.Invoke(1, 2, arg3, 4);

  ASSERT_EQ(arg3, 30);
}
} // namespace genebits::engine::tests