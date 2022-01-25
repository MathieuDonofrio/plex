#include "genebits/engine/parallel/task.h"
#include "genebits/engine/parallel/thread_pool.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TaskSyncData
  {
    Task task;
    Sync<SyncCounter> sync;
  };
} // namespace

TEST(ThreadPool_Tests, Constructor_CustomAmountThreads_CorrectCount)
{
  ThreadPool pool(4, false);

  EXPECT_EQ(pool.ThreadCount(), 4);
}

TEST(ThreadPool_Tests, Schedule_OneThreadOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(1, false);

  std::atomic_int count = 0;

  auto task = [&]() -> Task
  {
    co_await pool.Schedule();
    count++;
  }();

  SyncFlag flag;

  auto sync = MakeSync<SyncFlag>(task);

  sync.Start(flag);

  flag.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Schedule_OneThreadOneTask_DoubleWait_CorrectExecution)
{
  ThreadPool pool(1, false);

  std::atomic_int count = 0;

  auto task = [&]() -> Task
  {
    co_await pool.Schedule();
    count++;
  }();

  SyncFlag flag;

  auto sync = MakeSync<SyncFlag>(task);

  sync.Start(flag);

  flag.Wait();
  flag.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Schedule_16ThreadsOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(16, false);

  std::atomic_int count = 0;

  auto task = [&]() -> Task
  {
    co_await pool.Schedule();
    count++;
  }();

  SyncFlag flag;

  auto sync = MakeSync<SyncFlag>(task);

  sync.Start(flag);

  flag.Wait();

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Schedule_16ThreadsMultipleTasks_Wait_CorrectExecution)
{
  ThreadPool pool(16, false);

  constexpr size_t amount = 2000;

  std::vector<TaskSyncData> tasks;
  tasks.reserve(amount);

  SyncCounter counter(amount);

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    TaskSyncData data { [&]() -> Task
      {
        co_await pool.Schedule();
        count++;
      }(),
      MakeSync<SyncCounter>(data.task) };

    data.sync.Start(counter);

    tasks.push_back(std::move(data));
  }

  counter.Wait();

  EXPECT_EQ(count, amount);
}

} // namespace genebits::engine::tests