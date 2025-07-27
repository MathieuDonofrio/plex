#include "plex/ecs/view_relations.h"

#include <benchmark/benchmark.h>

namespace plex::bench
{
static void ViewRelations_AssureArchetype(benchmark::State& state)
{
  ViewRelations relations;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(relations.AssureArchetype<int, double, float, bool, long>());
  }

  benchmark::DoNotOptimize(relations);
}

BENCHMARK(ViewRelations_AssureArchetype);

static void ViewRelations_AssureView(benchmark::State& state)
{
  ViewRelations relations;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(relations.AssureView<int, double, float, bool, long>());
  }

  benchmark::DoNotOptimize(relations);
}

BENCHMARK(ViewRelations_AssureView);
} // namespace plex::bench