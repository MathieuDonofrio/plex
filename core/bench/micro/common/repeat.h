#ifndef PLEX_BENCHMARK_COMMON_REPEAT_H
#define PLEX_BENCHMARK_COMMON_REPEAT_H

namespace plex::bench
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
} // namespace plex::bench

#endif
