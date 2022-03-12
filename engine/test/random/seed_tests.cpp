#include "genebits/engine/random/seed.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Random_Tests, CompileTimeSeed_DifferentLine_DifferentSeed)
{
  uint64_t seed1 = CompileTimeSeed();
  uint64_t seed2 = CompileTimeSeed();

  EXPECT_NE(seed1, seed2);
}

TEST(Random_Tests, CompileTimeSeed_DifferentColumn_DifferentSeed)
{
  uint64_t seed1 = CompileTimeSeed(), seed2 = CompileTimeSeed();

  EXPECT_NE(seed1, seed2);
}

TEST(Seed_Tests, Seed_DoubleCreation_DifferentSeed)
{
  uint64_t seed1 = Seed(), seed2 = Seed();

  EXPECT_NE(seed1, seed2);
}
} // namespace genebits::engine::tests