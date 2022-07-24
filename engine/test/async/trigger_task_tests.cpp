#include "genebits/engine/async/trigger_task.h"

#include <thread>

#include <gmock/gmock.h>

namespace plex
{
namespace
{
  class MockTrigger
  {
  public:
    MOCK_METHOD(void, Fire, (), ());
  };
} // namespace

TEST(TriggerTask_Tests, MakeTriggerTask_SyncFire_FiredAndExecuted)
{
  size_t count = 0;

  auto task = [&]() -> Task<>
  {
    count++;
    co_return;
  }();

  MockTrigger trigger;

  EXPECT_CALL(trigger, Fire).Times(1);

  auto trigger_task = MakeTriggerTask<MockTrigger>(task);

  trigger_task.Start(trigger);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(TriggerTask_Tests, MakeTriggerTask_SyncFireAndResult_Fired)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    count++;
    co_return 99;
  }();

  MockTrigger trigger;

  EXPECT_CALL(trigger, Fire).Times(1);

  auto trigger_task = MakeTriggerTask<MockTrigger>(task);

  trigger_task.Start(trigger);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(TriggerTask_Tests, MakeTriggerTask_LValueResult_CorrectValue)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    count++;
    co_return 101;
  }();

  MockTrigger trigger;

  EXPECT_CALL(trigger, Fire).Times(1);

  auto trigger_task = MakeTriggerTask<MockTrigger>(task);

  trigger_task.Start(trigger);

  EXPECT_EQ(trigger_task.Result(), 101);
}

TEST(TriggerTask_Tests, MakeTriggerTask_RValueResult_CorrectValue)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    count++;
    co_return 102;
  }();

  MockTrigger trigger;

  EXPECT_CALL(trigger, Fire).Times(1);

  auto trigger_task = MakeTriggerTask<MockTrigger>(task);

  trigger_task.Start(trigger);

  EXPECT_EQ(std::move(trigger_task).Result(), 102);
}
} // namespace plex