
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

TEST(SystemGroup_Tests, Run_TwoSystemsDependent_ExecutedInSequence)
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

  Sequence s1;

  EXPECT_CALL(system1, OnUpdate)
    .Times(1)
    .InSequence(s1)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });

  EXPECT_CALL(system2, OnUpdate).Times(1).InSequence(s1);

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

  Sequence s1, s2, s3, s4, s5;

  EXPECT_CALL(system1, OnUpdate)
    .Times(1)
    .InSequence(s1)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });

  EXPECT_CALL(system2, OnUpdate)
    .Times(1)
    .InSequence(s2)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(20)); });

  EXPECT_CALL(system3, OnUpdate)
    .Times(1)
    .InSequence(s1, s2, s3)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); });

  EXPECT_CALL(system4, OnUpdate)
    .Times(1)
    .InSequence(s4)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(70)); });

  EXPECT_CALL(system5, OnUpdate)
    .Times(1)
    .InSequence(s4, s1, s5)
    .WillRepeatedly([](JobHandle) { std::this_thread::sleep_for(std::chrono::milliseconds(200)); });

  group.Run();

  group.ForceComplete();
}

} // namespace genebits::engine::tests