#ifndef GENEBITS_ENGINE_BENCHMARK_COMMON_REPEAT_H
#define GENEBITS_ENGINE_BENCHMARK_COMMON_REPEAT_H

namespace genebits::engine::bench
{
#define REPEAT2(expression) \
  expression;               \
  expression;

#define REPEAT4(expression) \
  REPEAT2(expression);      \
  REPEAT2(expression);

#define REPEAT8(expression) \
  REPEAT4(expression);      \
  REPEAT4(expression);

#define REPEAT16(expression) \
  REPEAT8(expression);       \
  REPEAT8(expression);

#define REPEAT32(expression) \
  REPEAT16(expression);      \
  REPEAT16(expression);
} // namespace genebits::engine::bench

#endif
