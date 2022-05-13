
#include "genebits/engine/ecs/system.h"

#include <gmock/gmock.h>

#include "genebits/engine/async/sync_wait.h"

namespace genebits::engine::tests
{
namespace
{
  size_t system_call_counter;

  template<typename... Queries>
  Task<void> SystemMock1(Queries&&...)
  {
    system_call_counter++;
    co_return;
  }

  template<typename... Queries>
  void SystemMock2(Queries&&...)
  {
    system_call_counter++;
  }
} // namespace

static_assert(Query<int> == false);
static_assert(Query<std::tuple<>> == false);
static_assert(Query<Resources<>>);
static_assert(Query<Entities<>>);
static_assert(Query<Resources<int>>);
static_assert(Query<Entities<float>>);
static_assert(Query<Resources<int, double>>);
static_assert(Query<Entities<float, double>>);
static_assert(Query<Resources<>&&>);

static_assert(System<decltype(SystemMock1<>)>);
static_assert(System<decltype(SystemMock1<Resources<>>)>);
static_assert(System<decltype(SystemMock1<Resources<int>>)>);
static_assert(System<decltype(SystemMock1<Resources<int, float>>)>);
static_assert(System<decltype(SystemMock1<Resources<int, float, double>>)>);
static_assert(System<decltype(SystemMock1<Resources<>, Entities<>>)>);
static_assert(System<decltype(SystemMock1<Entities<>, Resources<>>)>);
static_assert(System<decltype(SystemMock1<Entities<>, Entities<>>)>);
static_assert(System<decltype(SystemMock1<Entities<>, Entities<>, Resources<>>)>);
static_assert(System<decltype(SystemMock1<Entities<int>, Resources<float>>)>);
static_assert(System<decltype(SystemMock1<Entities<int>, Resources<float, double>, Resources<float>, Entities<long>>)>);
static_assert(System<decltype(SystemMock2<>)>);
static_assert(System<decltype(SystemMock2<Resources<>>)>);
static_assert(System<decltype(SystemMock2<Resources<int>>)>);
static_assert(System<decltype(SystemMock2<Resources<int, float>>)>);
static_assert(System<decltype(SystemMock2<Resources<int, float, double>>)>);
static_assert(System<decltype(SystemMock2<Resources<>, Entities<>>)>);
static_assert(System<decltype(SystemMock2<Entities<>, Resources<>>)>);
static_assert(System<decltype(SystemMock2<Entities<>, Entities<>>)>);
static_assert(System<decltype(SystemMock2<Entities<>, Entities<>, Resources<>>)>);
static_assert(System<decltype(SystemMock2<Entities<int>, Resources<float>>)>);
static_assert(System<decltype(SystemMock2<Entities<int>, Resources<float, double>, Resources<float>, Entities<long>>)>);

static_assert(SystemTraits<decltype(SystemMock1<>)>::IsCoroutine == true);
static_assert(SystemTraits<decltype(SystemMock2<>)>::IsCoroutine == false);
static_assert(SystemTraits<decltype(SystemMock1<Resources<>>)>::IsCoroutine == true);
static_assert(SystemTraits<decltype(SystemMock2<Resources<>>)>::IsCoroutine == false);

static_assert(SystemTraits<decltype(SystemMock1<>)>::QueryCount == 0);
static_assert(SystemTraits<decltype(SystemMock1<Resources<>>)>::QueryCount == 1);
static_assert(SystemTraits<decltype(SystemMock1<Resources<>, Entities<>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock1<Resources<int>, Entities<float>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock1<Resources<int>, Entities<float>, Entities<int>>)>::QueryCount == 3);
static_assert(SystemTraits<decltype(SystemMock2<>)>::QueryCount == 0);
static_assert(SystemTraits<decltype(SystemMock2<Resources<>>)>::QueryCount == 1);
static_assert(SystemTraits<decltype(SystemMock2<Resources<>, Entities<>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock2<Resources<int>, Entities<float>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock2<Resources<int>, Entities<float>, Entities<int>>)>::QueryCount == 3);

static_assert(SystemTraits<decltype(SystemMock1<>)>::ContainsQueryType<Resources>::value == false);
static_assert(SystemTraits<decltype(SystemMock1<Resources<>>)>::ContainsQueryType<Resources>::value == true);
static_assert(SystemTraits<decltype(SystemMock1<Resources<int>>)>::ContainsQueryType<Resources>::value == true);
static_assert(SystemTraits<decltype(SystemMock1<Resources<int, float>>)>::ContainsQueryType<Resources>::value == true);
static_assert(SystemTraits<decltype(SystemMock1<Entities<>>)>::ContainsQueryType<Resources>::value == false);
static_assert(SystemTraits<decltype(SystemMock1<Entities<int>>)>::ContainsQueryType<Resources>::value == false);
static_assert(
  SystemTraits<decltype(SystemMock1<Resources<>, Entities<>>)>::ContainsQueryType<Resources>::value == true);
static_assert(
  SystemTraits<decltype(SystemMock1<Resources<int>, Entities<>>)>::ContainsQueryType<Resources>::value == true);
static_assert(
  SystemTraits<decltype(SystemMock1<Resources<>, Entities<int>>)>::ContainsQueryType<Resources>::value == true);
static_assert(
  SystemTraits<decltype(SystemMock1<Resources<int>, Entities<float>>)>::ContainsQueryType<Resources>::value == true);

TEST(SystemTraits, Invoke_NoQueries_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_VoidReturnAndNoQueries_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock2<>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_EmptyEntitiesQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<Entities<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_EmptyResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<Resources<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_EmptyEntitiesAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<Entities<>, Resources<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_EntitiesWithComponentsQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<Entities<int>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_EntitiesWithComponentsAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock1<Entities<int, long>, Resources<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits, Invoke_VoidReturnEntitiesWithComponentsAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Registry registry;

  auto system = SystemMock2<Entities<int, long>, Resources<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemExecutor, Constructor_Coroutine)
{
  auto system = SystemMock1<Resources<>, Entities<>>;

  SystemExecutor executor(system);

  EXPECT_EQ(system, executor.Handle());
}

TEST(SystemExecutor, Constructor_Subroutine)
{
  auto system = SystemMock2<Resources<>, Entities<>>;

  SystemExecutor executor(system);

  EXPECT_EQ(system, executor.Handle());
}

TEST(SystemExecutor, Execute_NoData_SystemCalled)
{
  system_call_counter = 0;

  auto system = SystemMock2<Resources<>, Entities<>>;

  SystemExecutor executor(system);

  Registry registry;

  auto task = executor(registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemExecutor, Execute_WithData_SystemCalled)
{
  system_call_counter = 0;

  auto system = SystemMock2<Resources<int, float>, Entities<double>>;

  SystemExecutor executor(system);

  Registry registry;

  auto task = executor(registry);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemObject, Constructor_Coroutine)
{
  auto system = SystemMock1<Resources<>, Entities<>>;

  SystemObject object(system);

  EXPECT_EQ(system, object.Handle());
}

TEST(SystemObject, Constructor_Subroutine)
{
  auto system = SystemMock2<Resources<>, Entities<>>;

  SystemObject object(system);

  EXPECT_EQ(system, object.Handle());
}

TEST(SystemObject, HasDependency_SingleQueryVoidSystemNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<Resources<>>;
  auto system2 = SystemMock2<>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_SingleQueryNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<Resources<>>;
  auto system2 = SystemMock2<Resources<>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_MultipleQueriesNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<Resources<>, Entities<>>;
  auto system2 = SystemMock2<Resources<>, Entities<>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_WriteWrite_Dependency)
{
  auto system1 = SystemMock2<Resources<int>>;
  auto system2 = SystemMock2<Resources<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_ReadRead_NoDependency)
{
  auto system1 = SystemMock2<Resources<const int>>;
  auto system2 = SystemMock2<Resources<const int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_WriteRead_NoDependency)
{
  auto system1 = SystemMock2<Resources<int>>;
  auto system2 = SystemMock2<Resources<const int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_ReadWrite_NoDependency)
{
  auto system1 = SystemMock2<Resources<const int>>;
  auto system2 = SystemMock2<Resources<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_WriteWriteDifferentCategory_NoDependency)
{
  auto system1 = SystemMock2<Resources<int>, Entities<>>;
  auto system2 = SystemMock2<Resources<>, Entities<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_Complex_NoDependency)
{
  auto system1 = SystemMock2<Resources<const int>, Entities<const float, int>>;
  auto system2 = SystemMock2<Resources<const int, float>, Entities<const float>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_Complex_Dependency)
{
  auto system1 = SystemMock2<Resources<const int>, Entities<const float, int>>;
  auto system2 = SystemMock2<Resources<int, float>, Entities<const float>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject, HasDependency_WriteWriteThreadSafe_NoDependency)
{
  struct ThreadSafeRessource
  {
    using IsThreadSafe = std::true_type;

    size_t value;
  };

  auto system1 = SystemMock2<Resources<ThreadSafeRessource>>;
  auto system2 = SystemMock2<Resources<ThreadSafeRessource>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}
} // namespace genebits::engine::tests