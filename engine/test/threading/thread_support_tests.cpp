
#include "genebits/engine/threading/thread_support.h"

#include <cmath>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ThreadPool_Tests, ExponentialBackoff_OnConstruction_NotReachedMaxWait)
{
  ExponentialBackoff backoff;

  ASSERT_TRUE(backoff.IsUsingPauseLoops());
}

TEST(ThreadPool_Tests, ExponentialBackoff_WaitMaximumAmountOfTimes_ReachedMaxWait)
{
  ExponentialBackoff backoff;

  for (size_t i = 0; i <= std::log2(ExponentialBackoff::cMaxPauseLoops); i++)
  {
    ASSERT_TRUE(backoff.IsUsingPauseLoops());

    backoff.Wait();
  }

  ASSERT_FALSE(backoff.IsUsingPauseLoops());
}
} // namespace genebits::engine::tests