#ifndef PLEX_OS_THREAD_H
#define PLEX_OS_THREAD_H

#include <immintrin.h>
#include <thread>

namespace plex
{
///
/// Enum of the different scheduler policies supported.
///
/// Every policy is OS implementation specific.
///
enum class ThreadSchedulerPolicy
{
  /// Uses the standard time-sharing policy
  /// Priority of 0 must be used.
  ///
  /// Linux: Same as SCHED_OTHER/SCHED_NORMAL
  /// Windows: Uses priority THREAD_PRIORITY_NORMAL
  ///
  Normal,

  /// For running very low priority background jobs.
  /// Priority of 0 must be used.
  ///
  /// Linux: Same as SCHED_IDLE
  /// Windows: Uses priority THREAD_PRIORITY_IDLE
  ///
  Idle,

  /// For time critical threads.
  /// Priorities 1 to 31 must be used.
  ///
  /// Linux: same as SCHED_RR
  /// Windows: Uses priorities 1 to 15 (THREAD_PRIORITY_TIME_CRITICAL)
  ///
  Realtime
};

constexpr size_t MinRealtimePriority = 1; // Min priority for realtime scheduling policy
constexpr size_t MaxRealtimePriority = 31; // Max priority for realtime scheduling policy.

///
/// Sets the thread priority and scheduler policy.
///
/// @see ThreadSchedulerPolicy to see eligible priorities for the desired policy.
///
/// @param[in] handle The native thread handle to set priority for.
/// @param[in] policy The scheduler policy to use for the thread.
/// @param[in] priority The thread priority.
///
/// @return True if the priority was successfully set.
///
bool SetThreadPriority(std::thread::native_handle_type handle, ThreadSchedulerPolicy policy, int priority);

///
/// Sets the thread affinity. Allows operating system to only use cores that are in the mask.
///
/// Every bit in the mask represents a processor.
///
/// @example 0b1 is processor 1 only.
///
/// @param[in] handle The native thread handle to set affinity for.
/// @param[in] mask The processor mask.
///
/// @return True if the processor was set, false otherwise.
///
bool SetThreadAffinity(std::thread::native_handle_type handle, uint64_t mask);

namespace this_thread
{
  ///
  /// Obtains the native thread handle for platform specific threads.
  ///
  /// @note If the platform is not supported, nullptr is returned.
  ///
  /// @return Native thread handle.
  ///
  std::thread::native_handle_type NativeHandle();

  ///
  /// Sets the name of the thread for debugging purposes.
  ///
  /// @param[in] name Name to set.
  ///
  void SetName(const char* name);

  ///
  /// Provides a hint to the processor that we are in a spin-wait loop.
  ///
  /// Can help improve performance and power consumption of spin-wait loops.
  ///
  constexpr void Pause() noexcept
  {
    if (!std::is_constant_evaluated()) _mm_pause();
  }
} // namespace this_thread

} // namespace plex

#endif
