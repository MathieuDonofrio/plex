#include "engine/util/ring_buffer.h"

#include <deque>

#include <benchmark/benchmark.h>

namespace genebits::engine
{

static void RingBuffer_None_EnqueueDequeue(benchmark::State& state)
{
  for (auto _ : state)
  {
    RingBuffer<size_t, 128, ConcurrencyModel::None> ring_buffer;

    for (size_t i = 0; i < 128; i++)
    {
      ring_buffer.Enqueue(i);
    }

    for (size_t i = 0; i < 128; i++)
    {
      size_t item;

      ring_buffer.Dequeue(item);

      benchmark::DoNotOptimize(item);
    }

    benchmark::DoNotOptimize(ring_buffer);
  }
}

BENCHMARK(RingBuffer_None_EnqueueDequeue);

static void RingBuffer_ProducerConsumer_EnqueueDequeue(benchmark::State& state)
{
  for (auto _ : state)
  {
    RingBuffer<size_t, 128, ConcurrencyModel::ProducerConsumer> ring_buffer;

    for (size_t i = 0; i < 128; i++)
    {
      ring_buffer.Enqueue(i);
    }

    for (size_t i = 0; i < 128; i++)
    {
      size_t item;

      ring_buffer.Dequeue(item);

      benchmark::DoNotOptimize(item);
    }

    benchmark::DoNotOptimize(ring_buffer);
  }
}

BENCHMARK(RingBuffer_ProducerConsumer_EnqueueDequeue);

} // namespace genebits::engine