#include "genebits/engine/async/sync_wait.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/async/thread_pool.h"
#include "genebits/engine/async/when_all.h"

#include <gtest/gtest.h>

namespace plex::tests
{
TEST(ThreadPool_Tests, Constructor_CustomAmountThreads_CorrectCount)
{
  ThreadPool pool(4, false);

  EXPECT_EQ(pool.ThreadCount(), 4);
}

TEST(ThreadPool_Tests, Schedule_OneThreadOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(1, false);

  std::atomic_int count = 0;

  auto task = [&]() -> Task<>
  {
    co_await pool.Schedule();
    count++;
  }();

  SyncWait(task);

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Schedule_16ThreadsOneTask_Wait_CorrectExecution)
{
  ThreadPool pool(16, false);

  std::atomic_int count = 0;

  auto task = [&]() -> Task<>
  {
    co_await pool.Schedule();
    count++;
  }();

  SyncWait(task);

  EXPECT_EQ(count, 1);
}

TEST(ThreadPool_Tests, Schedule_16ThreadsMultipleTasks_Wait_CorrectExecution)
{
  ThreadPool pool(16, false);

  constexpr size_t amount = 2000;

  std::vector<Task<>> tasks;
  tasks.reserve(amount);

  std::atomic_int count = 0;

  for (size_t i = 0; i < amount; i++)
  {
    tasks.push_back(
      [&]() -> Task<>
      {
        co_await pool.Schedule();
        count++;
      }());
  }

  for (auto& task : tasks)
  {
    SyncWait(task);
  }

  EXPECT_EQ(count, amount);
}

} // namespace plex::tests