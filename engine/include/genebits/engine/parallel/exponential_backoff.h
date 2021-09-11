#ifndef GENEBITS_ENGINE_PARALLEL_EXPONENTIAL_BACKOFF_H
#define GENEBITS_ENGINE_PARALLEL_EXPONENTIAL_BACKOFF_H

#include "genebits/engine/parallel/thread.h"

namespace genebits::engine
{
///
/// Exponentially increases the polling time went waiting.
///
/// Using this can significantly decrease CPU usage in spin-wait loops.
///
class ExponentialBackoff
{
public:
  static constexpr size_t cMaxPauseLoops = 32; // Must be power of two

  ///
  /// Default Constructor
  ///
  constexpr ExponentialBackoff() : count_(1) {}

  ///
  /// Spins an amount of times while pausing or yielding. Amount of loops increase
  /// every time this method is called or the cap is reached.
  ///
  /// @note Loop increase caps at some point.
  ///
  void Wait() noexcept
  {
    if (IsUsingPauseLoops())
    {
      this_thread::Pause(count_);
      count_ <<= 1;
    }
    else
    {
      std::this_thread::yield();
    }
  }

  ///
  /// Returns whether or not the exponential backoff has reached the maximum wait time
  /// in pause loops. If this is reached, the exponential backoff will yield instead of
  /// pause.
  ///
  /// @return True if reached max wait, false otherwise.
  ///
  [[nodiscard]] constexpr bool IsUsingPauseLoops() const noexcept
  {
    return count_ <= cMaxPauseLoops;
  }

private:
  size_t count_;
};
} // namespace genebits::engine

#endif
