#include "plex/async/task.h"

#include <thread>

#include <gtest/gtest.h>

#include "plex/async/sync_wait.h"

namespace plex::tests
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

TEST(Task_Tests, IsReady_VoidAndNotStarted_NotReady)
{
  auto task = [&]() -> Task<> { co_return; }();

  EXPECT_FALSE(task.IsReady());
}

TEST(Task_Tests, IsReady_ResultAndNotStarted_NotReady)
{
  auto task = [&]() -> Task<int> { co_return 10; }();

  EXPECT_FALSE(task.IsReady());
}

TEST(Task_Tests, Eject_VoidAndSync_Executed)
{
  size_t count = 0;

  auto task = [&]() -> Task<>
  {
    count++;
    co_return;
  }();

  task.Eject();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(Task_Tests, Eject_ResultAndSync_Executed)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    count++;
    co_return 10;
  }();

  task.Eject();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(Task_Tests, Poll_VoidAndSync_Executed)
{
  size_t count = 0;

  auto task = [&]() -> Task<>
  {
    count++;
    co_return;
  }();

  task.Eject();

  task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
}

TEST(Task_Tests, Poll_ResultAndSync_CorrectValue)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    count++;
    co_return 10;
  }();

  task.Eject();

  auto value = task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, 10);
}

TEST(Task_Tests, CoAwait_ResultAndSync_CorrectValue)
{
  size_t count = 0;

  auto task = [&]() -> Task<int>
  {
    int value = co_await [&]() -> Task<int>
    {
      count++;
      co_return 10;
    }();

    count++;

    co_return value;
  }();

  task.Eject();

  auto value = task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 2);
  EXPECT_EQ(value, 10);
}

TEST(Task_Tests, Poll_VoidAndAsync_Executed)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  std::thread::id launch_id = std::this_thread::get_id();
  std::thread::id execute_id;

  auto task = [&]() -> Task<>
  {
    co_await AsyncExecuteAwaiter { &thread };

    execute_id = std::this_thread::get_id();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;
  }();

  task.Eject();

  task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_NE(launch_id, execute_id);

  if (thread.joinable()) thread.join();
}

TEST(Task_Tests, Poll_ResultAndAsync_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  std::thread::id launch_id;
  std::thread::id execute_id;

  auto task = [&]() -> Task<int>
  {
    launch_id = std::this_thread::get_id();

    co_await AsyncExecuteAwaiter { &thread };

    execute_id = std::this_thread::get_id();

    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

    count++;

    co_return 99;
  }();

  task.Eject();

  auto value = task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, 99);
  EXPECT_EQ(launch_id, std::this_thread::get_id());
  EXPECT_NE(launch_id, execute_id);

  if (thread.joinable()) thread.join();
}

TEST(Task_Tests, WhenReady_Async_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  std::thread::id launch_id_1;
  std::thread::id launch_id_2;
  std::thread::id execute_id_1;
  std::thread::id execute_id_2;

  auto task = [&]() -> Task<>
  {
    launch_id_1 = std::this_thread::get_id();

    co_await [&]() -> Task<int>
    {
      launch_id_2 = std::this_thread::get_id();

      co_await AsyncExecuteAwaiter { &thread };

      execute_id_1 = std::this_thread::get_id();

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 10;
    }()
                        .WhenReady();

    execute_id_2 = std::this_thread::get_id();

    count++;
  }();

  task.Eject();

  task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 2);
  EXPECT_EQ(launch_id_1, std::this_thread::get_id());
  EXPECT_EQ(launch_id_1, launch_id_2);
  EXPECT_NE(launch_id_1, execute_id_1);
  EXPECT_EQ(execute_id_1, execute_id_2);

  if (thread.joinable()) thread.join();
}

TEST(Task_Tests, CoAwait_ResultAndAsync_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread;

  std::thread::id launch_id_1;
  std::thread::id launch_id_2;
  std::thread::id execute_id_1;
  std::thread::id execute_id_2;

  auto task = [&]() -> Task<int>
  {
    launch_id_1 = std::this_thread::get_id();

    auto value = co_await [&]() -> Task<int>
    {
      launch_id_2 = std::this_thread::get_id();

      co_await AsyncExecuteAwaiter { &thread };

      execute_id_1 = std::this_thread::get_id();

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 99;
    }();

    execute_id_2 = std::this_thread::get_id();

    count++;

    co_return value;
  }();

  task.Eject();

  auto value = task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 2);
  EXPECT_EQ(value, 99);
  EXPECT_EQ(launch_id_1, std::this_thread::get_id());
  EXPECT_EQ(launch_id_1, launch_id_2);
  EXPECT_NE(launch_id_1, execute_id_1);
  EXPECT_EQ(execute_id_1, execute_id_2);

  if (thread.joinable()) thread.join();
}

TEST(Task_Tests, CoAwait_DifferentThreadsResultAndAsync_CorrectValue)
{
  std::atomic_size_t count = 0;

  std::thread thread1;
  std::thread thread2;

  std::thread::id launch_id_1;
  std::thread::id launch_id_2;
  std::thread::id launch_id_3;
  std::thread::id execute_id_1;
  std::thread::id execute_id_2;
  std::thread::id execute_id_3;

  auto task = [&]() -> Task<int>
  {
    launch_id_1 = std::this_thread::get_id();

    auto value1 = co_await [&]() -> Task<int>
    {
      launch_id_2 = std::this_thread::get_id();

      co_await AsyncExecuteAwaiter { &thread1 };

      execute_id_1 = std::this_thread::get_id();

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 10;
    }();

    auto value2 = co_await [&]() -> Task<int>
    {
      launch_id_3 = std::this_thread::get_id();

      co_await AsyncExecuteAwaiter { &thread2 };

      execute_id_3 = std::this_thread::get_id();

      std::this_thread::sleep_for(std::chrono::milliseconds { 1 });

      count++;

      co_return 10;
    }();

    execute_id_2 = std::this_thread::get_id();

    count++;

    co_return value1 + value2;
  }();

  task.Eject();

  auto value = task.Poll();

  EXPECT_TRUE(task.IsReady());
  EXPECT_EQ(count, 3);
  EXPECT_EQ(value, 20);
  EXPECT_EQ(launch_id_1, std::this_thread::get_id());
  EXPECT_EQ(launch_id_1, launch_id_2);
  EXPECT_NE(launch_id_2, execute_id_1);
  EXPECT_EQ(launch_id_3, execute_id_1);
  EXPECT_NE(launch_id_3, execute_id_3);
  EXPECT_EQ(execute_id_2, execute_id_3);

  if (thread1.joinable()) thread1.join();
  if (thread2.joinable()) thread2.join();
}

} // namespace plex::tests