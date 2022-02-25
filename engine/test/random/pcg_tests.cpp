#include "genebits/engine/random/pcg.h"

#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
// Random static tests

static_assert(std::is_trivially_copyable_v<PCG>, "Random must be trivially copyable");
static_assert(sizeof(PCG) == 8, "Size of random must be 8 bytes");

TEST(PCG_Tests, Next_Nothing_Changes)
{
  PCG random;

  uint32_t value1 = random();
  uint32_t value2 = random();
  uint32_t value3 = random();

  EXPECT_NE(value1, value2);
  EXPECT_NE(value2, value3);
}

TEST(PCG_Tests, NextBounded_UpperBound_InRange)
{
  for (uint64_t seed = 0; seed < 100; seed++)
  {
    PCG random { seed };

    for (size_t i = 0; i < 1000; i++)
    {
      uint32_t upper_bound = static_cast<uint32_t>(1 + seed * 3);

      uint32_t value = random(upper_bound);

      EXPECT_GE(value, 0u);
      EXPECT_LE(value, upper_bound);
    }
  }
}
} // namespace genebits::engine::tests