#include "genebits/engine/random/pcg.h"

#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
// Random static tests

static_assert(std::is_trivially_copyable_v<Random>, "Random must be trivially copyable");
static_assert(sizeof(Random) == 8, "Size of random must be 8 bytes");

TEST(PCG_Tests, NextUInt_Nothing_Changes)
{
  Random random;

  uint32_t value1 = random.NextUInt();
  uint32_t value2 = random.NextUInt();

  EXPECT_NE(value1, value2);
}

TEST(PCG_Tests, NextUInt_UpperBound_InRange)
{
  for (uint64_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (size_t i = 0; i < 10000; i++)
    {
      uint32_t upper_bound = static_cast<uint32_t>(1 + seed * 3);

      uint32_t value = random.NextUInt(upper_bound);

      EXPECT_GE(value, 0u);
      EXPECT_LE(value, upper_bound);
    }
  }
}

TEST(PCG_Tests, NextUInt_LowerBoundAndUpperBound_InRange)
{
  for (size_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (uint32_t i = 0; i < 10000; i++)
    {
      uint32_t lower_bound = static_cast<uint32_t>(seed);
      uint32_t upper_bound = static_cast<uint32_t>(1 + seed * 3);

      uint32_t value = random.NextUInt(lower_bound, upper_bound);

      EXPECT_GE(value, lower_bound);
      EXPECT_LE(value, upper_bound);
    }
  }
}
} // namespace genebits::engine::tests