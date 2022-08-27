
#include "plex/system/system.h"

#include <gmock/gmock.h>

#include "plex/async/sync_wait.h"

namespace plex::tests
{
namespace
{
  size_t void_query_mock_get_call_counter;
  size_t resources_mock_get_call_counter;
  size_t entities_mock_get_call_counter;

  template<typename... Types>
  struct ResourcesMock
  {
    static ResourcesMock Fetch(void*, Context&, Context&)
    {
      resources_mock_get_call_counter++;

      return ResourcesMock();
    }

    static consteval Array<QueryDataAccess, sizeof...(Types)> GetDataAccess() noexcept
    {
      return { QueryDataAccess {
        "resources", TypeName<Types>(), std::is_const_v<Types>, IsThreadSafe<Types>::value }... };
    }
  };

  template<typename... Components>
  struct EntitiesMock
  {
    static EntitiesMock Fetch(void*, Context&, Context&)
    {
      entities_mock_get_call_counter++;

      return EntitiesMock();
    }

    static consteval Array<QueryDataAccess, sizeof...(Components)> GetDataAccess() noexcept
    {
      return { QueryDataAccess {
        "components", TypeName<Components>(), std::is_const_v<Components>, IsThreadSafe<Components>::value }... };
    }
  };

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

static_assert(System<decltype(SystemMock1<>)>);
static_assert(System<decltype(SystemMock1<ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock1<ResourcesMock<int>>)>);
static_assert(System<decltype(SystemMock1<ResourcesMock<int, float>>)>);
static_assert(System<decltype(SystemMock1<ResourcesMock<int, float, double>>)>);
static_assert(System<decltype(SystemMock1<ResourcesMock<>, EntitiesMock<>>)>);
static_assert(System<decltype(SystemMock1<EntitiesMock<>, ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock1<EntitiesMock<>, EntitiesMock<>>)>);
static_assert(System<decltype(SystemMock1<EntitiesMock<>, EntitiesMock<>, ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock1<EntitiesMock<int>, ResourcesMock<float>>)>);
static_assert(System<
  decltype(SystemMock1<EntitiesMock<int>, ResourcesMock<float, double>, ResourcesMock<float>, EntitiesMock<long>>)>);
static_assert(System<decltype(SystemMock2<>)>);
static_assert(System<decltype(SystemMock2<ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock2<ResourcesMock<int>>)>);
static_assert(System<decltype(SystemMock2<ResourcesMock<int, float>>)>);
static_assert(System<decltype(SystemMock2<ResourcesMock<int, float, double>>)>);
static_assert(System<decltype(SystemMock2<ResourcesMock<>, EntitiesMock<>>)>);
static_assert(System<decltype(SystemMock2<EntitiesMock<>, ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock2<EntitiesMock<>, EntitiesMock<>>)>);
static_assert(System<decltype(SystemMock2<EntitiesMock<>, EntitiesMock<>, ResourcesMock<>>)>);
static_assert(System<decltype(SystemMock2<EntitiesMock<int>, ResourcesMock<float>>)>);
static_assert(System<
  decltype(SystemMock2<EntitiesMock<int>, ResourcesMock<float, double>, ResourcesMock<float>, EntitiesMock<long>>)>);

static_assert(SystemTraits<decltype(SystemMock1<>)>::IsCoroutine == true);
static_assert(SystemTraits<decltype(SystemMock2<>)>::IsCoroutine == false);
static_assert(SystemTraits<decltype(SystemMock1<ResourcesMock<>>)>::IsCoroutine == true);
static_assert(SystemTraits<decltype(SystemMock2<ResourcesMock<>>)>::IsCoroutine == false);

static_assert(SystemTraits<decltype(SystemMock1<>)>::QueryCount == 0);
static_assert(SystemTraits<decltype(SystemMock1<ResourcesMock<>>)>::QueryCount == 1);
static_assert(SystemTraits<decltype(SystemMock1<ResourcesMock<>, EntitiesMock<>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock1<ResourcesMock<int>, EntitiesMock<float>>)>::QueryCount == 2);
static_assert(
  SystemTraits<decltype(SystemMock1<ResourcesMock<int>, EntitiesMock<float>, EntitiesMock<int>>)>::QueryCount == 3);
static_assert(SystemTraits<decltype(SystemMock2<>)>::QueryCount == 0);
static_assert(SystemTraits<decltype(SystemMock2<ResourcesMock<>>)>::QueryCount == 1);
static_assert(SystemTraits<decltype(SystemMock2<ResourcesMock<>, EntitiesMock<>>)>::QueryCount == 2);
static_assert(SystemTraits<decltype(SystemMock2<ResourcesMock<int>, EntitiesMock<float>>)>::QueryCount == 2);
static_assert(
  SystemTraits<decltype(SystemMock2<ResourcesMock<int>, EntitiesMock<float>, EntitiesMock<int>>)>::QueryCount == 3);

TEST(SystemTraits_Tests, Invoke_NoQueries_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_VoidReturnAndNoQueries_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock2<>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_EmptyEntitiesQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<EntitiesMock<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_EmptyResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<ResourcesMock<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_EmptyEntitiesAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<EntitiesMock<>, ResourcesMock<>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_EntitiesWithComponentsQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<EntitiesMock<int>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_EntitiesWithComponentsAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock1<EntitiesMock<int, long>, ResourcesMock<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_VoidReturnEntitiesWithComponentsAndResourcesQuery_SystemCalled)
{
  system_call_counter = 0;

  Context context;

  auto system = SystemMock2<EntitiesMock<int, long>, ResourcesMock<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_SingleQuery_QueryGetCalled)
{
  resources_mock_get_call_counter = 0;

  Context context;

  auto system = SystemMock2<ResourcesMock<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(resources_mock_get_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_DoubleQuerySameType_QueryGetCalled)
{
  resources_mock_get_call_counter = 0;

  Context context;

  auto system = SystemMock2<ResourcesMock<float>, ResourcesMock<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(resources_mock_get_call_counter, 2);
}

TEST(SystemTraits_Tests, Invoke_DoubleQueryDifferent_QueryGetCalled)
{
  resources_mock_get_call_counter = 0;
  entities_mock_get_call_counter = 0;

  Context context;

  auto system = SystemMock2<ResourcesMock<float>, EntitiesMock<float>>;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(resources_mock_get_call_counter, 1);
  EXPECT_EQ(entities_mock_get_call_counter, 1);
}

TEST(SystemTraits_Tests, Invoke_ImplicitGlobal_CorrectData)
{
  Context context;
  context.Emplace<int>(10);

  static int call_count;
  call_count = 0;

  struct TestSystem
  {
    static void Run(const int& value)
    {
      call_count++;
      EXPECT_EQ(value, 10);
    }
  };

  auto system = TestSystem::Run;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(call_count, 1);
}

TEST(SystemTraits_Tests, Invoke_ExplicitGlobal_CorrectData)
{
  Context context;
  context.Emplace<int>(10);

  static int call_count;
  call_count = 0;

  struct TestSystem
  {
    static void Run(Global<int> value)
    {
      call_count++;
      EXPECT_EQ(value, 10);
    }
  };

  auto system = TestSystem::Run;

  auto task = SystemTraits<decltype(system)>::Invoke(system, context, context);

  SyncWait(task);

  EXPECT_EQ(call_count, 1);
}

TEST(SystemTraits_Tests, GetDataAccess_ImplicitGlobalByCopy_IsReadOnly)
{
  struct TestSystem
  {
    static void Run(int) {}
  };

  auto system = TestSystem::Run;

  auto access = SystemTraits<decltype(system)>::GetDataAccess();

  EXPECT_EQ(access.size(), 1);
  EXPECT_TRUE(access[0].read_only);
}

TEST(SystemExecutor_Tests, Constructor_Coroutine)
{
  auto system = SystemMock1<ResourcesMock<>, EntitiesMock<>>;

  SystemExecutor executor(system);

  EXPECT_EQ(system, executor.Handle());
}

TEST(SystemExecutor_Tests, Constructor_Subroutine)
{
  auto system = SystemMock2<ResourcesMock<>, EntitiesMock<>>;

  SystemExecutor executor(system);

  EXPECT_EQ(system, executor.Handle());
}

TEST(SystemExecutor_Tests, Execute_NoData_SystemCalled)
{
  system_call_counter = 0;

  auto system = SystemMock2<ResourcesMock<>, EntitiesMock<>>;

  SystemExecutor executor(system);

  Context context;

  auto task = executor(context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemExecutor_Tests, Execute_WithData_SystemCalled)
{
  system_call_counter = 0;

  auto system = SystemMock2<ResourcesMock<int, float>, EntitiesMock<double>>;

  SystemExecutor executor(system);

  Context context;

  auto task = executor(context, context);

  SyncWait(task);

  EXPECT_EQ(system_call_counter, 1);
}

TEST(SystemObject_Tests, Constructor_Coroutine)
{
  auto system = SystemMock1<ResourcesMock<>, EntitiesMock<>>;

  SystemObject object(system);

  EXPECT_EQ(system, object.Handle());
}

TEST(SystemObject_Tests, Constructor_Subroutine)
{
  auto system = SystemMock2<ResourcesMock<>, EntitiesMock<>>;

  SystemObject object(system);

  EXPECT_EQ(system, object.Handle());
}

TEST(SystemObject_Tests, HasDependency_SystemNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<>>;
  auto system2 = SystemMock2<>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_SimpleNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<>>;
  auto system2 = SystemMock2<ResourcesMock<>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_MultipleQueriesNoDependencies_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<>, EntitiesMock<>>;
  auto system2 = SystemMock2<ResourcesMock<>, EntitiesMock<>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_WriteWrite_Dependency)
{
  auto system1 = SystemMock2<ResourcesMock<int>>;
  auto system2 = SystemMock2<ResourcesMock<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_ReadRead_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<const int>>;
  auto system2 = SystemMock2<ResourcesMock<const int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_WriteRead_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<int>>;
  auto system2 = SystemMock2<ResourcesMock<const int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_ReadWrite_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<const int>>;
  auto system2 = SystemMock2<ResourcesMock<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_WriteWriteDifferentCategory_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<int>, EntitiesMock<>>;
  auto system2 = SystemMock2<ResourcesMock<>, EntitiesMock<int>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_Complex_NoDependency)
{
  auto system1 = SystemMock2<ResourcesMock<const int>, EntitiesMock<const float, int>>;
  auto system2 = SystemMock2<ResourcesMock<const int, float>, EntitiesMock<const float>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_Complex_Dependency)
{
  auto system1 = SystemMock2<ResourcesMock<const int>, EntitiesMock<const float, int>>;
  auto system2 = SystemMock2<ResourcesMock<int, float>, EntitiesMock<const float>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_TRUE(object1.HasDependency(object2));
}

TEST(SystemObject_Tests, HasDependency_WriteWriteThreadSafe_NoDependency)
{
  struct ThreadSafeRessource
  {
    using IsThreadSafe = std::true_type;

    size_t value;
  };

  auto system1 = SystemMock2<ResourcesMock<ThreadSafeRessource>>;
  auto system2 = SystemMock2<ResourcesMock<ThreadSafeRessource>>;

  SystemObject object1(system1);
  SystemObject object2(system2);

  EXPECT_FALSE(object1.HasDependency(object2));
}
} // namespace plex::tests