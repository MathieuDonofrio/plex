#include "genebits/engine/parallel/job.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(BasicJob_Tests, Wait_SingleExecute_Compleated)
{
  ThreadPool pool;

  std::atomic<int> counter = 0;

  BasicLambdaJob job { [&counter]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      counter++;
    } };

  pool.EnqueueAll(job.GetTasks());

  job.Wait();

  EXPECT_EQ(counter.load(), 1);
}

TEST(BasicJob_Tests, Wait_IndirectExecute_Compleated)
{
  ThreadPool pool;

  std::atomic<int> counter = 0;

  void* dummy = nullptr; // Makes the lambda bigger in order to trigger second constructor.

  BasicLambdaJob job { [&counter, dummy]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      counter++;
    } };

  pool.EnqueueAll(job.GetTasks());

  job.Wait();

  EXPECT_EQ(counter.load(), 1);
}

TEST(ParallelForJob_Tests, Wait_Execute_Compleated)
{
  ThreadPool pool;

  static constexpr size_t amount = 100;

  std::array<std::atomic<int>, amount> access {};
  std::atomic<int> counter = 0;

  ParallelForLambdaJob job { [&counter, &access](size_t index)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));

      access[index]++;
      counter++;
    },
    amount };

  pool.EnqueueAll(job.GetTasks());

  job.Wait();

  EXPECT_EQ(counter.load(), amount);

  for (size_t i = 0; i < amount; i++)
  {
    EXPECT_EQ(access[i].load(), 1);
  }
}

TEST(JobScheduler_Tests, Schedule_SingleBasicJob_Executed)
{
  ThreadPool pool;

  JobScheduler scheduler { pool };

  std::atomic<bool> flag = false;

  BasicLambdaJob job { [&flag]()
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

  BasicLambdaJob job1 { [&test_value]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      test_value = 99;
    } };

  BasicLambdaJob job2 { [&test_value]() { test_value = 10; } };

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

  BasicLambdaJob job1 { [&test_value1]()
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      test_value1 = 99;
    } };

  BasicLambdaJob job2 { [&test_value2]() { test_value2 = 10; } };

  JobHandle handle = scheduler.CombineJobHandles(scheduler.Schedule(&job2), scheduler.Schedule(&job1));

  handle.Complete();

  EXPECT_EQ(test_value1.load(), 99);
  EXPECT_EQ(test_value2.load(), 10);
}

} // namespace genebits::engine::tests