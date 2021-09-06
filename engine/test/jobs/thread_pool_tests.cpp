#include "genebits/engine/jobs/thread_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ThreadPool_Tests, EnqueueTask_NoTasks_Invoked)
{
  ThreadPool pool;

  Task task;

  int count = 0;

  task.Bind([&count]() { count++; });

  pool.Execute(&task);

  task.Wait();

  ASSERT_EQ(count, 1);
}

TEST(ThreadPool_Tests, EnqueueTask_MultipleTasks_Invoked)
{
  ThreadPool pool;

  constexpr size_t amount = 100;

  Task tasks[amount];

  int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Bind([&count]() { count++; });

    pool.Execute(&tasks[i]);
  }

  for (size_t i = 0; i < amount; i++)
  {
    tasks[i].Wait();
  }

  ASSERT_EQ(count, amount);
}
} // namespace genebits::engine::tests