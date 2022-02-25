#ifndef GENEBITS_ENGINE_ASYNC_THREAD_H
#define GENEBITS_ENGINE_ASYNC_THREAD_H

#include <atomic>
#include <immintrin.h>
#include <thread>
#include <vector>

namespace genebits::engine
{
///
/// Holds information about a physical processor.
///
struct ProcessorInfo
{
  uint64_t mask;
};

///
/// Enum about the cache type
///
enum class CacheType : uint32_t
{
  Unified,
  Instruction,
  Data,
  Trace
};

///
/// Holds information about a cache
///
struct CacheInfo
{
  CacheType type;
  uint32_t level;
  uint32_t size;
  uint32_t line_size;
};

// Cache line size.
// Sometimes this is not implemented, so we assume 64 bytes. (64 bytes on x86-64)
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
constexpr size_t hardware_constructive_interference_size = 64;
constexpr size_t hardware_destructive_interference_size = 64;
#endif

///
/// Holds information about a physical processor.
///
struct CPUInfo
{
  std::vector<ProcessorInfo> processors;

  std::vector<CacheInfo> caches;
};

///
/// Tries to return information about the CPU.
///
/// @warning Not required to return valid information. This should only be used
/// as a hint. If the CPU info is not supported the data will be empty.
///
/// @return CPU information.
///
CPUInfo GetCPUInfo();

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
