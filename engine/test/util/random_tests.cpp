#include "genebits/engine/util/random.h"

#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
// Random static tests

static_assert(std::is_trivially_copyable_v<Random>, "Random must be trivially copyable");
static_assert(sizeof(Random) == 8, "Size of random must be 8 bytes");

TEST(Random_Tests, NextUInt_Nothing_Changes)
{
  Random random;

  uint32_t value1 = random.NextUInt();
  uint32_t value2 = random.NextUInt();

  ASSERT_NE(value1, value2);
}

TEST(Random_Tests, NextUInt_UpperBound_InRange)
{
  for (uint64_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (size_t i = 0; i < 10000; i++)
    {
      uint32_t upper_bound = static_cast<uint32_t>(1 + seed * 3);

      uint32_t value = random.NextUInt(upper_bound);

      ASSERT_GE(value, 0u);
      ASSERT_LE(value, upper_bound);
    }
  }
}

TEST(Random_Tests, NextUInt_LowerBoundAndUpperBound_InRange)
{
  for (size_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (uint32_t i = 0; i < 10000; i++)
    {
      uint32_t lower_bound = static_cast<uint32_t>(seed);
      uint32_t upper_bound = static_cast<uint32_t>(1 + seed * 3);

      uint32_t value = random.NextUInt(lower_bound, upper_bound);

      ASSERT_GE(value, lower_bound);
      ASSERT_LE(value, upper_bound);
    }
  }
}

TEST(Random_Tests, NextFloat_01_InRange)
{
  for (size_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (uint32_t i = 0; i < 10000; i++)
    {
      float value = random.NextFloat();

      ASSERT_GE(value, 0);
      ASSERT_LE(value, 1);
    }
  }
}

TEST(Random_Tests, NextFloat_UpperBound_InRange)
{
  for (size_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (uint32_t i = 0; i < 10000; i++)
    {
      float upper_bound = static_cast<float>(1 + seed * 3);

      float value = random.NextFloat(upper_bound);

      ASSERT_GE(value, 0);
      ASSERT_LE(value, upper_bound);
    }
  }
}

TEST(Random_Tests, NextFloat_LowerBoundAndUpperBound_InRange)
{
  for (size_t seed = 0; seed < 100; seed++)
  {
    Random random { seed };

    for (uint32_t i = 0; i < 10000; i++)
    {
      float lower_bound = static_cast<float>(seed);
      float upper_bound = static_cast<float>(1 + seed * 3);

      float value = random.NextFloat(lower_bound, upper_bound);

      ASSERT_GE(value, lower_bound);
      ASSERT_LE(value, upper_bound);
    }
  }
}

TEST(Random_Tests, CompileTimeSeed_DifferentLine_DifferentSeed)
{
  uint64_t seed1 = CompileTimeSeed();
  uint64_t seed2 = CompileTimeSeed();

  ASSERT_NE(seed1, seed2);
}

TEST(Random_Tests, CompileTimeSeed_DifferentColumn_DifferentSeed)
{
  uint64_t seed1 = CompileTimeSeed(), seed2 = CompileTimeSeed();

  ASSERT_NE(seed1, seed2);
}

TEST(Random_Tests, Seed_DoubleCreation_DifferentSeed)
{
  uint64_t seed1 = Seed(), seed2 = Seed();

  ASSERT_NE(seed1, seed2);
}

TEST(Random_Tests, TLRandom_DifferentThreads_DifferentRandomGenerator)
{
  Random random1 = TLRandom();

  Random random2;

  std::thread t1([&random2]() { random2 = TLRandom(); });

  t1.join();

  ASSERT_NE(random1.State(), random2.State());
}
} // namespace genebits::engine::tests