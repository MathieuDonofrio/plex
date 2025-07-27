#include "plex/system/query.h"

#include <gmock/gmock.h>

namespace plex::tests
{
namespace
{
  template<typename... Components>
  struct QueryMock
  {
    static QueryMock Fetch(void*, Context&, Context&)
    {
      return QueryMock();
    }

    static consteval std::array<QueryDataAccess, sizeof...(Components)> GetDataAccess() noexcept
    {
      return { QueryDataAccess {
        TypeName<Components>(), {}, std::is_const_v<Components>, IsThreadSafe<Components>::value }... };
    }
  };

  struct ThreadSafeType
  {
    using IsThreadSafe = std::true_type;
  };

} // namespace

static_assert(Query<int> == false);
static_assert(Query<std::tuple<>> == false);
static_assert(Query<QueryMock<>>);
static_assert(Query<QueryMock<>>);
static_assert(Query<QueryMock<int>>);
static_assert(Query<QueryMock<float>>);
static_assert(Query<QueryMock<int, double>>);
static_assert(Query<QueryMock<float, double>>);
static_assert(Query<QueryMock<>>);

TEST(BasicQueryDataAccessFactory_Tests, GetDataAccess_Nothing_ReturnsEmpty)
{
  QueryDataAccessRange auto array = QueryMock<>::GetDataAccess();

  EXPECT_EQ(array.size(), 0);
}

TEST(BasicQueryDataAccessFactory_Tests, GetDataAccess_SingleReadOnly_CorrectDataAccess)
{
  QueryDataAccessRange auto array = QueryMock<const int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].source, TypeName<const int>());
  EXPECT_EQ(array[0].section, std::string_view {});
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(BasicQueryDataAccessFactory_Tests, GetDataAccess_SingleReadWrite_CorrectDataAccessy)
{
  QueryDataAccessRange auto array = QueryMock<int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].source, TypeName<int>());
  EXPECT_EQ(array[0].section, std::string_view {});
  EXPECT_FALSE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(BasicQueryDataAccessFactory_Tests, GetDataAccess_SingleThreadSafe_CorrectDataAccessy)
{
  QueryDataAccessRange auto array = QueryMock<ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].source, TypeName<ThreadSafeType>());
  EXPECT_EQ(array[0].section, std::string_view {});
  EXPECT_FALSE(array[0].read_only);
  EXPECT_TRUE(array[0].thread_safe);
}

TEST(BasicQueryDataAccessFactory_Tests, GetDataAccess_Multiple_CorrectDataAccesses)
{
  QueryDataAccessRange auto array = QueryMock<const int, float, const ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 3);

  EXPECT_EQ(array[0].source, TypeName<const int>());
  EXPECT_EQ(array[0].section, std::string_view {});
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);

  EXPECT_EQ(array[1].source, TypeName<float>());
  EXPECT_EQ(array[1].section, std::string_view {});
  EXPECT_FALSE(array[1].read_only);
  EXPECT_FALSE(array[1].thread_safe);

  EXPECT_EQ(array[2].source, TypeName<const ThreadSafeType>());
  EXPECT_EQ(array[2].section, std::string_view {});
  EXPECT_TRUE(array[2].read_only);
  EXPECT_TRUE(array[2].thread_safe);
}

TEST(Global_Tests, Fetch_SingleExistsInContext_CorrectValue)
{
  Context dummy_context;

  Context context;
  context.Emplace<int>(10);

  auto local = Global<int>::Fetch(nullptr, context, dummy_context);

  EXPECT_EQ(*local, 10);
}

TEST(Global_Tests, Fetch_SingleReadOnlyExistsInContext_CorrectValue)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });

  auto local = Global<const TestStruct>::Fetch(nullptr, context, dummy_context);

  EXPECT_EQ(local->value, 10);
}

TEST(Global_Tests, Fetch_Multiple_CorrectValues)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });
  context.Emplace<double>(0.5);
  context.Emplace<int>(99);

  auto local = Global<const TestStruct, double, const int>::Fetch(nullptr, context, dummy_context);

  EXPECT_EQ(std::get<const TestStruct>(local).value, 10);
  EXPECT_EQ(std::get<double>(local), 0.5);
  EXPECT_EQ(std::get<const int>(local), 99);
}

TEST(Global_Tests, Fetch_MultipleStructuedBindings_CorrectValues)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });
  context.Emplace<double>(0.5);
  context.Emplace<int>(99);

  auto local = Global<const TestStruct, double, const int>::Fetch(nullptr, context, dummy_context);

  auto [a, b, c] = local;

  EXPECT_EQ(a.value, 10);
  EXPECT_EQ(b, 0.5);
  EXPECT_EQ(c, 99);
}

TEST(Local_Tests, Fetch_SingleExistsInContext_CorrectValue)
{
  Context dummy_context;

  Context context;
  context.Emplace<int>(10);

  auto local = Local<int>::Fetch(nullptr, dummy_context, context);

  EXPECT_EQ(*local, 10);
}

TEST(Local_Tests, Fetch_SingleReadOnlyExistsInContext_CorrectValue)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });

  auto local = Local<const TestStruct>::Fetch(nullptr, dummy_context, context);

  EXPECT_EQ(local->value, 10);
}

TEST(Local_Tests, Fetch_Multiple_CorrectValues)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });
  context.Emplace<double>(0.5);
  context.Emplace<int>(99);

  auto local = Local<const TestStruct, double, const int>::Fetch(nullptr, dummy_context, context);

  EXPECT_EQ(std::get<const TestStruct>(local).value, 10);
  EXPECT_EQ(std::get<double>(local), 0.5);
  EXPECT_EQ(std::get<const int>(local), 99);
}

TEST(Local_Tests, Fetch_MultipleStructuedBindings_CorrectValues)
{
  struct TestStruct
  {
    int value;
  };

  Context dummy_context;

  Context context;
  context.Emplace<TestStruct>(TestStruct { 10 });
  context.Emplace<double>(0.5);
  context.Emplace<int>(99);

  auto local = Local<const TestStruct, double, const int>::Fetch(nullptr, dummy_context, context);

  auto [a, b, c] = local;

  EXPECT_EQ(a.value, 10);
  EXPECT_EQ(b, 0.5);
  EXPECT_EQ(c, 99);
}
} // namespace plex::tests