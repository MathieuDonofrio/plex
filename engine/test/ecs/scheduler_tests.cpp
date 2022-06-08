#include "genebits/engine/ecs/scheduler.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  template<typename... Components>
  struct MockQuery : public QueryDataAccessFactory<MockQuery<Components...>, Components...>
  {
    static MockQuery FetchData([[maybe_unused]] Context& context)
    {
      return MockQuery();
    }

    static constexpr std::string_view GetCategory()
    {
      return "Test";
    }
  };

  template<size_t id>
  std::atomic_size_t& SystemMockCallCount()
  {
    static std::atomic_size_t call_count { 0 };
    return call_count;
  }

  template<size_t id, Query... queries>
  Task<void> SystemMock()
  {
    SystemMockCallCount<id>()++;
    co_return;
  }
} // namespace

TEST(Scheduler_Tests, Constructor_Nothing_NoSystems)
{
  auto stage1 = MakeRef<Stage>();

  stage1->AddSystem(SystemMock<0>);

  EXPECT_EQ(stage1->GetSystemCount(), 1);
}
} // namespace genebits::engine::tests