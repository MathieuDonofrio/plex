#include "genebits/engine/parallel/job.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
using namespace ::testing;

class MockMonoJob : public MonoJob<MockMonoJob>
{
public:
  MOCK_METHOD(void, Update, ());
};

class MockForJob : public ForJob<MockForJob>
{
public:
  MockForJob(size_t amount) : ForJob<MockForJob>(amount) {}

  MOCK_METHOD(void, Update, (size_t));
};

static_assert(Job<MockMonoJob>, "MockMonoJob must meet Job requirements");
static_assert(!BatchedJob<MockMonoJob>, "MockMonoJob must not meed BatchedJob requirements");
static_assert(Job<MockForJob>, "MockForJob must meet Job requirements");
static_assert(BatchedJob<MockForJob>, "MockForJob must meet BatchedJob requirements");

TEST(MonoJob_Tests, Schedule_AfterInitialization_CallsUpdate)
{
  ThreadPool pool;

  MockMonoJob job;

  EXPECT_CALL(job, Update()).Times(1);

  job.Schedule(pool);
  job.Compleate();
}

TEST(ForJob_Tests, Schedule_OneThreadOneBatch_CallsUpdateCorrectAmount)
{
  ThreadPool pool(1);

  constexpr size_t cAmount = 100;

  MockForJob job(cAmount);

  EXPECT_CALL(job, Update(_)).Times(static_cast<int>(cAmount));

  job.Schedule(pool, 1);

  job.Compleate();
}

TEST(ForJob_Tests, Schedule_OneThreadTwoBatches_CallsUpdateCorrectAmount)
{
  ThreadPool pool(1);

  constexpr size_t cAmount = 100;

  MockForJob job(cAmount);

  EXPECT_CALL(job, Update(_)).Times(static_cast<int>(cAmount));

  job.Schedule(pool, 2);

  job.Compleate();
}

TEST(ForJob_Tests, Schedule_TwoThreadsTwoBatches_CallsUpdateCorrectAmount)
{
  ThreadPool pool(2);

  constexpr size_t cAmount = 100;

  MockForJob job(cAmount);

  EXPECT_CALL(job, Update(_)).Times(static_cast<int>(cAmount));

  job.Schedule(pool, 2);

  job.Compleate();
}

TEST(ForJob_Tests, Schedule_8Threads4Batches_CallsUpdateCorrectAmount)
{
  ThreadPool pool(8);

  constexpr size_t cAmount = 100;

  MockForJob job(cAmount);

  EXPECT_CALL(job, Update(_)).Times(static_cast<int>(cAmount));

  job.Schedule(pool, 4);

  job.Compleate();
}
} // namespace genebits::engine::tests