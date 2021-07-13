#include "engine/util/ring_buffer.h"

#include <deque>

#include <benchmark/benchmark.h>

namespace genebits::engine
{

static void RingBuffer_None_EnqueueDequeue(benchmark::State& state)
{
  RingBuffer<size_t, 64, ConcurrencyGuarantee::None> ring_buffer;

  for (auto _ : state)
  {
    ring_buffer.Enqueue(10);

    size_t item;

    ring_buffer.Dequeue(item);

    benchmark::DoNotOptimize(item);
  }

  benchmark::DoNotOptimize(ring_buffer);
}

BENCHMARK(RingBuffer_None_EnqueueDequeue);

static void RingBuffer_ProducerConsumer_EnqueueDequeue(benchmark::State& state)
{
  RingBuffer<size_t, 64, ConcurrencyGuarantee::OneToOne> ring_buffer;

  for (auto _ : state)
  {
    ring_buffer.Enqueue(10);

    size_t item;

    ring_buffer.Dequeue(item);

    benchmark::DoNotOptimize(item);
  }

  benchmark::DoNotOptimize(ring_buffer);
}

BENCHMARK(RingBuffer_ProducerConsumer_EnqueueDequeue);

static void RingBuffer_ProducerConsumer_EnqueueDequeueContention(benchmark::State& state)
{
  RingBuffer<size_t, 64, ConcurrencyGuarantee::OneToOne> ring_buffer;

  for (size_t i = 0; i < 16; i++)
  {
    ring_buffer.Enqueue(i);
  }

  for (auto _ : state)
  {
    ring_buffer.Enqueue(10);

    size_t item;

    ring_buffer.Dequeue(item);

    benchmark::DoNotOptimize(item);
  }

  benchmark::DoNotOptimize(ring_buffer);
}

BENCHMARK(RingBuffer_ProducerConsumer_EnqueueDequeueContention)->Threads(2);

} // namespace genebits::engine