#ifndef GENEBITS_ENGINE_OS_CPU_INFO_H
#define GENEBITS_ENGINE_OS_CPU_INFO_H

#include <atomic>
#include <vector>

namespace genebits::engine
{
// Cache line size.
// Sometimes this is not implemented, so we assume 64 bytes. (64 bytes on x86-64)
#ifdef __cpp_lib_hardware_interference_size
#define CACHE_LINE_SIZE = std::hardware_destructive_interference_size
#else
#define CACHE_LINE_SIZE 64
#endif

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
