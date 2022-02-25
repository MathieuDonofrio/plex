#ifndef GENEBITS_ENGINE_OS_THREAD_H
#define GENEBITS_ENGINE_OS_THREAD_H

#include <immintrin.h>
#include <thread>

namespace genebits::engine
{
///
/// Sets the thread affinity. Allows operating system to only use cores that are in the mask.
///
/// Every bit in the mask represents a processor.
///
/// @example 0b1 is processor 1 only.
///
/// @param[in] handle Native thread handle.
/// @param[in] mask Processor Mask.
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
  inline void Pause() noexcept
  {
    _mm_pause();
  }
} // namespace this_thread

} // namespace genebits::engine

#endif
