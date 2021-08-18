#include "genebits/engine/graphics/key_codes.h"

#include <benchmark/benchmark.h>

#include "genebits/engine/util/random.h"

namespace genebits::engine
{

static void Random_KeyCodeToString(benchmark::State& state)
{
  Random random(42); // Use runtime seed.

  for (auto _ : state)
  {
    int32_t rnd = static_cast<int32_t>(random.NextUInt(349 + 2)) - 2;
    benchmark::DoNotOptimize(KeyCodeToString(static_cast<KeyCode>(rnd)));
  }
}

BENCHMARK(Random_KeyCodeToString);

static void Random_KeyCodeToStringSwitch(benchmark::State& state)
{
  Random random(42); // Use runtime seed.

  for (auto _ : state)
  {
    int32_t rnd = static_cast<int32_t>(random.NextUInt(349 + 2)) - 2;
    benchmark::DoNotOptimize(KeyCodeToStringSwitch(static_cast<KeyCode>(rnd)));
  }
}

BENCHMARK(Random_KeyCodeToStringSwitch);
} // namespace genebits::engine