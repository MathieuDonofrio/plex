
#include "genebits/engine/ecs/phase.h"

#include <gmock/gmock.h>

#include "genebits/engine/parallel/sync_wait.h"

namespace genebits::engine::tests
{
namespace
{
  template<typename... Components>
  class MockSystem : public System<Components...>
  {
  public:
    MOCK_METHOD(Task<>, OnUpdate, ());
  };

  class UpdateNotifier
  {
  public:
    MOCK_METHOD(void, NotifyStart, ());
    MOCK_METHOD(void, NotifyEnd, ());
  };

  Task<> MakeSleepTask(UpdateNotifier* notifier, ThreadPool& pool, size_t ms)
  {
    if (notifier) notifier->NotifyStart();

    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds(ms));

    if (notifier) notifier->NotifyEnd();
  }

  Task<> MakeQuickTask(UpdateNotifier* notifier)
  {
    if (notifier)
    {
      notifier->NotifyStart();
      notifier->NotifyEnd();
    }

    co_return;
  }

#define SLEEP_TASK(notifier, pool, ms) [&]() { return MakeSleepTask(notifier, pool, ms); }

#define QUICK_TASK(notifier) [&]() { return MakeQuickTask(notifier); }

} // namespace

TEST(Phase_Tests, Compile_0Systems_Empty)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  Phase phase = Phase::Compile(group);

  EXPECT_EQ(phase.Count(), 0);
}

TEST(Phase_Tests, Compile_1Systems_HasOneSystem)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system = MakeRef<MockSystem<>>();

  group->Add(system);

  Phase phase = Phase::Compile(group);

  EXPECT_EQ(phase.Count(), 1);
}

TEST(Phase_Tests, Compile_2SystemsNoDeps_NoDeps)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<>>();
  auto system2 = MakeRef<MockSystem<>>();

  group->Add(system1);
  group->Add(system2);

  Phase phase = Phase::Compile(group);

  EXPECT_EQ(phase.Count(), 2);

  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system1.Get()));
}

TEST(Phase_Tests, Compile_2SystemsWithDeps_CorrectDeps)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<int>>();
  auto system2 = MakeRef<MockSystem<int>>();

  group->Add(system1);
  group->Add(system2);

  Phase phase = Phase::Compile(group);

  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));
  EXPECT_TRUE(phase.CheckDependency(system2.Get(), system1.Get()));
}

TEST(Phase_Tests, Run_SingleSystem_Update)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system = MakeRef<MockSystem<>>();

  group->Add(system);

  Phase phase = Phase::Compile(group);

  UpdateNotifier notifier;

  EXPECT_CALL(*system.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&notifier, pool, 10));

  EXPECT_CALL(notifier, NotifyStart).Times(1);
  EXPECT_CALL(notifier, NotifyEnd).Times(1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_2Systems_UpdateAll)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<>>();
  auto system2 = MakeRef<MockSystem<>>();

  group->Add(system1);
  group->Add(system2);

  Phase phase = Phase::Compile(group);

  UpdateNotifier s1n;
  UpdateNotifier s2n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 10));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s2n, pool, 10));

  EXPECT_CALL(s1n, NotifyStart).Times(1);
  EXPECT_CALL(s1n, NotifyEnd).Times(1);
  EXPECT_CALL(s2n, NotifyStart).Times(1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_2SystemsNoDeps_QuickestFinishesFirst)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<const int>>();
  auto system2 = MakeRef<MockSystem<const int>>();

  group->Add(system1);
  group->Add(system2);

  Phase phase = Phase::Compile(group);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));

  // System 2
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));

  UpdateNotifier s1n;
  UpdateNotifier s2n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s2n));

  ::testing::Sequence s1;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1); // System 2 if faster
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_2SystemsWithDeps_UpdateInSeq)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<int>>();
  auto system2 = MakeRef<MockSystem<int>>();

  group->Add(system1);
  group->Add(system2);

  Phase phase = Phase::Compile(group);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));

  // System 2
  EXPECT_TRUE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));

  UpdateNotifier s1n;
  UpdateNotifier s2n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s2n));

  ::testing::Sequence s1;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_5SystemsWithDeps_UpdateInSeq)
{
  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<int, double>>();
  auto system2 = MakeRef<MockSystem<const int>>();
  auto system3 = MakeRef<MockSystem<float, const int>>();
  auto system4 = MakeRef<MockSystem<bool, float>>();
  auto system5 = MakeRef<MockSystem<double, const bool, float>>();

  group->Add(system1);
  group->Add(system2);
  group->Add(system3);
  group->Add(system4);
  group->Add(system5);

  Phase phase = Phase::Compile(group);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system5.Get()));

  // System 2
  EXPECT_TRUE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system5.Get()));

  // System 3
  EXPECT_TRUE(phase.CheckDependency(system3.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system5.Get()));

  // System 4
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system2.Get()));
  EXPECT_TRUE(phase.CheckDependency(system4.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system5.Get()));

  // System 5
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system3.Get()));
  EXPECT_TRUE(phase.CheckDependency(system5.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system5.Get()));

  UpdateNotifier s1n, s2n, s3n, s4n, s5n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s2n, pool, 50));
  EXPECT_CALL(*system3.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s3n, pool, 75));
  EXPECT_CALL(*system4.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s4n, pool, 50));
  EXPECT_CALL(*system5.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s5n));

  ::testing::Sequence s1, s2, s3;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1, s2);
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1, s2);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1);
  EXPECT_CALL(s3n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s3n, NotifyEnd).Times(1).InSequence(s2);
  EXPECT_CALL(s4n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s4n, NotifyEnd).Times(1).InSequence(s2);
  EXPECT_CALL(s5n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s5n, NotifyEnd).Times(1).InSequence(s2);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_2Systems2GroupsNoDeps_QuickestFinishesFirst)
{
  ThreadPool pool;

  auto group1 = MakeRef<SystemGroup>();
  auto group2 = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<const int>>();
  auto system2 = MakeRef<MockSystem<const int>>();

  group1->Add(system1);
  group2->Add(system2);

  Phase phase = Phase::Compile(group1, group2);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));

  // System 2
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));

  UpdateNotifier s1n;
  UpdateNotifier s2n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s2n));

  ::testing::Sequence s1;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1); // System 2 if faster
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_2Systems2GroupsWithDeps_UpdateInSeq)
{
  ThreadPool pool;

  auto group1 = MakeRef<SystemGroup>();
  auto group2 = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<int>>();
  auto system2 = MakeRef<MockSystem<int>>();

  group1->Add(system1);
  group2->Add(system2);

  Phase phase = Phase::Compile(group1, group2);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));

  // System 2
  EXPECT_TRUE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));

  UpdateNotifier s1n;
  UpdateNotifier s2n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s2n));

  ::testing::Sequence s1;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1);

  SyncWait(phase.Run());
}

