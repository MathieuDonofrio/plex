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

  pool.Execute(task);

  task.Wait();

  ASSERT_EQ(count, 1);
}
} // namespace genebits::engine::tests