#include "genebits/engine/parallel/job.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(JobScheduler_Tests, Schedule_SingleBasicJob_Executed)
{
  ThreadPool pool;

  JobScheduler scheduler { pool };

  std::atomic<bool> flag = false;

  BasicJob job { [&flag]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      flag = true;
    } };

  JobHandle handle = scheduler.Schedule(&job);

  handle.Complete();

  EXPECT_TRUE(flag);
}

TEST(JobScheduler_Tests, Schedule_WithDependency_ExecutedInOrder)
{
  ThreadPool pool;

  JobScheduler scheduler { pool };

  std::atomic<int> test_value = 0;

  BasicJob job1 { [&test_value]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      test_value = 99;
    } };

  BasicJob job2 { [&test_value]() { test_value = 10; } };

  JobHandle handle1 = scheduler.Schedule(&job1);

  JobHandle handle2 = scheduler.Schedule(&job2, handle1); // Should complete handle 1

  handle2.Complete();

  EXPECT_EQ(test_value.load(), 10);
}

TEST(JobScheduler_Tests, CombineJobHandles_TwoJobs_BothCompleted)
{
  ThreadPool pool;

  JobScheduler scheduler { pool };

  std::atomic<int> test_value1 = 0;
  std::atomic<int> test_value2 = 0;

  BasicJob job1 { [&test_value1]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      test_value1 = 99;
    } };

  BasicJob job2 { [&test_value2]() { test_value2 = 10; } };

  JobHandle handle = scheduler.CombineJobHandles(scheduler.Schedule(&job2), scheduler.Schedule(&job1));

  handle.Complete();

  EXPECT_EQ(test_value1.load(), 99);
  EXPECT_EQ(test_value2.load(), 10);
}

} // namespace genebits::engine::tests