#ifndef GENEBITS_ENGINE_UTIL_CONCURRENCY_H
#define GENEBITS_ENGINE_UTIL_CONCURRENCY_H

#include <atomic>
#include <new>

#include "config.h"

namespace genebits::engine
{
///
/// Enum of the different concurrency models that can be used to specify a specialized
/// container for maximum performance.
///
/// @note The handling of concurrency is implementation defined (Could just always lock).
///
enum class ConcurrencyModel
{
  // Not thread-safe
  None,

  // Thread-safe for a single producer thread and a single consumer thread
  ProducerConsumer,

  // Thread-safe for multiple producer threads and a single consumer thread
  SingleConsumer,

  // Always thread-safe
  Full
};

#ifdef __cpp_lib_hardware_interference_size
#define STANDARD_HARDWARE_INTERFERENCY_SIZE true
#else
#define STANDARD_HARDWARE_INTERFERENCY_SIZE false
#endif

///
/// Defines size of a cacheline. Can be used to fit variables on different
/// cachelines to avoid cache synchronization after thread writes.
///
/// @note 64 bytes on x86-64
///
constexpr size_t cCacheLineSize = STANDARD_HARDWARE_INTERFERENCY_SIZE
                                    ? std::hardware_destructive_interference_size
                                    : 2 * sizeof(std::max_align_t);

#undef STANDARD_HARDWARE_INTERFERENCY_SIZE

///
/// Very good spin lock implementation.
///
/// @note Based on implementation from: https://rigtorp.se/spinlock/
///
class SpinLock
{
public:
  ///
  /// Default constructor
  ///
  constexpr SpinLock()
    : lock_ { false }
  {
  }

  ///
  /// Locks the spin lock, blocks if the lock is not available.
  ///
  constexpr void lock() noexcept
  {
    for (;;)
    {
      // Optimistically assume the lock is free on the first try
      if (!lock_.exchange(true, std::memory_order_acquire)) return;

      // Wait for lock to be released without generating cache misses
      while (lock_.load(std::memory_order_relaxed))
      {};
    }
  }

  ///
  /// Tries to lock the spin lock. Returns immediately.
  ///
  /// @return True if aquired lock, false otherwise
  ///
  constexpr bool try_lock() noexcept
  {
    return !lock_.exchange(true, std::memory_order::acquire);
  }

  ///
  /// Unlocks the spin lock.
  ///
  constexpr void unlock() noexcept
  {
    lock_.store(false, std::memory_order::release);
  }

private:
  std::atomic<bool> lock_;
};

} // namespace genebits::engine

#endif
