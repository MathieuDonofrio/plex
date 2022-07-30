#include "plex/async/sync_wait.h"

#include <thread>

#include <gmock/gmock.h>

namespace plex
{
namespace
{
  struct AsyncExecuteAwaiter
  {
    std::thread* thread;

    bool await_ready() const noexcept
    {
      return false;
    }

    void await_resume() const noexcept {}

    void await_suspend(std::coroutine_handle<> coro) noexcept
    {
      new (thread) std::thread([](std::coroutine_handle<> handle) { handle.resume(); }, coro);
    }
  };
} // namespace

TEST(SyncWaitTask_Tests, ManualWait_SyncWaitFlag_Executed)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  auto task = [&]() -> Task<>
  {
    co_await AsyncExecuteAwaiter { &thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;
  }();

  SyncWaitFlag flag;

  auto sync_wait_task = MakeTriggerTask<SyncWaitFlag>(task);

  sync_wait_task.Start(flag);

  flag.Wait();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);

  if (thread.joinable()) thread.join();
}

TEST(SyncWaitTask_Tests, ManualWait_ReturnResult_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  auto task = [&]() -> Task<int>
  {
    co_await AsyncExecuteAwaiter { &thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;

    co_return 10;
  }();

  SyncWaitFlag flag;

  auto sync_wait_task = MakeTriggerTask<SyncWaitFlag>(task);

  sync_wait_task.Start(flag);

  flag.Wait();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_EQ(sync_wait_task.Result(), 10);

  if (thread.joinable()) thread.join();
}

TEST(SyncWaitTask_Tests, ManualWait_EmbeddedTasks_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread1;
  std::thread thread2;

  auto task = [&]() -> Task<int>
  {
    co_await AsyncExecuteAwaiter { &thread1 };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    auto value = co_await [&]() -> Task<int>
    {
      co_await AsyncExecuteAwaiter { &thread2 };

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 15;
    }();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;

    co_return value + 5;
  }();

  SyncWaitFlag flag;

  auto sync_wait_task = MakeTriggerTask<SyncWaitFlag>(task);

  sync_wait_task.Start(flag);

  flag.Wait();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 2);
  EXPECT_EQ(sync_wait_task.Result(), 20);

  if (thread1.joinable()) thread1.join();
  if (thread2.joinable()) thread2.join();
}

TEST(SyncWaitTask_Tests, ManualWait_SyncWaitCounter_Executed)
{
  std::atomic_size_t count = 0;

  std::thread thread1;
  std::thread thread2;
  std::thread thread3;

  auto make_task = [&](std::thread* thread) -> Task<>
  {
    co_await AsyncExecuteAwaiter { thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;
  };

  auto task1 = make_task(&thread1);
  auto task2 = make_task(&thread1);
  auto task3 = make_task(&thread1);

  SyncWaitCounter counter(3);

  auto sync_wait_task1 = MakeTriggerTask<SyncWaitCounter>(task1);
  auto sync_wait_task2 = MakeTriggerTask<SyncWaitCounter>(task2);
  auto sync_wait_task3 = MakeTriggerTask<SyncWaitCounter>(task3);

  sync_wait_task1.Start(counter);
  sync_wait_task2.Start(counter);
  sync_wait_task3.Start(counter);

  counter.Wait();

  EXPECT_TRUE(task1.IsReady());
  EXPECT_TRUE(task2.IsReady());
  EXPECT_TRUE(task3.IsReady());

  EXPECT_EQ(count, 3);

  if (thread1.joinable()) thread1.join();
  if (thread2.joinable()) thread2.join();
  if (thread3.joinable()) thread3.join();
}

TEST(SyncWaitTask_Tests, ManualWait_MultipleReturnResults_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread1;
  std::thread thread2;
  std::thread thread3;

  auto make_task = [&](std::thread* thread) -> Task<int>
  {
    co_await AsyncExecuteAwaiter { thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;

    co_return 10;
  };

  auto task1 = make_task(&thread1);
  auto task2 = make_task(&thread1);
  auto task3 = make_task(&thread1);

  SyncWaitCounter counter(3);

  auto sync_wait_task1 = MakeTriggerTask<SyncWaitCounter>(task1);
  auto sync_wait_task2 = MakeTriggerTask<SyncWaitCounter>(task2);
  auto sync_wait_task3 = MakeTriggerTask<SyncWaitCounter>(task3);

  sync_wait_task1.Start(counter);
  sync_wait_task2.Start(counter);
  sync_wait_task3.Start(counter);

  counter.Wait();

  EXPECT_TRUE(task1.IsReady());
  EXPECT_TRUE(task2.IsReady());
  EXPECT_TRUE(task3.IsReady());

  EXPECT_EQ(count, 3);

  EXPECT_EQ(sync_wait_task1.Result(), 10);
  EXPECT_EQ(sync_wait_task2.Result(), 10);
  EXPECT_EQ(sync_wait_task3.Result(), 10);

  if (thread1.joinable()) thread1.join();
  if (thread2.joinable()) thread2.join();
  if (thread3.joinable()) thread3.join();
}

TEST(SyncWaitTask_Tests, SyncWait_VoidTaskRef_Executed)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  auto task = [&]() -> Task<>
  {
    co_await AsyncExecuteAwaiter { &thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;
  }();

  SyncWait(task);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);

  if (thread.joinable()) thread.join();
}

TEST(SyncWaitTask_Tests, SyncWait_ResultTaskRef_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  auto task = [&]() -> Task<int>
  {
    co_await AsyncExecuteAwaiter { &thread };

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;

    co_return 99;
  }();

  auto value = SyncWait(task);

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, 99);

  if (thread.joinable()) thread.join();
}

TEST(SyncWaitTask_Tests, SyncWait_VoidTask_Executed)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  SyncWait(
    [&]() -> Task<>
    {
      co_await AsyncExecuteAwaiter { &thread };

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;
    }());

  EXPECT_EQ(count, 1);

  if (thread.joinable()) thread.join();
}

TEST(SyncWaitTask_Tests, SyncWait_ResultTask_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  auto value = SyncWait(
    [&]() -> Task<int>
    {
      co_await AsyncExecuteAwaiter { &thread };

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 99;
    }());

  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, 99);

  if (thread.joinable()) thread.join();
}
} // namespace plex