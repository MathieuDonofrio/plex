
#include "genebits/engine/ecs/system.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
/*
namespace
{
  template<size_t Tag>
  struct SC
  {};

  template<typename... Components>
  class MockSystem : public System<Components...>
  {
  public:
    MOCK_METHOD(void, OnUpdate, (JobHandle));
  };

  class MockJob : public BasicJob<MockJob>
  {
  public:
    ~MockJob() override
    {
      Destroy();
    }

    void operator()()
    {
      Update();
    }

    MOCK_METHOD(void, Destroy, ());
    MOCK_METHOD(void, Update, ());
  };

  void Sleep(size_t ms)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

  bool TestExistence(Phase& phase, SystemBase* system, std::initializer_list<SystemBase*> sync_points)
  {
    auto cs = std::find_if(phase.begin(), phase.end(), [system](auto compiled) { return compiled.system == system; });

    if (cs == phase.end()) return false;

    for (SystemBase* sync_point : sync_points)
    {
      if (cs->sync.end() == std::find(cs->sync.begin(), cs->sync.end(), sync_point)) return false;
    }

    return true;
  }
} // namespace

TEST(System_Tests, GetDataAccess_ReadWriteComponent_Correct)
{
  MockSystem<int> system;

  SystemDataAccessList access = system.GetDataAccess();

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_FALSE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_ReadOnlyComponent_Correct)
{
  MockSystem<const int> system;

  SystemDataAccessList access = system.GetDataAccess();

  EXPECT_EQ(access.size(), 1);

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_TRUE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_MultipleComponents_Correct)
{
  MockSystem<int, const float, bool, const double> system;

  SystemDataAccessList access = system.GetDataAccess();

  EXPECT_EQ(access.size(), 4);

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_FALSE(access[0].read_only);

  EXPECT_EQ(access[1].id, GetComponentId<const float>());
  EXPECT_TRUE(access[1].read_only);

  EXPECT_EQ(access[2].id, GetComponentId<bool>());
  EXPECT_FALSE(access[2].read_only);

  EXPECT_EQ(access[3].id, GetComponentId<const double>());
  EXPECT_TRUE(access[3].read_only);
}

TEST(SystemGroup_Tests, Constructor_Empty_NoSystems)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group;

  EXPECT_EQ(group.Count(), 0);
}

TEST(SystemGroup_Tests, AddSystem_SingleSystem_IncreasesCount)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  EXPECT_EQ(group.Count(), 1);
}

TEST(SystemGroup_Tests, AddSystem_MultipleSystems_IncreasesCount)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  EXPECT_EQ(group.Count(), 1);

  MockSystem<int> system2;
  group.Add(&system2);

  EXPECT_EQ(group.Count(), 2);

  MockSystem<float> system3;
  group.Add(&system3);

  EXPECT_EQ(group.Count(), 3);

  MockSystem<float, const int> system4;
  group.Add(&system4);

  EXPECT_EQ(group.Count(), 4);
}

TEST(SystemGroup_Tests, Run_SingleSystem_UpdatesSystem)
{
  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  group.InitializeSystems(registry, scheduler);

  EXPECT_CALL(system1, OnUpdate).Times(1);

  system1.Run({});
}

TEST(Phase_Tests, Compile_SingleSystem_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  MockSystem<int> system2; // Not compiled

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, { &system1 }));

  EXPECT_FALSE(TestExistence(phase, &system2, {}));
}

TEST(Phase_Tests, Compile_SingleReadonlySystem_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<const int> system1;
  group.Add(&system1);

  MockSystem<int> system2; // Not compiled

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, {}));

  EXPECT_FALSE(TestExistence(phase, &system1, { &system1 }));
  EXPECT_FALSE(TestExistence(phase, &system2, {}));
}

TEST(Phase_Tests, Compile_TwoSystemsDependent_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<int> system2;

  group.Add(&system1);
  group.Add(&system2);

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, { &system2 }));
  EXPECT_TRUE(TestExistence(phase, &system2, { &system1 }));

  EXPECT_FALSE(TestExistence(phase, &system1, { &system1 }));
  EXPECT_FALSE(TestExistence(phase, &system2, { &system2 }));
}

TEST(Phase_Tests, Compile_TwoSystemsIndependent_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<float> system2;

  group.Add(&system1);
  group.Add(&system2);

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, { &system1 }));
  EXPECT_TRUE(TestExistence(phase, &system2, { &system2 }));

  EXPECT_FALSE(TestExistence(phase, &system1, { &system2 }));
  EXPECT_FALSE(TestExistence(phase, &system2, { &system1 }));
}

TEST(Phase_Tests, Compile_TwoSystemsReadonlyIndependent_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<const int> system1;
  MockSystem<const float> system2;

  group.Add(&system1);
  group.Add(&system2);

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, {}));
  EXPECT_TRUE(TestExistence(phase, &system2, {}));

  EXPECT_FALSE(TestExistence(phase, &system1, { &system2 }));
  EXPECT_FALSE(TestExistence(phase, &system2, { &system1 }));
}

TEST(Phase_Tests, Compile_ExternalDependency_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<const int> system1;
  MockSystem<const float> system2;

  group.Add(&system1);
  group.Add(&system2);

  SystemGroup dep1;

  MockSystem<int> dep1_system1;

  dep1.Add(&dep1_system1);

  Phase phase = Phase::Compile(group, { &dep1 });

  EXPECT_FALSE(TestExistence(phase, &dep1_system1, {}));
  EXPECT_TRUE(TestExistence(phase, &system1, { &dep1_system1 }));
  EXPECT_TRUE(TestExistence(phase, &system2, {}));
}

TEST(Phase_Tests, Compile_Complex3Systems_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<const int, float> system1;
  MockSystem<int, bool> system2;
  MockSystem<float, const bool, const int> system3;

  group.Add(&system1);
  group.Add(&system2);
  group.Add(&system3);

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, { &system3 }));
  EXPECT_TRUE(TestExistence(phase, &system2, { &system1 }));
  EXPECT_TRUE(TestExistence(phase, &system3, { &system2 }));
}

TEST(Phase_Tests, Compile_Complex5Systems_CorrectDependencies)
{
  SystemGroup group;

  MockSystem<const int, float> system1;
  MockSystem<int, bool> system2;
  MockSystem<const float, const double> system3;
  MockSystem<float, const bool, const int> system4;
  MockSystem<const bool, double> system5;

  group.Add(&system1);
  group.Add(&system2);
  group.Add(&system3);
  group.Add(&system4);
  group.Add(&system5);

  Phase phase = Phase::Compile(group);

  EXPECT_TRUE(TestExistence(phase, &system1, { &system4 }));
  EXPECT_TRUE(TestExistence(phase, &system2, { &system1, &system5 }));
  EXPECT_TRUE(TestExistence(phase, &system3, {}));
  EXPECT_TRUE(TestExistence(phase, &system4, { &system2, &system3 }));
  EXPECT_TRUE(TestExistence(phase, &system5, {}));

  EXPECT_FALSE(TestExistence(phase, &system1, { &system1 }));
  EXPECT_FALSE(TestExistence(phase, &system2, { &system2 }));
  EXPECT_FALSE(TestExistence(phase, &system3, { &system1, &system5 }));
  EXPECT_FALSE(TestExistence(phase, &system4, { &system4 }));
  EXPECT_FALSE(TestExistence(phase, &system5, { &system5 }));
}

TEST(Phase_Tests, Run_SingleSystem_Executed)
{
  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  EXPECT_CALL(system1, OnUpdate).Times(1);

  phase.Run();

  phase.ForceComplete();
}

TEST(Phase_Tests, Run_SingleSystemMultipleRuns_ExecutedMultipleTimes)
{
  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  group.Add(&system1);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  EXPECT_CALL(system1, OnUpdate).Times(3);

  phase.Run();
  phase.Run();
  phase.Run();

  phase.ForceComplete();
}

TEST(Phase_Tests, Run_TwoSystemsIndependent_Executed)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<const int> system1;
  MockSystem<const float> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  EXPECT_CALL(system1, OnUpdate).Times(1);
  EXPECT_CALL(system2, OnUpdate).Times(1);

  phase.Run();

  phase.ForceComplete();
}

TEST(Phase_Tests, Run_SingleSystemUpdatedOnce_CorrectlyHandled)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system;
  group.Add(&system);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();

  EXPECT_CALL(system, OnUpdate).Times(1).WillOnce([&](auto deps) { system.ScheduleDefered(job1, deps); });

  EXPECT_CALL(*job1, Update).Times(1).WillOnce([]() { Sleep(10); });

  EXPECT_CALL(*job1, Destroy).Times(1);

  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
}

TEST(Phase_Tests, Run_SameSystemUpdatedTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system;
  group.Add(&system);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();

  Sequence s1;

  EXPECT_CALL(system, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
}

TEST(Phase_Tests, Run_TwoSystemsDependent_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<int> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();

  Sequence s1;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);

  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
}

TEST(Phase_Tests, Run_TwoSystemsDependentTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<int> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();
  auto job4 = MakeRef<MockJob>();

  Sequence s1;

  EXPECT_CALL(system1, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job3, deps); });
  EXPECT_CALL(system2, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); })
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job4, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(5); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job4, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
  ::testing::Mock::AllowLeak(job3.Get()); // Safe
  ::testing::Mock::AllowLeak(job4.Get()); // Safe
}

TEST(Phase_Tests, Run_TwoSystemsParallel_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<float> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();

  Sequence s1, s2;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);

  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
}

TEST(Phase_Tests, Run_TwoSystemsParallelTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<float> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();
  auto job4 = MakeRef<MockJob>();

  Sequence s1, s2;

  EXPECT_CALL(system1, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job3, deps); });
  EXPECT_CALL(system2, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); })
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job4, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(5); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s2);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job4, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
  ::testing::Mock::AllowLeak(job3.Get()); // Safe
  ::testing::Mock::AllowLeak(job4.Get()); // Safe
}

TEST(Phase_Tests, Run_WriteRead_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<const int> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();

  Sequence s1;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);

  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
}

TEST(Phase_Tests, Run_WriteReadTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<int> system1;
  MockSystem<const int> system2;

  group.Add(&system1);
  group.Add(&system2);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();
  auto job4 = MakeRef<MockJob>();

  Sequence s1;

  EXPECT_CALL(system1, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job3, deps); });
  EXPECT_CALL(system2, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); })
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job4, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);
  EXPECT_CALL(*job4, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
}

TEST(Phase_Tests, Run_ReadReadWrite_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<const int> system1;
  MockSystem<const int> system2;
  MockSystem<int> system3;

  group.Add(&system1);
  group.Add(&system2);
  group.Add(&system3);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();

  Sequence s1, s2;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });
  EXPECT_CALL(system3, OnUpdate).Times(1).WillOnce([&](auto deps) { system3.ScheduleDefered(job3, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1, s2).WillOnce([]() { Sleep(10); });

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);

  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
  ::testing::Mock::AllowLeak(job3.Get()); // Safe
}

TEST(Phase_Tests, Run_ReadReadWriteTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<const int> system1;
  MockSystem<const int> system2;
  MockSystem<int> system3;

  group.Add(&system1);
  group.Add(&system2);
  group.Add(&system3);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();
  auto job4 = MakeRef<MockJob>();
  auto job5 = MakeRef<MockJob>();
  auto job6 = MakeRef<MockJob>();

  Sequence s1, s2, s3, s4, s5, s6;

  EXPECT_CALL(system1, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job4, deps); });
  EXPECT_CALL(system2, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); })
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job5, deps); });
  EXPECT_CALL(system3, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system3.ScheduleDefered(job3, deps); })
    .WillOnce([&](auto deps) { system3.ScheduleDefered(job6, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(30); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1, s2, s3).WillOnce([]() { Sleep(2); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s3, s4).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job5, Update).Times(1).InSequence(s5).WillOnce([]() { Sleep(30); });
  EXPECT_CALL(*job6, Update).Times(1).InSequence(s4, s5, s6).WillOnce([]() { Sleep(2); });

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);
  EXPECT_CALL(*job4, Destroy).Times(1);
  EXPECT_CALL(*job5, Destroy).Times(1);
  EXPECT_CALL(*job6, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
  ::testing::Mock::AllowLeak(job3.Get()); // Safe
  ::testing::Mock::AllowLeak(job4.Get()); // Safe
  ::testing::Mock::AllowLeak(job5.Get()); // Safe
  ::testing::Mock::AllowLeak(job6.Get()); // Safe
}

TEST(Phase_Tests, Run_Complex5SystemsTwice_ExecutedInSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);

  SystemGroup group;

  MockSystem<const SC<0>, SC<1>> system1; // SF:  // LF: system 3, system 5
  MockSystem<const SC<0>> system2; // SF: // LF: system 3
  MockSystem<const SC<1>, SC<0>> system3; // SF: system 1, system 2 // LF:
  MockSystem<SC<2>> system4; // SF: // LF: system 5
  MockSystem<const SC<2>, const SC<1>, SC<3>> system5; // SF: system 4, system 1

  group.Add(&system1);
  group.Add(&system2);
  group.Add(&system3);
  group.Add(&system4);
  group.Add(&system5);

  group.InitializeSystems(registry, scheduler);

  Phase phase = Phase::Compile(group);

  auto job1 = MakeRef<MockJob>();
  auto job2 = MakeRef<MockJob>();
  auto job3 = MakeRef<MockJob>();
  auto job4 = MakeRef<MockJob>();
  auto job5 = MakeRef<MockJob>();
  auto job6 = MakeRef<MockJob>();
  auto job7 = MakeRef<MockJob>();
  auto job8 = MakeRef<MockJob>();
  auto job9 = MakeRef<MockJob>();
  auto job10 = MakeRef<MockJob>();

  Sequence s1, s2, s3, s4, s5;

  EXPECT_CALL(system1, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system1.ScheduleDefered(job6, deps); });
  EXPECT_CALL(system2, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); })
    .WillOnce([&](auto deps) { system2.ScheduleDefered(job7, deps); });
  EXPECT_CALL(system3, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system3.ScheduleDefered(job3, deps); })
    .WillOnce([&](auto deps) { system3.ScheduleDefered(job8, deps); });
  EXPECT_CALL(system4, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system4.ScheduleDefered(job4, deps); })
    .WillOnce([&](auto deps) { system4.ScheduleDefered(job9, deps); });
  EXPECT_CALL(system5, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system5.ScheduleDefered(job5, deps); })
    .WillOnce([&](auto deps) { system5.ScheduleDefered(job10, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(100); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1, s2, s3).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s4).WillOnce([]() { Sleep(70); });
  EXPECT_CALL(*job5, Update).Times(1).InSequence(s4, s1, s5).WillOnce([]() { Sleep(200); });
  EXPECT_CALL(*job6, Update).Times(1).InSequence(s3, s5, s1).WillOnce([]() { Sleep(100); });
  EXPECT_CALL(*job7, Update).Times(1).InSequence(s3, s2).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job8, Update).Times(1).InSequence(s1, s2, s3).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job9, Update).Times(1).InSequence(s5, s4).WillOnce([]() { Sleep(70); });
  EXPECT_CALL(*job10, Update).Times(1).InSequence(s4, s1, s5).WillOnce([]() { Sleep(200); });

  EXPECT_CALL(*job1, Destroy).Times(1);
  EXPECT_CALL(*job3, Destroy).Times(1);
  EXPECT_CALL(*job2, Destroy).Times(1);
  EXPECT_CALL(*job4, Destroy).Times(1);
  EXPECT_CALL(*job5, Destroy).Times(1);
  EXPECT_CALL(*job6, Destroy).Times(1);
  EXPECT_CALL(*job7, Destroy).Times(1);
  EXPECT_CALL(*job8, Destroy).Times(1);
  EXPECT_CALL(*job9, Destroy).Times(1);
  EXPECT_CALL(*job10, Destroy).Times(1);

  phase.Run();
  phase.Run();

  phase.ForceComplete();

  ::testing::Mock::AllowLeak(job1.Get()); // Safe
  ::testing::Mock::AllowLeak(job2.Get()); // Safe
  ::testing::Mock::AllowLeak(job3.Get()); // Safe
  ::testing::Mock::AllowLeak(job4.Get()); // Safe
  ::testing::Mock::AllowLeak(job5.Get()); // Safe
  ::testing::Mock::AllowLeak(job6.Get()); // Safe
  ::testing::Mock::AllowLeak(job7.Get()); // Safe
  ::testing::Mock::AllowLeak(job8.Get()); // Safe
  ::testing::Mock::AllowLeak(job9.Get()); // Safe
  ::testing::Mock::AllowLeak(job10.Get()); // Safe
}
*/
} // namespace genebits::engine::tests