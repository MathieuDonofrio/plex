#ifndef GENEBITS_ENGINE_THREADING_THREAD_SUPPORT_H
#define GENEBITS_ENGINE_THREADING_THREAD_SUPPORT_H

#include <atomic>
#include <immintrin.h>
#include <thread>

namespace genebits::engine
{
namespace this_thread
{
  ///
  /// Yields to other threads.
  ///
  /// Faster than std::this_thread::yield().
  ///
  inline void Pause() noexcept
  {
    _mm_pause();
  }

  ///
  /// Spins for the specified number of loops while yielding to other threads.
  ///
  /// @param[in] loops amount of loops to perform.
  ///
  inline void Pause(size_t loops) noexcept
  {
    while (loops-- != 0)
    {
      Pause();
    }
  }
} // namespace this_thread

///
/// Exponentially waits for an amount of yielding spins.
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
  /// Spins an amount of times while yielding. Amount of loops increase
  /// every time this method is called.
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
  /// in pause loops. If this is reached, the exponential backoff will yield.
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
