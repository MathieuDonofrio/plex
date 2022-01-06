#ifndef GENEBITS_ENGINE_BENCHMARK_FAKE_WORK_H
#define GENEBITS_ENGINE_BENCHMARK_FAKE_WORK_H

namespace genebits::engine::bench
{
inline void Work(size_t amount)
{
  uint64_t seed = static_cast<uint64_t>(time(nullptr));

  benchmark::DoNotOptimize(seed);

  benchmark::ClobberMemory();

  uint64_t state = seed;

  for (size_t i = 0; i < amount; i++)
  {
    auto old = state;

    state = old * 1664525 + 1013904223;

    state ^= old;

    benchmark::DoNotOptimize(state);
  }

  benchmark::DoNotOptimize(state);

  benchmark::ClobberMemory();
}
} // namespace genebits::engine::bench

#endif
