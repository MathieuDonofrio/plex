
#include "genebits/engine/threading/exponential_backoff.h"

#include <cmath>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ExponentialBackoff_Tests, Constructor_BeforeWaiting_IsUsingPauseLoops)
{
  ExponentialBackoff backoff;

  ASSERT_TRUE(backoff.IsUsingPauseLoops());
}

TEST(ExponentialBackoff_Tests, Wait_UntilReachedMaxPauseLoops_IsUsingPauseLoops)
{
  ExponentialBackoff backoff;

  for (size_t i = 0; i <= std::log2(ExponentialBackoff::cMaxPauseLoops); i++)
  {
    ASSERT_TRUE(backoff.IsUsingPauseLoops());

    backoff.Wait();
  }
}

TEST(ExponentialBackoff_Tests, Wait_AfterReachedMaxPauseLoops_NotUsingPauseLoops)
{
  ExponentialBackoff backoff;

  for (size_t i = 0; i <= std::log2(ExponentialBackoff::cMaxPauseLoops); i++)
  {
    backoff.Wait();
  }

  ASSERT_FALSE(backoff.IsUsingPauseLoops());
}
} // namespace genebits::engine::tests