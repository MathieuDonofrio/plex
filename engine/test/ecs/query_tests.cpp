#include "genebits/engine/ecs/query.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
namespace
{
  template<typename... Components>
  struct QueryMock : public QueryDataAccessFactory<QueryMock<Components...>, Components...>
  {
    static QueryMock FetchData(void*, Context&, Context&)
    {
      return QueryMock();
    }

    static constexpr std::string_view GetCategory()
    {
      return "Test";
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
static_assert(Query<QueryMock<>&&>);

TEST(QueryDataAccessFactoryTests, GetDataAccess_Nothing_ReturnsEmpty)
{
  QueryDataAccessList auto array = QueryMock<>::GetDataAccess();

  EXPECT_EQ(array.size(), 0);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleReadOnly_CorrectDataAccess)
{
  QueryDataAccessList auto array = QueryMock<const int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeName<const int>());
  EXPECT_EQ(array[0].category, "Test");
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleReadWrite_CorrectDataAccessy)
{
  QueryDataAccessList auto array = QueryMock<int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeName<int>());
  EXPECT_EQ(array[0].category, "Test");
  EXPECT_FALSE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleThreadSafe_CorrectDataAccessy)
{
  QueryDataAccessList auto array = QueryMock<ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeName<ThreadSafeType>());
  EXPECT_EQ(array[0].category, "Test");
  EXPECT_FALSE(array[0].read_only);
  EXPECT_TRUE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_Multiple_CorrectDataAccesses)
{
  QueryDataAccessList auto array = QueryMock<const int, float, const ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 3);

  EXPECT_EQ(array[0].name, TypeName<const int>());
  EXPECT_EQ(array[0].category, "Test");
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);

  EXPECT_EQ(array[1].name, TypeName<float>());
  EXPECT_EQ(array[1].category, "Test");
  EXPECT_FALSE(array[1].read_only);
  EXPECT_FALSE(array[1].thread_safe);

  EXPECT_EQ(array[2].name, TypeName<const ThreadSafeType>());
  EXPECT_EQ(array[2].category, "Test");
  EXPECT_TRUE(array[2].read_only);
  EXPECT_TRUE(array[2].thread_safe);
}

} // namespace genebits::engine::tests