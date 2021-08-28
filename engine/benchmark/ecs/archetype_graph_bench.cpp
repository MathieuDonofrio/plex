#include "genebits/engine/ecs/archetype_graph.h"

#include <benchmark/benchmark.h>

namespace genebits::engine
{
static void ArchetypeGraph_AssureArchetype(benchmark::State& state)
{
  ArchetypeGraph graph;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(graph.AssureArchetype<int, double, float>());
  }

  benchmark::DoNotOptimize(graph);
}

BENCHMARK(ArchetypeGraph_AssureArchetype);

static void ArchetypeGraph_AssureView(benchmark::State& state)
{
  ArchetypeGraph graph;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(graph.AssureView<int, double, float>());
  }

  benchmark::DoNotOptimize(graph);
}

BENCHMARK(ArchetypeGraph_AssureView);
} // namespace genebits::engine