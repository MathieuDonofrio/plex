#include "genebits/engine/parallel/thread_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ThreadPool_Tests, Constructor_CustomAmountThreads_CorrectCount)
{
  ThreadPool pool(4);

  EXPECT_EQ(pool.ThreadCount(), 4);
}

TEST(ThreadPool_Tests, Enqueue_OneThreadOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_OneThreadOneTask_Poll_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_OneThreadOneTask_TryPollWait_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  if (!task.TryPoll()) task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_OneThreadOneTask_DoubleWait_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  task.Wait();
  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_16ThreadsOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(16);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_16ThreadsOneTask_Poll_CorrectExecution)
{
  ThreadPool pool(16);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Enqueue(&task);

  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Enqueue_16ThreadsMultipleTasks_Wait_CorrectExecution)
{
  ThreadPool pool(16);

  constexpr size_t amount = 2000;

  // With 10000 the test crashes for some reason.
  // Maybe because of standard bug or OS?
  Task tasks[amount];

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Executor().Bind([&count]() { count++; });

    pool.Enqueue(&tasks[i]);
  }

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Wait();
  }

  EXPECT_EQ(count, amount);
}

TEST(ThreadPool_Tests, Enqueue_16ThreadsMultipleTasks_Poll_CorrectExecution)
{
  ThreadPool pool(16);

  constexpr size_t amount = 2000;

  // With 10000 the test crashes for some reason.
  // Maybe because of standard bug or OS?
  Task tasks[amount];

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Executor().Bind([&count]() { count++; });

    pool.Enqueue(&tasks[i]);
  }

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Poll();
  }

  EXPECT_EQ(count, amount);
}

TEST(ThreadPool_Tests, EnqueueAll_OneThreadOneTask_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.EnqueueAll(&task, &task + 1);

  task.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, EnqueueAll_OneThreadMultipleTasks_CorrectExecution)
{
  ThreadPool pool(1);

  constexpr size_t amount = 10;

  Task tasks[amount];

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Executor().Bind([&count]() { count++; });
  }

  pool.EnqueueAll(&tasks[0], &tasks[amount]);

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Wait();
  }

  EXPECT_EQ(count, amount);
}

TEST(ThreadPool_Tests, EnqueueAll_16ThreadsMultipleTasks_CorrectExecution)
{
  ThreadPool pool(16);

  constexpr size_t amount = 100;

  Task tasks[amount];

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Executor().Bind([&count]() { count++; });
  }

  pool.EnqueueAll(&tasks[0], &tasks[amount]);

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Wait();
  }

  EXPECT_EQ(count, amount);
}

} // namespace genebits::engine::tests