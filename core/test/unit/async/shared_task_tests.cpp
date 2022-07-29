#include "plex/async/shared_task.h"

#include <gtest/gtest.h>

#include "plex/async/async_latch.h"
#include "plex/async/sync_wait.h"
#include "plex/async/thread_pool.h"
#include "plex/async/when_all.h"

namespace plex::tests
{
TEST(SharedTask_Tests, IsReady_VoidAndNotStarted_NotReady)
{
  auto task = [&]() -> SharedTask<> { co_return; }();

  EXPECT_FALSE(task.IsReady());
}

TEST(SharedTask_Tests, IsReady_ResultAndNotStarted_NotReady)
{
  auto task = [&]() -> SharedTask<int> { co_return 10; }();

  EXPECT_FALSE(task.IsReady());
}

TEST(SharedTask_Tests, CoAwait_Void_Ready)
{
  size_t count = 0;

  auto task = [&]() -> SharedTask<>
  {
    count++;
    co_return;
  }();

  SyncWait(task);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(SharedTask_Tests, CoAwait_Result_ReadyAndCorrectValue)
{
  size_t count = 0;

  auto task = [&]() -> SharedTask<int>
  {
    count++;
    co_return 10;
  }();

  auto value = SyncWait(task);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, 10);
}

TEST(SharedTask_Tests, CoAwait_MultipleSync_NoStackOverflow)
{
  const size_t amount = 1'000'000;

  auto make_task = [&]() -> SharedTask<int> { co_return 1; };

  size_t result = 0;

  SyncWait(
    [&]() -> Task<>
    {
      for (size_t i = 0; i < amount; ++i)
      {
        result += co_await make_task();
      }
    }());

  EXPECT_EQ(result, amount);
}

TEST(SharedTask_Tests, CoAwait_VoidAndMultipleAwaiters_CorrectlyWaited)
{
  AsyncLatch latch(1);

  size_t count = 0;

  auto shared_task = [&]() -> SharedTask<>
  {
    co_await latch;
    count++;
  }();

  auto make_shared_task_waiter = [&](SharedTask<> task) -> Task<> { co_await task; };

  SyncWait(WhenAll(make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    [&]() -> Task<>
    {
      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });
      EXPECT_EQ(count, 0);

      latch.CountDown();

      co_return;
    }()));

  EXPECT_EQ(count, 1);
}

TEST(SharedTask_Tests, CoAwait_ResultAndMultipleAwaiters_CorrectlyWaited)
{
  AsyncLatch latch(1);

  size_t count = 0;

  auto shared_task = [&]() -> SharedTask<int>
  {
    co_await latch;
    count++;
    co_return 1;
  }();

  auto make_shared_task_waiter = [&](SharedTask<int> task) -> Task<>
  {
    auto result = co_await task;
    EXPECT_EQ(result, 1);
  };

  SyncWait(WhenAll(make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    make_shared_task_waiter(shared_task),
    [&]() -> Task<>
    {
      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });
      EXPECT_EQ(count, 0);

      latch.CountDown();

      co_return;
    }()));

  EXPECT_EQ(count, 1);
}

} // namespace plex::tests