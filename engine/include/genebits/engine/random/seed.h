#ifndef GENEBITS_ENGINE_RANDOM_SEED_H
#define GENEBITS_ENGINE_RANDOM_SEED_H

#include <atomic>
#include <cstdint>
#include <ctime>
#include <random>
#include <source_location>
#include <thread>

namespace genebits::engine
{
///
/// Generates a seed at compile time.
///
/// Combination of build time seed and source location seed.
///
/// @param[in] location The source location (Default is correct)
///
/// @return Compile time seed
///
consteval uint64_t CompileTimeSeed(const std::source_location location = std::source_location::current())
{
  // Time seed changes every new build
  // Inspiration from Jason Turner video: https://www.youtube.com/watch?v=rpn_5Mrrxf8
  uint64_t time_seed = 0;

  for (const char c : __TIME__)
  {
    time_seed <<= 8u;
    time_seed |= static_cast<uint8_t>(c);
  }

  // Location seed changes when called in a different location
  uint64_t location_seed = location.column() + (location.line() << 6) + (location.line() >> 2);

  const char* file_name = location.file_name();

  size_t rot = 0;

  while (*file_name != '\0')
  {
    location_seed ^= static_cast<uint8_t>(*file_name) << ((rot++ << 3) & 31u);
    ++file_name;
  }

  return time_seed ^ location_seed;
}

///
/// Generates a seed derived from system time.
///
/// Pretty expensive.
///
/// @return Generated time seed.
///
inline uint64_t TimeSeed()
{
  // Combining time with thread id should get us a pretty unique seed.
  auto thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());

  return static_cast<uint64_t>(std::time(nullptr) ^ thread_hash);
}

///
/// Generates a non-deterministic seed.
///
/// Non-deterministic behaviour is implementation defined and may use a prng.
///
/// Very expensive. For example, on windows this may call some crypto library.
///
/// @return Generated device seed.
///
inline uint64_t DeviceSeed()
{
  std::random_device random_source;

  return random_source();
}

///
/// Generates a pretty good unique seed.
///
/// Uses a simple static atomic LCG initialized once with a strong non-deterministic seed.
///
/// Very fast seed.
///
/// @return uint64_t Generated seed.
///
inline uint64_t Seed()
{
  // Initialize with non-deterministic seed
  static std::atomic<uint64_t> state = DeviceSeed() ^ TimeSeed();

  while (true)
  {
    uint64_t current = state.load(std::memory_order_relaxed);

    // L'Ecuyer, "Tables of Linear Congruential Generators of
    // Different Sizes and Good Lattice Structure", 1999
    const uint64_t next = current * 1181783497276652981L;

    if (state.compare_exchange_weak(current, next, std::memory_order_relaxed)) return next;
  }
}

} // namespace genebits::engine

#endif