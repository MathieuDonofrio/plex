#include "genebits/engine/scheduler/scheduler.h"

#include <gtest/gtest.h>

#include "genebits/engine/async/sync_wait.h"
#include "genebits/engine/async/thread_pool.h"

namespace plex::tests
{
namespace
{
  ThreadPool thread_pool;

  template<size_t id>
  struct MockData
  {
    size_t value;
  };

  template<size_t id>
  struct MockStage
  {};

  template<typename... Components>
  struct MockQuery : public QueryDataAccessFactory<MockQuery<Components...>, Components...>
  {
    static MockQuery FetchData(void*, Context&, Context&)
    {
      return MockQuery();
    }

    static constexpr std::string_view GetCategory()
    {
      return "Test";
    }
  };

  Vector<size_t>& SystemMockCallOrder()
  {
    static Vector<size_t> vector;
    return vector;
  }

  template<size_t id>
  std::atomic_size_t& SystemMockCallCount()
  {
    static std::atomic_size_t call_count { 0 };
    return call_count;
  }

  template<size_t id, Query... queries>
  void SystemMock([[maybe_unused]] queries... q)
  {
    SystemMockCallCount<id>()++;
    SystemMockCallOrder().push_back(id);
  }

  template<size_t id, Query... queries>
  Task<void> AsyncSystemMock([[maybe_unused]] queries... q)
  {
    co_await thread_pool.Schedule();
    SystemMockCallCount<id>()++;
    SystemMockCallOrder().push_back(id);
  }
} // namespace

TEST(Scheduler_Tests, RunAll_NothingScheduled_NoFailiure)
{
  Context context;

  Scheduler scheduler;

  EXPECT_NO_FATAL_FAILURE(SyncWait(scheduler.RunAll(context)));
}

TEST(Scheduler_Tests, RunAll_SingleSystemScheduled_SystemCalled)
{
  Context context;

  Scheduler scheduler;

  scheduler.AddSystem<MockStage<1>>(SystemMock<1>);

  SystemMockCallCount<1>() = 0;

  scheduler.Schedule<MockStage<1>>();

  auto task = scheduler.RunAll(context);
  SyncWait(task);

  EXPECT_EQ(SystemMockCallCount<1>(), 1);
}

TEST(Scheduler_Tests, RunAll_MultipleSystemsSameStage_CorrectExecution)
{
  Context context;

  Scheduler scheduler;

  scheduler.AddSystem<MockStage<1>>(SystemMock<1>);
  scheduler.AddSystem<MockStage<1>>(SystemMock<2>);
  scheduler.AddSystem<MockStage<1>>(SystemMock<3>);
  scheduler.AddSystem<MockStage<1>>(SystemMock<4>);
  scheduler.AddSystem<MockStage<1>>(SystemMock<5>);

  SystemMockCallOrder().clear();

  SystemMockCallCount<1>() = 0;
  SystemMockCallCount<2>() = 0;
  SystemMockCallCount<3>() = 0;
  SystemMockCallCount<4>() = 0;
  SystemMockCallCount<5>() = 0;

  scheduler.Schedule<MockStage<1>>();

  auto task = scheduler.RunAll(context);
  SyncWait(task);

  EXPECT_EQ(SystemMockCallCount<1>(), 1);
  EXPECT_EQ(SystemMockCallCount<2>(), 1);
  EXPECT_EQ(SystemMockCallCount<3>(), 1);
  EXPECT_EQ(SystemMockCallCount<4>(), 1);
  EXPECT_EQ(SystemMockCallCount<5>(), 1);

  Vector<size_t> expected_order { { 1, 2, 3, 4, 5 } };
  EXPECT_EQ(SystemMockCallOrder(), expected_order);
}

TEST(Scheduler_Tests, RunAll_MultipleSystemsDifferentStages_CorrectExecution)
{
  Context context;

  Scheduler scheduler;

  scheduler.AddSystem<MockStage<1>>(SystemMock<1, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<2>>(SystemMock<2, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<3>>(SystemMock<3, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<4>>(SystemMock<4, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<5>>(SystemMock<5, MockQuery<MockData<0>>>);

  SystemMockCallOrder().clear();

  SystemMockCallCount<1>() = 0;
  SystemMockCallCount<2>() = 0;
  SystemMockCallCount<3>() = 0;
  SystemMockCallCount<4>() = 0;
  SystemMockCallCount<5>() = 0;

  scheduler.Schedule<MockStage<1>>();
  scheduler.Schedule<MockStage<2>>();
  scheduler.Schedule<MockStage<3>>();
  scheduler.Schedule<MockStage<4>>();
  scheduler.Schedule<MockStage<5>>();

  auto task = scheduler.RunAll(context);
  SyncWait(task);

  EXPECT_EQ(SystemMockCallCount<1>(), 1);
  EXPECT_EQ(SystemMockCallCount<2>(), 1);
  EXPECT_EQ(SystemMockCallCount<3>(), 1);
  EXPECT_EQ(SystemMockCallCount<4>(), 1);
  EXPECT_EQ(SystemMockCallCount<5>(), 1);

  Vector<size_t> expected_order { { 1, 2, 3, 4, 5 } };
  EXPECT_EQ(SystemMockCallOrder(), expected_order);
}

TEST(Scheduler_Tests, RunAll_AsycDependantSystems_ExecuteInOrder)
{
  Context context;

  Scheduler scheduler;

  scheduler.AddSystem<MockStage<1>>(AsyncSystemMock<1, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<2>>(AsyncSystemMock<2, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<3>>(AsyncSystemMock<3, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<4>>(AsyncSystemMock<4, MockQuery<MockData<0>>>);
  scheduler.AddSystem<MockStage<5>>(AsyncSystemMock<5, MockQuery<MockData<0>>>);

  SystemMockCallOrder().clear();

  SystemMockCallCount<1>() = 0;
  SystemMockCallCount<2>() = 0;
  SystemMockCallCount<3>() = 0;
  SystemMockCallCount<4>() = 0;
  SystemMockCallCount<5>() = 0;

  scheduler.Schedule<MockStage<1>>();
  scheduler.Schedule<MockStage<2>>();
  scheduler.Schedule<MockStage<3>>();
  scheduler.Schedule<MockStage<4>>();
  scheduler.Schedule<MockStage<5>>();

  auto task = scheduler.RunAll(context);
  SyncWait(task);

  EXPECT_EQ(SystemMockCallCount<1>(), 1);
  EXPECT_EQ(SystemMockCallCount<2>(), 1);
  EXPECT_EQ(SystemMockCallCount<3>(), 1);
  EXPECT_EQ(SystemMockCallCount<4>(), 1);
  EXPECT_EQ(SystemMockCallCount<5>(), 1);

  Vector<size_t> expected_order { { 1, 2, 3, 4, 5 } };
  EXPECT_EQ(SystemMockCallOrder(), expected_order);
}

} // namespace plex::tests