TEST(Phase_Tests, Run_5Systems3GroupsWithDeps_UpdateInSeq)
{
  ThreadPool pool;

  auto group1 = MakeRef<SystemGroup>();
  auto group2 = MakeRef<SystemGroup>();
  auto group3 = MakeRef<SystemGroup>();

  auto system1 = MakeRef<MockSystem<int, double>>();
  auto system2 = MakeRef<MockSystem<const int>>();
  auto system3 = MakeRef<MockSystem<float, const int>>();
  auto system4 = MakeRef<MockSystem<bool, float>>();
  auto system5 = MakeRef<MockSystem<double, const bool, float>>();

  group1->Add(system1);
  group1->Add(system2);
  group2->Add(system3);
  group3->Add(system4);
  group3->Add(system5);

  Phase phase = Phase::Compile(group1, group2, group3);

  // System 1
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system1.Get(), system5.Get()));

  // System 2
  EXPECT_TRUE(phase.CheckDependency(system2.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system2.Get(), system5.Get()));

  // System 3
  EXPECT_TRUE(phase.CheckDependency(system3.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system3.Get(), system5.Get()));

  // System 4
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system2.Get()));
  EXPECT_TRUE(phase.CheckDependency(system4.Get(), system3.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system4.Get(), system5.Get()));

  // System 5
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system1.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system2.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system3.Get()));
  EXPECT_TRUE(phase.CheckDependency(system5.Get(), system4.Get()));
  EXPECT_FALSE(phase.CheckDependency(system5.Get(), system5.Get()));

  UpdateNotifier s1n, s2n, s3n, s4n, s5n;

  EXPECT_CALL(*system1.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s1n, pool, 100));
  EXPECT_CALL(*system2.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s2n, pool, 50));
  EXPECT_CALL(*system3.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s3n, pool, 75));
  EXPECT_CALL(*system4.Get(), OnUpdate).Times(1).WillOnce(SLEEP_TASK(&s4n, pool, 50));
  EXPECT_CALL(*system5.Get(), OnUpdate).Times(1).WillOnce(QUICK_TASK(&s5n));

  ::testing::Sequence s1, s2, s3;

  EXPECT_CALL(s1n, NotifyStart).Times(1).InSequence(s1, s2);
  EXPECT_CALL(s1n, NotifyEnd).Times(1).InSequence(s1, s2);
  EXPECT_CALL(s2n, NotifyStart).Times(1).InSequence(s1);
  EXPECT_CALL(s2n, NotifyEnd).Times(1).InSequence(s1);
  EXPECT_CALL(s3n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s3n, NotifyEnd).Times(1).InSequence(s2);
  EXPECT_CALL(s4n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s4n, NotifyEnd).Times(1).InSequence(s2);
  EXPECT_CALL(s5n, NotifyStart).Times(1).InSequence(s2);
  EXPECT_CALL(s5n, NotifyEnd).Times(1).InSequence(s2);

  SyncWait(phase.Run());
}

} // namespace genebits::engine::tests