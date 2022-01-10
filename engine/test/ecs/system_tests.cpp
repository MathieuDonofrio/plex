
#include "genebits/engine/ecs/system.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
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
} // namespace

TEST(System_Tests, GetDataAccess_ReadWriteComponent_Correct)
{
  MockSystem<int> system;

  std::vector<SystemDataAccess> access = system.GetDataAccess();

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_FALSE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_ReadOnlyComponent_Correct)
{
  MockSystem<const int> system;

  std::vector<SystemDataAccess> access = system.GetDataAccess();

  EXPECT_EQ(access.size(), 1);

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_TRUE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_MultipleComponents_Correct)
{
  MockSystem<int, const float, bool, const double> system;

  std::vector<SystemDataAccess> access = system.GetDataAccess();

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

  SystemGroup group(&scheduler, &registry);

  EXPECT_EQ(group.Count(), 0);
}

TEST(SystemGroup_Tests, AddSystem_SingleSystem_IncreasesCount)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system1;

  group.AddSystem(&system1);

  EXPECT_EQ(group.Count(), 1);
}

TEST(SystemGroup_Tests, AddSystem_MultipleSystems_IncreasesCount)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system1;

  group.AddSystem(&system1);

  EXPECT_EQ(group.Count(), 1);

  MockSystem<int> system2;

  group.AddSystem(&system2);

  EXPECT_EQ(group.Count(), 2);

  MockSystem<float> system3;

  group.AddSystem(&system3);

  EXPECT_EQ(group.Count(), 3);

  MockSystem<float, const int> system4;

  group.AddSystem(&system4);

  EXPECT_EQ(group.Count(), 4);
}

TEST(SystemGroup_Tests, Run_SingleSystem_Executed)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system1;

  group.AddSystem(&system1);

  group.Compile();

  EXPECT_CALL(system1, OnUpdate).Times(1);

  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_SingleSystemMultipleRuns_ExecutedMultipleTimes)
{
  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system1;

  group.AddSystem(&system1);

  group.Compile();

  EXPECT_CALL(system1, OnUpdate).Times(3);

  group.Run();
  group.Run();
  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_TwoSystemsIndependent_Executed)
{
  using ::testing::Sequence;

  ThreadPool pool;

  Registry registry;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MockSystem<const int> system1;
  MockSystem<const float> system2;

  group.AddSystem(&system1);
  group.AddSystem(&system2);

  group.Compile();

  EXPECT_CALL(system1, OnUpdate).Times(1);
  EXPECT_CALL(system2, OnUpdate).Times(1);

  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_SingleSystemUpdatedOnce_CorrectlyHandled)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);
  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system;
  group.AddSystem(&system);

  group.Compile();

  auto* job1 = new MockJob;

  EXPECT_CALL(system, OnUpdate).Times(1).WillOnce([&](auto deps) { system.ScheduleDefered(job1, deps); });

  EXPECT_CALL(*job1, Update).Times(1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job1, Destroy).Times(1);

  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_SameSystemUpdatedTwice_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);
  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system;
  group.AddSystem(&system);

  group.Compile();

  auto* job1 = new MockJob;
  auto* job2 = new MockJob;

  Sequence s1, s2;

  EXPECT_CALL(system, OnUpdate)
    .Times(2)
    .WillOnce([&](auto deps) { system.ScheduleDefered(job1, deps); })
    .WillOnce([&](auto deps) { system.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);

  EXPECT_CALL(*job1, Destroy).Times(1).InSequence(s2);
  EXPECT_CALL(*job2, Destroy).Times(1).InSequence(s2);

  group.Run();
  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_TwoSystemsDependent_InSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);
  SystemGroup group(&scheduler, &registry);

  MockSystem<int> system1;
  MockSystem<int> system2;

  group.AddSystem(&system1);
  group.AddSystem(&system2);

  group.Compile();

  auto* job1 = new MockJob;
  auto* job2 = new MockJob;

  Sequence s1, s2;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s1);

  group.Run();

  group.ForceComplete();
}

TEST(SystemGroup_Tests, Run_MultipleSystemsDependent_ExecutedInSequence)
{
  using ::testing::Sequence;

  ThreadPool pool;
  Registry registry;
  JobScheduler scheduler(pool);
  SystemGroup group(&scheduler, &registry);

  MockSystem<const SC<0>, SC<1>> system1; // Dependencies:
  MockSystem<const SC<0>> system2; // Dependencies:
  MockSystem<const SC<1>, SC<0>> system3; // Dependencies: system 1, system 2
  MockSystem<SC<2>> system4; // Dependencies:
  MockSystem<const SC<2>, const SC<1>, SC<3>> system5; // Dependencies: system 4, system 1

  group.AddSystem(&system1);
  group.AddSystem(&system2);
  group.AddSystem(&system3);
  group.AddSystem(&system4);
  group.AddSystem(&system5);

  group.Compile();

  auto* job1 = new MockJob;
  auto* job2 = new MockJob;
  auto* job3 = new MockJob;
  auto* job4 = new MockJob;
  auto* job5 = new MockJob;

  Sequence s1, s2, s3, s4, s5;

  EXPECT_CALL(system1, OnUpdate).Times(1).WillOnce([&](auto deps) { system1.ScheduleDefered(job1, deps); });
  EXPECT_CALL(system2, OnUpdate).Times(1).WillOnce([&](auto deps) { system2.ScheduleDefered(job2, deps); });
  EXPECT_CALL(system3, OnUpdate).Times(1).WillOnce([&](auto deps) { system3.ScheduleDefered(job3, deps); });
  EXPECT_CALL(system4, OnUpdate).Times(1).WillOnce([&](auto deps) { system4.ScheduleDefered(job4, deps); });
  EXPECT_CALL(system5, OnUpdate).Times(1).WillOnce([&](auto deps) { system5.ScheduleDefered(job5, deps); });

  EXPECT_CALL(*job1, Update).Times(1).InSequence(s1).WillOnce([]() { Sleep(100); });
  EXPECT_CALL(*job2, Update).Times(1).InSequence(s2).WillOnce([]() { Sleep(20); });
  EXPECT_CALL(*job3, Update).Times(1).InSequence(s1, s2, s3).WillOnce([]() { Sleep(10); });
  EXPECT_CALL(*job4, Update).Times(1).InSequence(s4).WillOnce([]() { Sleep(70); });
  EXPECT_CALL(*job5, Update).Times(1).InSequence(s4, s1, s5).WillOnce([]() { Sleep(200); });

  group.Run();

  group.ForceComplete();
}

} // namespace genebits::engine::tests