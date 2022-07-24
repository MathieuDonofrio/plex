#ifndef PLEX_BENCHMARK_COMMON_FAKE_WORK_H
#define PLEX_BENCHMARK_COMMON_FAKE_WORK_H

#include <benchmark/benchmark.h>

namespace plex::bench
{
inline void Work(size_t amount)
{
  static const volatile auto seed = static_cast<uint64_t>(time(nullptr));

  uint64_t state = seed;

  for (size_t i = 0; i < amount; i++)
  {
    auto old = state;

    state = old * 1664525 + 1013904223;

    state ^= old;
  }

  benchmark::DoNotOptimize(state);
  benchmark::ClobberMemory();
}
} // namespace plex::bench

#endif
