#include "genebits/engine/async/async_latch.h"

#include <gmock/gmock.h>

#include "genebits/engine/async/sync_wait.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/async/thread_pool.h"
#include "genebits/engine/async/when_all.h"

namespace plex
{
TEST(AsyncLatch_Tests, Constructor_Zero_Released)
{
  AsyncLatch latch(0);

  EXPECT_TRUE(latch.IsReleased());
}

TEST(AsyncLatch_Tests, Constructor_NonZero_NotReleased)
{
  AsyncLatch latch(1);

  EXPECT_FALSE(latch.IsReleased());

  latch.CountDown();
}

TEST(AsyncLatch_Tests, CountDown_One_Released)
{
  AsyncLatch latch(1);

  EXPECT_FALSE(latch.IsReleased());

  latch.CountDown();

  EXPECT_TRUE(latch.IsReleased());
}

TEST(AsyncLatch_Tests, CountDown_Two_Released)
{
  AsyncLatch latch(2);

  EXPECT_FALSE(latch.IsReleased());

  latch.CountDown();

  EXPECT_FALSE(latch.IsReleased());

  latch.CountDown();

  EXPECT_TRUE(latch.IsReleased());
}

TEST(AsyncLatch_Tests, CountDown_OneNessesaryTwoProvided_Released)
{
  AsyncLatch latch(1);

  EXPECT_FALSE(latch.IsReleased());

  latch.CountDown();

  EXPECT_TRUE(latch.IsReleased());

  latch.CountDown();

  EXPECT_TRUE(latch.IsReleased());
}

TEST(AsyncLatch_Tests, CountDown_Many_Released)
{
  size_t amount = 100;

  AsyncLatch latch(100);

  for (size_t i = 0; i < amount; i++)
  {
    EXPECT_FALSE(latch.IsReleased());

    latch.CountDown();
  }

  EXPECT_TRUE(latch.IsReleased());
}

TEST(AsyncLatch_Tests, CoAwait_ZeroInitialized_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(0);

  std::atomic_size_t count = 0;

  auto make_task = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    count++;
  };

  SyncWait(make_task());

  EXPECT_EQ(count, 1);
}

TEST(AsyncLatch_Tests, CoAwait_OneAndPreCountDown_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(1);

  std::atomic_size_t count = 0;

  auto make_task = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    count++;
  };

  latch.CountDown();

  SyncWait(make_task());

  EXPECT_EQ(count, 1);
}

TEST(AsyncLatch_Tests, CoAwait_OneDriverAndOneWaiter_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(1);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(), make_driver()));

  EXPECT_EQ(drivers, 1);
  EXPECT_EQ(waiters, 1);
}

TEST(AsyncLatch_Tests, CoAwait_ManyDriverAndOneWaiter_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(1);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(), make_driver(), make_driver(), make_driver(), make_driver()));

  EXPECT_EQ(drivers, 4);
  EXPECT_EQ(waiters, 1);
}

TEST(AsyncLatch_Tests, CoAwait_OneDriverAndManyWaiter_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(1);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(), make_waiter(), make_waiter(), make_waiter(), make_driver()));

  EXPECT_EQ(drivers, 1);
  EXPECT_EQ(waiters, 4);
}

TEST(AsyncLatch_Tests, CoAwait_ManyDriverAndManyWaiter_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(1);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(),
    make_waiter(),
    make_waiter(),
    make_waiter(),
    make_driver(),
    make_driver(),
    make_driver(),
    make_driver()));

  EXPECT_EQ(drivers, 4);
  EXPECT_EQ(waiters, 4);
}

TEST(AsyncLatch_Tests, CoAwait_TwoDriverOnWaiterTwoCount_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(2);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(), make_driver(), make_driver()));

  EXPECT_EQ(drivers, 2);
  EXPECT_EQ(waiters, 1);
}

TEST(AsyncLatch_Tests, CoAwait_TwoDriverTwoWaiterTwoCount_Continue)
{
  ThreadPool pool;

  AsyncLatch latch(2);

  std::atomic_size_t drivers = 0;
  std::atomic_size_t waiters = 0;

  auto make_driver = [&]() -> Task<>
  {
    co_await pool.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    latch.CountDown();

    drivers++;
  };

  auto make_waiter = [&]() -> Task<>
  {
    co_await pool.Schedule();

    co_await latch;

    waiters++;
  };

  SyncWait(WhenAll(make_waiter(), make_waiter(), make_driver(), make_driver()));

  EXPECT_EQ(drivers, 2);
  EXPECT_EQ(waiters, 2);
}
} // namespace plex