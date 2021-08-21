#include "genebits/engine/graphics/key_codes.h"

#include <benchmark/benchmark.h>

#include "genebits/engine/util/random.h"

namespace genebits::engine
{

static void Worst_KeyCodeToStringSwitch(benchmark::State& state)
{

  const uint32_t worst_case = 348;
  for (auto _ : state)
  {
    benchmark::DoNotOptimize(KeyCodeToStringSwitch(static_cast<KeyCode>(worst_case)));
  }
}

BENCHMARK(Worst_KeyCodeToStringSwitch);

const int32_t rnd[] { 238,
  26,
  277,
  141,
  131,
  269,
  338,
  319,
  246,
  90,
  92,
  87,
  6,
  276,
  105,
  326,
  184,
  335,
  323,
  206,
  319,
  185,
  176,
  340,
  68,
  64,
  18,
  21,
  182,
  260,
  261,
  142,
  85,
  164,
  96,
  36,
  320,
  8,
  311,
  42,
  90,
  184,
  258,
  282,
  39,
  174,
  268,
  178,
  49,
  126,
  44,
  344,
  8,
  96,
  57,
  219,
  312,
  124,
  101,
  341,
  125,
  196,
  35,
  75,
  254,
  18,
  85,
  58,
  207,
  136,
  44,
  177,
  102,
  49,
  20,
  54,
  337,
  306,
  34,
  186,
  57,
  89,
  208,
  128,
  48,
  4,
  271,
  166,
  221,
  -1,
  151,
  152,
  87,
  166,
  345,
  224,
  277,
  340,
  274,
  91 };

static void Random_KeyCodeToStringSwitch(benchmark::State& state)
{

  static uint32_t i = 0;
  i++;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(KeyCodeToStringSwitch(static_cast<KeyCode>(rnd[i])));
  }
  if (i == 99) { i = 0; }
}

BENCHMARK(Random_KeyCodeToStringSwitch);

} // namespace genebits::engine