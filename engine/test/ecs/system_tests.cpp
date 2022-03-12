
#include "genebits/engine/ecs/system.h"

#include <gmock/gmock.h>

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
} // namespace

TEST(System_Tests, GetDataAccess_ReadWriteComponent_Correct)
{
  MockSystem<int> system;

  const SystemDataAccessList& access = system.GetDataAccess();

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_FALSE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_ReadOnlyComponent_Correct)
{
  MockSystem<const int> system;

  const SystemDataAccessList& access = system.GetDataAccess();

  EXPECT_EQ(access.size(), 1);

  EXPECT_EQ(access[0].id, GetComponentId<int>());
  EXPECT_TRUE(access[0].read_only);
}

TEST(System_Tests, GetDataAccess_MultipleComponents_Correct)
{
  MockSystem<int, const float, bool, const double> system;

  const SystemDataAccessList& access = system.GetDataAccess();

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
} // namespace genebits::engine::tests