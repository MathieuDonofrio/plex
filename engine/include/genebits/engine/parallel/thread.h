#ifndef GENEBITS_ENGINE_PARALLEL_THREAD_H
#define GENEBITS_ENGINE_PARALLEL_THREAD_H

#include <atomic>
#include <immintrin.h>
#include <thread>

namespace genebits::engine
{
namespace this_thread
{
  ///
  /// Provides a hint to the processor that we are in a spin-wait loop.
  ///
  /// Can help improve performance and power consumption of spin-wait loops.
  ///
  inline void Pause() noexcept
  {
    _mm_pause();
  }

  ///
  /// Pauses for a given amount of cpu time in terms of loops.
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

  ///
  /// Sets the name of the thread for debugging purposes.
  ///
  /// @param[in] name Name to set.
  ///
  void SetName(const char* name);

} // namespace this_thread

///
/// Pointer to a native thread (platform-specific).
///
using NativeThreadHandle = void*;

///
/// Obtains the native thread handle for platform specific threads.
///
/// @note If the platform is not supported, nullptr is returned.
///
/// @return Native thread handle.
///
NativeThreadHandle GetCurrentNativeThread();

///
/// Binds the thread to a logical processor index starting from 0. Returns true if the processor
/// was set.
///
/// @param[in] handle Native thread handle.
/// @param[in] index Processor index.
///
/// @return True if the processor was set, false otherwise.
///
bool SetThreadProcessor(NativeThreadHandle handle, size_t index);

///
/// Tries to return the amount of physical processors. Not always accurate and will
/// sometimes return the amount of logical processors or a hint of amount of logical
/// processors.
///
/// @return Hint to amount of physical processors.
///
size_t GetAmountPhysicalProcessors();

///
/// Tries to return the amount of logical processors. Not always accurate and will return
/// a hint of amount of logical processors.
///
/// @return Hint to amount of logical processors.
///
size_t GetAmountLogicalProcessors();

} // namespace genebits::engine

#endif
