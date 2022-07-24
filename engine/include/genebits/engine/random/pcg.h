#ifndef GENEBITS_ENGINE_RANDOM_PCG_H
#define GENEBITS_ENGINE_RANDOM_PCG_H

#include <atomic>
#include <cstdint>
#include <ctime>
#include <limits>
#include <source_location>

namespace plex
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
class PCG
{
public:
  static constexpr uint64_t cMultiplier = 6364136223846793005ull;
  static constexpr uint64_t cIncrement = 1442695040888963407ull;

  ///
  /// Default constructor
  ///
  constexpr PCG() noexcept : PCG(0) {}

  ///
  /// Constructor with user specified seed.
  ///
  constexpr explicit PCG(const uint64_t seed) noexcept : state_(seed ^ cMultiplier) // Initial scramble
  {
    // Run two LCG iterations to correctly initialize
    Next();
    Next();
  }

  ///
  /// Advances the internal state and returns the generated value.
  ///
  /// @return uint32_t Next uniform 32-bit random number.
  ///
  constexpr uint32_t operator()() noexcept
  {
    return Next();
  }

  ///
  /// Advances the internal state and returns the generated value in range [0, bound)
  ///
  /// @return uint32_t Next bounded uniform 32-bit random number.
  ///
  constexpr uint32_t operator()(const uint32_t bound) noexcept
  {
    return NextBounded(bound);
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
      if (r >= threshold) return r % bound;
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
} // namespace plex

#endif