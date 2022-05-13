#include "genebits/engine/ecs/query.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
namespace
{
  template<typename... Components>
  struct QueryMock : public QueryDataAccessFactory<QueryCategory::None, Components...>
  {
    static QueryMock Get([[maybe_unused]] Registry& registry)
    {
      return QueryMock();
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
  SystemDataAccessList auto array = QueryMock<>::GetDataAccess();

  EXPECT_EQ(array.size(), 0);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleReadOnly_CorrectDataAccess)
{
  SystemDataAccessList auto array = QueryMock<const int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeInfo<const int>::Name());
  EXPECT_EQ(array[0].category_id, QueryCategory::None);
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleReadWrite_CorrectDataAccessy)
{
  SystemDataAccessList auto array = QueryMock<int>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeInfo<int>::Name());
  EXPECT_EQ(array[0].category_id, QueryCategory::None);
  EXPECT_FALSE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_SingleThreadSafe_CorrectDataAccessy)
{
  SystemDataAccessList auto array = QueryMock<ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 1);

  EXPECT_EQ(array[0].name, TypeInfo<ThreadSafeType>::Name());
  EXPECT_EQ(array[0].category_id, QueryCategory::None);
  EXPECT_FALSE(array[0].read_only);
  EXPECT_TRUE(array[0].thread_safe);
}

TEST(QueryDataAccessFactoryTests, GetDataAccess_Multiple_CorrectDataAccesses)
{
  SystemDataAccessList auto array = QueryMock<const int, float, const ThreadSafeType>::GetDataAccess();

  EXPECT_EQ(array.size(), 3);

  EXPECT_EQ(array[0].name, TypeInfo<const int>::Name());
  EXPECT_EQ(array[0].category_id, QueryCategory::None);
  EXPECT_TRUE(array[0].read_only);
  EXPECT_FALSE(array[0].thread_safe);

  EXPECT_EQ(array[1].name, TypeInfo<float>::Name());
  EXPECT_EQ(array[1].category_id, QueryCategory::None);
  EXPECT_FALSE(array[1].read_only);
  EXPECT_FALSE(array[1].thread_safe);

  EXPECT_EQ(array[2].name, TypeInfo<const ThreadSafeType>::Name());
  EXPECT_EQ(array[2].category_id, QueryCategory::None);
  EXPECT_TRUE(array[2].read_only);
  EXPECT_TRUE(array[2].thread_safe);
}

} // namespace genebits::engine::tests