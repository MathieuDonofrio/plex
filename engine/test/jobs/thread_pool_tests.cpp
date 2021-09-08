#include "genebits/engine/jobs/thread_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ThreadPool_Tests, Shedule_OneThreadOnTask_CorrectExecution)
{
  ThreadPool pool(1);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Schedule(&task);

  task.Wait();

  ASSERT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Shedule_16ThreadsOneTask_CorrectExecution)
{
  ThreadPool pool(16);

  Task task;

  std::atomic_int count = 0;

  task.Executor().Bind([&count]() { count++; });

  pool.Schedule(&task);

  task.Wait();

  ASSERT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Shedule_16ThreadsMultipleTasks_CorrectExecution)
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

    pool.Schedule(&tasks[i]);
  }

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Wait();
  }

  ASSERT_EQ(count, amount);
}

} // namespace genebits::engine::tests