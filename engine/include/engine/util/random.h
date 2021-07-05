#ifndef GENEBITS_ENGINE_UTIL_RANDOM_H_
#define GENEBITS_ENGINE_UTIL_RANDOM_H_

#include <atomic>
#include <cstdint>
#include <ctime>
#include <limits>
#include <source_location>

namespace genebits::engine
{
///
/// Pseudo-random number generator from PCG family.
///
/// The PCG family are simple fast space-efficient statistically
/// good algorithms for number generation.
///
/// This implementation is based on a minimal c implementation:
/// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
/// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
///
class Random
{
public:
  static constexpr uint64_t cMultiplier = 6364136223846793005ull;
  static constexpr uint64_t cIncrement = 1442695040888963407ull;
  static constexpr uint64_t cScramble = 0x5DEECE66D;

  ///
  /// Default constructor
  ///
  constexpr Random()
    : state_(cMultiplier ^ cScramble)
  {
  }

  ///
  /// Constructor with user specified seed.
  ///
  constexpr explicit Random(const uint64_t seed) noexcept
    : state_(seed ^ cScramble)
  {
    // Run two LCG iterations to correctly initialize
    state_ = state_ * cMultiplier + cIncrement;
    state_ = state_ * cMultiplier + cIncrement;
  }

  ///
  /// Random floating point uniform between lower_bound and upper_bound
  ///
  /// @param[in] lower_bound Lower bound.
  /// @param[in] upper_bound Upper bound.
  ///
  /// @return Random float
  ///
  [[nodiscard]] constexpr float NextFloat(const float lower_bound, const float upper_bound) noexcept
  {
    return lower_bound + (upper_bound - lower_bound) * static_cast<float>(Next()) / static_cast<float>(std::numeric_limits<uint32_t>::max());
  }

  ///
  /// Random floating point uniform between lower_bound and upper_bound
  ///
  /// @param[in] lower_bound Lower bound.
  /// @param[in] upper_bound Upper bound.
  ///
  /// @return Random float
  ///
  [[nodiscard]] constexpr float NextFloat(const float upper_bound) noexcept
  {
    return static_cast<float>(Next()) * (upper_bound / static_cast<float>(std::numeric_limits<uint32_t>::max()));
  }

  ///
  /// Random floating point uniform between 0 and 1
  ///
  /// @param[in] lower_bound Lower bound.
  /// @param[in] upper_bound Upper bound.
  ///
  /// @return Random float.
  ///
  [[nodiscard]] constexpr float NextFloat() noexcept
  {
    return static_cast<float>(Next()) / static_cast<float>(std::numeric_limits<uint32_t>::max());
  }

  ///
  /// Random unsigned int between lower_bound and upper_bound.
  ///
  /// @param[in] lower_bound Lower bound.
  /// @param[in] upper_bound Upper bound.
  ///
  /// @return Random unsigned int.
  ///
  [[nodiscard]] constexpr uint32_t NextUInt(const uint32_t lower_bound, const uint32_t upper_bound) noexcept
  {
    return NextBounded(upper_bound - lower_bound) + lower_bound;
  }

  ///
  /// Random unsigned int between 0 and upper_bound.
  ///
  /// @param[in] upper_bound Upper bound
  ///
  /// @return Random unsigned int.
  ///
  [[nodiscard]] constexpr uint32_t NextUInt(const uint32_t upper_bound) noexcept
  {
    return NextBounded(upper_bound);
  }

  ///
  /// Random unsigned int.
  ///
  /// @return Random unsigned int.
  ///
  [[nodiscard]] constexpr uint32_t NextUInt() noexcept
  {
    return Next();
  }

  ///
  /// Returns the current state.
  ///
  /// @return constexpr uint64_t Current state
  ///
  [[nodiscard]] constexpr uint64_t State() const noexcept
  {
    return state_;
  }

private:
  ///
  /// Generates a uniform 32-bit random number smaller than specified bound.
  ///
  /// The generated uniform will be bigger or equal to 0 and smaller than the bound.
  ///
  /// @note Advances internal state.
  ///
  /// @warning Cannot use a bound smaller than 1.
  ///
  /// @param[in] bound Max value exclusive.
  ///
  /// @return uint32_t Next uniform 32-bit random number, bounded.
  ///
  constexpr uint32_t NextBounded(const uint32_t bound) noexcept
  {
    // To avoid bias, we need to make the range of the RNG a multiple of
    // bound, which we do by dropping output less than a threshold.
    const uint32_t threshold = (-bound) % bound;

    // 82.25% of the time, we can expect it to require just one iteration
    while (true)
    {
      const uint32_t r = Next();
      if (r >= threshold)
        return r % bound;
    }
  }

  ///
  /// @brief Generates a uniform 32-bit random number.
  ///
  /// @note Advances the internal state.
  ///
  /// @see pcg-random.org
  ///
  /// @return uint32_t Next uniform 32-bit random number.
  ///
  constexpr uint32_t Next() noexcept
  {
    const uint64_t old_state = state_;

    // Advance internal state
    state_ = old_state * cMultiplier + (cIncrement | 1u);

    // Calculate output function (XSH RR), uses old state for max ILP
    const auto xor_shifted = static_cast<uint32_t>(((old_state >> 18u) ^ old_state) >> 27u);
    const auto rot = static_cast<uint32_t>(old_state >> 59u);

    return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31u));
  }

private:
  uint64_t state_;
};

///
/// Compile-time seed.
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
  uint64_t time_seed = 0;

  for (const auto c : __TIME__)
  {
    time_seed <<= 8u;
    time_seed |= static_cast<uint8_t>(c);
  }

  time_seed ^= Random::cScramble;

  // Location seed changes when called in a different location
  uint64_t location_seed = Random::cIncrement;

  for (const auto c : __FILE__)
  {
    location_seed ^= static_cast<uint8_t>(c);
    location_seed *= Random::cMultiplier;
  }

  location_seed ^= (location.line() + location.line() * location.column());

  location_seed = location_seed * Random::cMultiplier + Random::cIncrement;

  return time_seed ^ location_seed;
}

///
/// Thread-safe non-deterministic unique seed.
///
/// Combination of time-based non-determinism and atomic linear congruential generator.
/// The result is a pretty good non-deterministic seed.
///
/// Because obtaining current time is slow, it only obtains time once.
///
/// @return uint64_t Seed
///
inline uint64_t Seed()
{
  // Initialized with time based seed
  static std::atomic<uint64_t> state = static_cast<uint64_t>(std::time(nullptr)) ^ Random::cScramble;

  while (true)
  {
    uint64_t current = state;

    // Runs an LCG iteration for randomness in the seed.
    uint64_t next = current * Random::cMultiplier + Random::cIncrement;

    if (current == state.exchange(next)) return next;
  }
}

///
/// Returns a thread-local random instance.
///
/// @return Thread local random.
///
inline Random& TLRandom()
{
  static thread_local Random random(Seed());

  return random;
}
} // namespace genebits::engine

#endif