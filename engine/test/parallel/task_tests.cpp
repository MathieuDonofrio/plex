#include "genebits/engine/parallel/thread_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(sizeof(Task) == sizeof(DataTask<double>));

TEST(Task_Tests, Finished_OnConstruction_False)
{
  Task task;

  EXPECT_FALSE(task.Finished());

  task.Finish();
}

TEST(Task_Tests, Finish_AfterConstruction_Finished)
{
  Task task;

  task.Finish();

  EXPECT_TRUE(task.Finished());
}

TEST(Task_Tests, Executor_AssignAndInvoke_CalledOnce)
{
  Task task;

  size_t amount = 0;

  task.Executor().Bind([&amount]() { amount++; });

  task.Executor().Invoke();

  EXPECT_EQ(amount, 1);

  task.Finish();
}

TEST(Task_Tests, TryPoll_WhenReturnsTrue_Finished)
{
  Task task;

  std::atomic<size_t> amount = 0;

  task.Executor().Bind([&amount]() { amount++; });

  std::thread thread(
    [&]()
    {
      task.Executor().Invoke();
      task.Finish();
    });

  if (task.TryPoll<1 << 16>())
  {
    EXPECT_EQ(amount, 1);
    EXPECT_TRUE(task.Finished());
  }

  thread.join();
}

TEST(Task_Tests, Poll_FewMilliseconds_BlockedCorrectly)
{
  Task task;

  std::atomic<size_t> amount = 0;

  task.Executor().Bind([&amount]() { amount++; });

  std::thread thread(
    [&]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      task.Executor().Invoke();
      task.Finish();
    });

  task.Poll();

  EXPECT_EQ(amount, 1);
  EXPECT_TRUE(task.Finished());

  thread.join();
}

TEST(Task_Tests, Wait_FewMilliseconds_BlockedCorrectly)
{
  Task task;

  std::atomic<size_t> amount = 0;

  task.Executor().Bind([&amount]() { amount++; });

  std::thread thread(
    [&]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      task.Executor().Invoke();
      task.Finish();
    });

  task.Wait();

  EXPECT_EQ(amount, 1);
  EXPECT_TRUE(task.Finished());

  thread.join();
}

} // namespace genebits::engine::tests