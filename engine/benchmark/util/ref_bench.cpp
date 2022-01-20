#include "genebits/engine/util/ref.h"

#include <memory>

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
namespace
{
  class Object
  {
    double a; // Dummy fields
    double b;
    double c;
    double d;
    double e;
    double f;
  };

  class RefCountedObject : public RefCounted, public Object
  {};

  class AtomicRefCountedObject : public AtomicRefCounted, public Object
  {};
} // namespace

static void Ref_STD_SharedPtr_MakeShared_Ref1(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main = std::make_shared<Object>();

    benchmark::DoNotOptimize(main);
  }
}

BENCHMARK(Ref_STD_SharedPtr_MakeShared_Ref1);

static void Ref_STD_SharedPtr_Ref1(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main { new Object };

    benchmark::DoNotOptimize(main);
  }
}

BENCHMARK(Ref_STD_SharedPtr_Ref1);

static void Ref_STD_SharedPtr_Ref2(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main { new Object };
    std::shared_ptr<Object> cpy1 { main };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
  }
}

BENCHMARK(Ref_STD_SharedPtr_Ref2);

static void Ref_STD_SharedPtr_Ref3(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main { new Object };
    std::shared_ptr<Object> cpy1 { main };
    std::shared_ptr<Object> cpy2 { cpy1 };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
    benchmark::DoNotOptimize(cpy2);
  }
}

BENCHMARK(Ref_STD_SharedPtr_Ref3);

static void Ref_Ref1(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main { new Object };

    benchmark::DoNotOptimize(main);
  }
}

BENCHMARK(Ref_Ref1);

static void Ref_Ref2(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<Object> main { new Object };
    std::shared_ptr<Object> cpy1 { main };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
  }
}

BENCHMARK(Ref_Ref2);

static void Ref_Ref3(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<Object> main { new Object };
    Ref<Object> cpy1 { main };
    Ref<Object> cpy2 { cpy1 };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
    benchmark::DoNotOptimize(cpy2);
  }
}

BENCHMARK(Ref_Ref3);

static void Ref_Intrusive_Ref1(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<RefCountedObject> main { new RefCountedObject };

    benchmark::DoNotOptimize(main);
  }
}

BENCHMARK(Ref_Intrusive_Ref1);

static void Ref_Intrusive_Ref2(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<RefCountedObject> main { new RefCountedObject };
    Ref<RefCountedObject> cpy1 { main };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
  }
}

BENCHMARK(Ref_Intrusive_Ref2);

static void Ref_Intrusive_Ref3(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<RefCountedObject> main { new RefCountedObject };
    Ref<RefCountedObject> cpy1 { main };
    Ref<RefCountedObject> cpy2 { cpy1 };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
    benchmark::DoNotOptimize(cpy2);
  }
}

BENCHMARK(Ref_Intrusive_Ref3);

static void Ref_AtomicIntrusive_Ref1(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<AtomicRefCountedObject> main { new AtomicRefCountedObject };

    benchmark::DoNotOptimize(main);
  }
}

BENCHMARK(Ref_AtomicIntrusive_Ref1);

static void Ref_AtomicIntrusive_Ref2(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<AtomicRefCountedObject> main { new AtomicRefCountedObject };
    Ref<AtomicRefCountedObject> cpy1 { main };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
  }
}

BENCHMARK(Ref_AtomicIntrusive_Ref2);

static void Ref_AtomicIntrusive_Ref3(benchmark::State& state)
{
  for (auto _ : state)
  {
    Ref<AtomicRefCountedObject> main { new AtomicRefCountedObject };
    Ref<AtomicRefCountedObject> cpy1 { main };
    Ref<AtomicRefCountedObject> cpy2 { cpy1 };

    benchmark::DoNotOptimize(main);
    benchmark::DoNotOptimize(cpy1);
    benchmark::DoNotOptimize(cpy2);
  }
}

BENCHMARK(Ref_AtomicIntrusive_Ref3);

} // namespace genebits::engine::bench