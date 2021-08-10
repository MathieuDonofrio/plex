#ifndef GENEBITS_ENGINE_UTIL_CONCURRENCY_H
#define GENEBITS_ENGINE_UTIL_CONCURRENCY_H

#include <atomic>
#include <immintrin.h>
#include <new>
#include <thread>

namespace genebits::engine
{
///
/// Enum of the different concurrency models that can be used to specify a specialized
/// container for maximum performance.
///
/// @note The handling of concurrency is implementation defined (Could just always lock).
///
enum class ConcurrencyGuarantee
{
  // Not thread-safe
  None,

  // Thread-safe for a single producer thread and a single consumer thread
  OneToOne,

  // Thread-safe for multiple producer threads and a single consumer thread
  ManyToOne,

  // Thread-safe for a single producer and multiple consumer threads
  OneToMany,

  // Always thread-safe
  ManyToMany
};

#ifdef __cpp_lib_hardware_interference_size
///
/// Defines size of a cacheline. Can be used to fit variables on different
/// cachelines to avoid cache synchronization after thread writes.
///
/// @note 64 bytes on x86-64
///
constexpr size_t cCacheLineSize = std::hardware_destructive_interference_size;
#else
///
/// Defines size of a cacheline. Can be used to fit variables on different
/// cachelines to avoid cache synchronization after thread writes.
///
/// @warning
///   Standard hardware interference size not defined. Using approximated method.
///
/// @note 64 bytes on x86-64
///
constexpr size_t cCacheLineSize = 2 * sizeof(std::max_align_t);
#endif

namespace this_thread
{
  ///
  /// Spins for the specified number of loops while yielding to other threads.
  ///
  /// @param[in] loops amount of loops to perform.
  ///
  inline void pause(size_t loops)
  {
    for (; loops != 0; loops--)
    {
      _mm_pause();
    }
  }
} // namespace this_thread

///
/// Exponentially waits for an amount of yielding spins.
///
class ExponentialBackoff
{
public:
  static constexpr size_t cMaxWaitLoops = (1 << 5); // 32 (Power of two)

  ///
  /// Default Constructor
  ///
  constexpr ExponentialBackoff()
    : count_(0) {}

  ///
  /// Spins an amount of times while yielding. Amount of loops increase
  /// every time this method is called.
  ///
  /// @note Loop increase caps at some point.
  ///
  void wait()
  {
    this_thread::pause(count_ & (cMaxWaitLoops - 1));

    count_ <<= 1;
  }

private:
  size_t count_;
};

///
/// Very good spin lock implementation.
///
/// @note Implements the standard Lockable concept
///
class SpinMutex
{
public:
  ///
  /// Default constructor
  ///
  constexpr SpinMutex()
    : lock_ { false }
  {
  }

  ///
  /// Locks the spin lock, blocks if the lock is not available.
  ///
  void lock() noexcept
  {
    // Optimistically assume the lock is free on the first try
    if (try_lock()) return;

    ExponentialBackoff backoff;

    // Wait for lock to be released without generating cache misses
    while (lock_.load(std::memory_order_relaxed))
    {
      backoff.wait();
    };
  }

  ///
  /// Tries to lock the spin lock. Returns immediately.
  ///
  /// @return True if aquired lock, false otherwise
  ///
  bool try_lock() noexcept
  {
    return !lock_.exchange(true, std::memory_order_acquire);
  }

  ///
  /// Unlocks the spin lock.
  ///
  void unlock() noexcept
  {
    lock_.store(false, std::memory_order_release);
  }

private:
  std::atomic<bool> lock_;
};

} // namespace genebits::engine

#endif
