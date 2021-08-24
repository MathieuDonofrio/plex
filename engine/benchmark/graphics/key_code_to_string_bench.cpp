#include "genebits/engine/graphics/key_codes.h"

#include <array>

#include <benchmark/benchmark.h>

#include "genebits/engine/util/random.h"

namespace genebits::engine
{

static void Worst_KeyCodeToStringSwitch(benchmark::State& state)
{

  const uint32_t worst_case = 348;
  for (auto _ : state)
  {
    benchmark::DoNotOptimize(KeyCodeToString(static_cast<KeyCode>(worst_case)));
  }
}

BENCHMARK(Worst_KeyCodeToStringSwitch);

template<uint32_t Count>
consteval auto GenerateRandomKeyCodes()
{
  Random rng = Random(CompileTimeSeed());
  std::array<int32_t, Count> rnd;
  for (uint32_t i = 0; i < Count; ++i)
  {
    rnd[i] = static_cast<int32_t>(rng.NextUInt(0, 348 + 2)) - 2;
  }
  return rnd;
}

static void Random_KeyCodeToStringSwitch(benchmark::State& state)
{
  constexpr auto rnd = GenerateRandomKeyCodes<100>();

  static uint32_t i = 0;
  i++;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(KeyCodeToString(static_cast<KeyCode>(rnd[i])));
  }
  if (i == 99) { i = 0; }
}

BENCHMARK(Random_KeyCodeToStringSwitch);

} // namespace genebits::engine