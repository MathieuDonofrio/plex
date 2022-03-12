#include "genebits/engine/utilities/delegate.h"

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

static_assert(std::is_trivially_copyable_v<Delegate<void(const TestArg&)>>, "Delegate must be trivially copyable");
static_assert(
  std::is_trivially_destructible_v<Delegate<void(const TestArg&)>>, "Delegate must be trivially destructible");
static_assert(!std::is_polymorphic_v<Delegate<void(const TestArg&)>>, "Delegate cannot be polymorphic");

TEST(Delegate_Tests, Constructor_Default_Null)
{
  Delegate<void(const TestArg&)> handler;

  EXPECT_FALSE(handler);
}

TEST(Delegate_Tests, Bind_FreeFunction_NotNull)
{
  Delegate<void(const TestArg&)> handler;
  handler.Bind<&addValue1Global>();

  EXPECT_TRUE(handler);
}

TEST(Delegate_Tests, Invoke_FreeFunction_DelegatesCall)
{
  sumValue1Global = 0;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<&addValue1Global>();

  handler.Invoke(TestArg { 1u });

  EXPECT_EQ(sumValue1Global, 1u);

  handler.Invoke({ 10u });

  EXPECT_EQ(sumValue1Global, 11u);
}

TEST(Delegate_Tests, InvokeOperator_FreeFunction_DelegatesCall)
{
  sumValue1Global = 0;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<&addValue1Global>();

  handler.Invoke(TestArg { 1u });

  EXPECT_EQ(sumValue1Global, 1u);

  handler({ 10u });

  EXPECT_EQ(sumValue1Global, 11u);
}

TEST(Delegate_Tests, Invoke_MemberFunction_DelegatesCall)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  handler.Invoke({ 1u });

  EXPECT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  EXPECT_EQ(listener.sumValue1, 11u);
}

TEST(Delegate_Tests, Invoke_ConstMemberFunction_DelegatesCall)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  handler.Invoke({ 1u });

  EXPECT_EQ(listener.sumValue1, 1u);

  handler.Invoke({ 10u });

  EXPECT_EQ(listener.sumValue1, 11u);
}

TEST(Delegate_Tests, Invoke_Lambda_DelegatesCall)
{
  size_t sum = 0;

  Delegate<void(const TestArg&)> handler;
  handler.Bind([&sum](const TestArg& arg) { sum += arg.value; });

  handler.Invoke({ 1u });

  EXPECT_EQ(sum, 1u);

  handler.Invoke({ 10u });

  EXPECT_EQ(sum, 11u);
}

TEST(Delegate_Tests, Equality_FreeFunction_Equal)
{
  Delegate<void(const TestArg&)> handler;
  handler.Bind<&addValue1Global>();

  EXPECT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_FreeFunction_NotEqual)
{
  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<&addValue1Global>();

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<&addValue2Global>();

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_Equal)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<TestListener, &TestListener::addValue1>(&listener);

  EXPECT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_MemberFunction_FunctionNotEqual)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValue1>(&listener2);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_MemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener1);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValue2>(&listener2);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_Equal)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler;
  handler.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  EXPECT_EQ(handler, handler);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_FunctionNotEqual)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_InstanceNotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener1);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst1>(&listener2);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_ConstMemberFunction_NotEqual)
{
  TestListener listener1;
  TestListener listener2;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst2>(&listener1);

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind<TestListener, &TestListener::addValueConst2>(&listener2);

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, Equality_Lambda_NotEqual)
{
  Delegate<void(const TestArg&)> handler1;
  handler1.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  Delegate<void(const TestArg&)> handler2;
  handler2.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  EXPECT_NE(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_FreeFunction_Equal)
{
  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<&addValue1Global>();

  Delegate<void(const TestArg&)> handler2 = handler1;

  EXPECT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_MemberFunction_Equal)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValue1>(&listener);

  Delegate<void(const TestArg&)> handler2 = handler1;

  EXPECT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_ConstMemberFunction_Equal)
{
  TestListener listener;

  Delegate<void(const TestArg&)> handler1;
  handler1.Bind<TestListener, &TestListener::addValueConst1>(&listener);

  Delegate<void(const TestArg&)> handler2 = handler1;

  EXPECT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, CopyAssignment_Lambda_Equal)
{
  Delegate<void(const TestArg&)> handler1;
  handler1.Bind(
    [](const TestArg& arg)
    {
      static volatile size_t sum = 0;
      sum += arg.value;
    });

  Delegate<void(const TestArg&)> handler2 = handler1;

  EXPECT_EQ(handler1, handler2);
}

TEST(Delegate_Tests, Invoke_MultipleArgs_CorrectValues)
{
  Delegate<void(int, int, int)> handler;

  handler.Bind(
    [](int arg1, int arg2, int arg3)
    {
      EXPECT_EQ(arg1, 1);
      EXPECT_EQ(arg2, 2);
      EXPECT_EQ(arg3, 3);
    });

  handler.Invoke(1, 2, 3);
}

TEST(Delegate_Tests, Invoke_MultipleArgs_CorrectForwarding)
{
  Delegate<void(int&&, const int&, int&, int)> handler;

  handler.Bind(
    [](int&& arg1, const int& arg2, int& arg3, int arg4)
    {
      EXPECT_EQ(arg1, 1);
      EXPECT_EQ(arg2, 2);
      EXPECT_EQ(arg3, 3);
      EXPECT_EQ(arg4, 4);

      arg3 = 30;
    });

  int arg3 = 3;

  handler.Invoke(1, 2, arg3, 4);

  EXPECT_EQ(arg3, 30);
}

TEST(Delegate_Tests, Invoke_NonVoidReturnType_CorrectValue)
{
  Delegate<int(int)> handler;

  handler.Bind([](int arg) { return arg * 2; });

  EXPECT_EQ(2, handler.Invoke(1));
  EXPECT_EQ(4, handler.Invoke(2));
  EXPECT_EQ(20, handler.Invoke(10));
}
} // namespace genebits::engine::tests