#include "plex/scheduler/scheduler.h"

#include <gtest/gtest.h>

#include <bit>
#include <optional>

namespace plex::tests
{
namespace
{
  template<size_t id>
  struct MockData
  {
    size_t value;
  };

  template<size_t id>
  struct MockStage
  {};

  template<typename... Components>
  struct MockQuery
  {
    static MockQuery Fetch(void*, Context&, Context&)
    {
      return MockQuery();
    }

    static consteval std::array<QueryDataAccess, sizeof...(Components)> GetDataAccess() noexcept
    {
      return { QueryDataAccess {
        "mock", TypeName<Components>(), std::is_const_v<Components>, IsThreadSafe<Components>::value }... };
    }
  };

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
  }

  template<System SystemType>
  std::optional<size_t> FindSystem(const Vector<Scheduler::Step>& steps, SystemType system)
  {
    for (size_t i = 0; i < steps.size(); i++)
    {
      if (steps[i].system->Handle() == std::bit_cast<SystemHandle>(system))
      {
        return i;
      }
    }

    return std::nullopt;
  }

  bool HasDependency(const Vector<Scheduler::Step>& steps, size_t system1, size_t system2)
  {
    for (size_t dependency : steps[system2].dependencies)
    {
      if (dependency == system1 || HasDependency(steps, system1, dependency)) return true;
    }

    return false;
  }

  template<System System1, System System2>
  bool RunsAfter(const Vector<Scheduler::Step>& steps, System1 system1, System2 system2)
  {
    std::optional<size_t> system1_index = FindSystem(steps, system1);
    std::optional<size_t> system2_index = FindSystem(steps, system2);

    ASSERT(system1_index, "Did not find system 1");
    ASSERT(system2_index, "Did not find system 2");

    return HasDependency(steps, system1_index.value(), system2_index.value());
  }

  template<System System1, System System2>
  bool IsOrderedExclusive(const Vector<Scheduler::Step>& steps, System1 system1, System2 system2)
  {
    bool cond1 = RunsAfter(steps, system1, system2);
    bool cond2 = RunsAfter(steps, system2, system1);

    return (cond1 || cond2) && !(cond1 && cond2);
  }

  bool IsCyclic(const Vector<Scheduler::Step>& steps, size_t system, size_t i)
  {
    for (auto dependency : steps[i].dependencies)
    {
      if (dependency == system || IsCyclic(steps, system, dependency)) return true;
    }

    return false;
  }

