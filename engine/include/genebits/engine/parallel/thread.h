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

  void SetName(const char* name);

} // namespace this_thread

using NativeThreadHandle = void*;

NativeThreadHandle GetCurrentNativeThread();

void SetThreadProcessor(NativeThreadHandle handle, size_t cpu);

size_t GetAmountPhysicalProcessors();

size_t GetAmountLogicalProcessors();

} // namespace genebits::engine

#endif