  bool HasCircularDependency(const Vector<Scheduler::Step>& steps)
  {
    for (size_t i = 0; i < steps.size(); i++)
    {
      for (auto dependency : steps[i].dependencies)
      {
        if (IsCyclic(steps, i, dependency)) return true;
      }
    }

    return false;
  }
} // namespace

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_NoSystems_ZeroSteps)
{
  auto steps = ComputeSchedulerData({});

  EXPECT_EQ(steps.size(), 0);
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_SingleSystem_OneStep)
{
  Stage stage1;

  stage1.AddSystem(SystemMock<1>);

  Vector<Stage*> stages { { &stage1 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 1);

  EXPECT_EQ(steps[0].system->Handle(), std::bit_cast<SystemHandle>(&SystemMock<1>));
  EXPECT_EQ(steps[0].dependencies.size(), 0);
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_TwoSystemsNoQueriesOneStage_CanRunInParallel)
{
  Stage stage1;

  stage1.AddSystem(SystemMock<1>);
  stage1.AddSystem(SystemMock<2>);

  Vector<Stage*> stages { { &stage1 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 2);

  EXPECT_FALSE(HasCircularDependency(steps));

  EXPECT_FALSE(RunsAfter(steps, SystemMock<1>, SystemMock<2>));
  EXPECT_FALSE(RunsAfter(steps, SystemMock<2>, SystemMock<1>));
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_TwoSystemsNoQueriesTwoStages_CanRunInParallel)
{
  Stage stage1;

  stage1.AddSystem(SystemMock<1>);

  Stage stage2;

  stage2.AddSystem(SystemMock<2>);

  Vector<Stage*> stages { { &stage1, &stage2 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 2);

  EXPECT_FALSE(HasCircularDependency(steps));

  EXPECT_FALSE(RunsAfter(steps, SystemMock<1>, SystemMock<2>));
  EXPECT_FALSE(RunsAfter(steps, SystemMock<2>, SystemMock<1>));
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_TwoSystemsWithDependencySameStage_CanRunInParallel)
{
  auto system1 = SystemMock<1, MockQuery<MockData<0>>>;
  auto system2 = SystemMock<2, MockQuery<MockData<0>>>;

  Stage stage1;

  stage1.AddSystem(system1);
  stage1.AddSystem(system2);

  Vector<Stage*> stages { { &stage1 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 2);

  EXPECT_FALSE(HasCircularDependency(steps));

  EXPECT_TRUE(IsOrderedExclusive(steps, system1, system2));
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_TwoSystemsWithDependencyDifferentStage_InSequence)
{
  auto system1 = SystemMock<1, MockQuery<MockData<0>>>;
  auto system2 = SystemMock<2, MockQuery<MockData<0>>>;

  Stage stage1;

  stage1.AddSystem(system1);

  Stage stage2;

  stage2.AddSystem(system2);

  Vector<Stage*> stages { { &stage1, &stage2 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 2);

  EXPECT_FALSE(HasCircularDependency(steps));

  EXPECT_TRUE(IsOrderedExclusive(steps, system1, system2));
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_Complex4s_InCorrectSequence)
{
  auto system1 = SystemMock<1, MockQuery<MockData<0>, MockData<2>>>;
  auto system2 = SystemMock<2, MockQuery<MockData<0>, const MockData<2>>>;

  auto system3 = SystemMock<3, MockQuery<MockData<0>, MockData<3>>>;
  auto system4 = SystemMock<4, MockQuery<MockData<3>>>;

  Stage stage1;

  stage1.AddSystem(system1);
  stage1.AddSystem(system2);

  Stage stage2;

  stage2.AddSystem(system3);
  stage2.AddSystem(system4);

  Vector<Stage*> stages { { &stage1, &stage2 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 4);

  EXPECT_FALSE(HasCircularDependency(steps));

  // Stage 1

  EXPECT_TRUE(IsOrderedExclusive(steps, system1, system2));

  EXPECT_FALSE(RunsAfter(steps, system3, system1));
  EXPECT_FALSE(RunsAfter(steps, system4, system1));

  EXPECT_FALSE(RunsAfter(steps, system3, system2));
  EXPECT_FALSE(RunsAfter(steps, system4, system2));

  // Stage 2

  EXPECT_TRUE(IsOrderedExclusive(steps, system3, system4));

  EXPECT_TRUE(RunsAfter(steps, system1, system3));
  EXPECT_TRUE(RunsAfter(steps, system2, system3));

  EXPECT_FALSE(RunsAfter(steps, system1, system4));
  EXPECT_FALSE(RunsAfter(steps, system2, system4));
}

TEST(Scheduler_Algorithm_Tests, ComputeSchedulerData_Complex8s_RunsAfterDependencies)
{
  auto system1 = SystemMock<1, MockQuery<MockData<0>, MockData<1>>>;
  auto system2 = SystemMock<2, MockQuery<MockData<0>, MockData<2>>>;

  auto system3 = SystemMock<3, MockQuery<MockData<0>, const MockData<1>>>;
  auto system4 = SystemMock<4, MockQuery<MockData<3>, const MockData<2>, const MockData<1>>>;

  auto system5 = SystemMock<5, MockQuery<const MockData<0>, const MockData<3>, MockData<4>>>;
  auto system6 = SystemMock<6, MockQuery<const MockData<0>, const MockData<2>, MockData<5>>>;
  auto system7 = SystemMock<7, MockQuery<const MockData<1>, MockData<4>, MockData<5>>>;
  auto system8 = SystemMock<8, MockQuery<const MockData<0>, const MockData<5>, MockData<6>>>;

  Stage stage1;

  stage1.AddSystem(system1);
  stage1.AddSystem(system2);

  Stage stage2;

  stage2.AddSystem(system3);
  stage2.AddSystem(system4);

  Stage stage3;

  stage3.AddSystem(system5);
  stage3.AddSystem(system6);
  stage3.AddSystem(system7);
  stage3.AddSystem(system8);

  Vector<Stage*> stages { { &stage1, &stage2, &stage3 } };

  auto steps = ComputeSchedulerData(stages);

  EXPECT_EQ(steps.size(), 8);

  EXPECT_FALSE(HasCircularDependency(steps));

  // System 1

  // System 2

  // System 3

  EXPECT_TRUE(RunsAfter(steps, system1, system3));
  EXPECT_TRUE(RunsAfter(steps, system2, system3));

  // System 4

  EXPECT_TRUE(RunsAfter(steps, system1, system4));
  EXPECT_TRUE(RunsAfter(steps, system2, system4));

  // System 5

  EXPECT_TRUE(RunsAfter(steps, system1, system5));
  EXPECT_TRUE(RunsAfter(steps, system2, system5));
  EXPECT_TRUE(RunsAfter(steps, system3, system5));
  EXPECT_TRUE(RunsAfter(steps, system4, system5));

  // System 6

  EXPECT_TRUE(RunsAfter(steps, system1, system6));
  EXPECT_TRUE(RunsAfter(steps, system2, system6));
  EXPECT_TRUE(RunsAfter(steps, system3, system6));

  // System 7

  EXPECT_TRUE(RunsAfter(steps, system1, system7));

  // System 8

  EXPECT_TRUE(RunsAfter(steps, system1, system8));
  EXPECT_TRUE(RunsAfter(steps, system2, system8));
  EXPECT_TRUE(RunsAfter(steps, system3, system8));
}

} // namespace plex::tests