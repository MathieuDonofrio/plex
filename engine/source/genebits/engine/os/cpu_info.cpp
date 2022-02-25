#include "genebits/engine/os/cpu_info.h"

#include "genebits/engine/config/compiler.h"
#include "genebits/engine/debug/assertion.h"

#include <memory>

#if PLATFORM_WINDOWS
// Lean windows include
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif PLATFORM_LINUX
#include <pthread.h>
#include <unistd.h>
#endif

namespace genebits::engine
{
CPUInfo GetCPUInfo()
{
  CPUInfo cpu_info;

#if PLATFORM_WINDOWS
  DWORD length = 0;

  [[maybe_unused]] auto result1 = GetLogicalProcessorInformationEx(RelationAll, nullptr, &length);

  ASSERT(!result1 && GetLastError() == ERROR_INSUFFICIENT_BUFFER, "Error while obtaining processor info");

  std::unique_ptr<uint8_t[]> buffer(new uint8_t[length]);
  const auto info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.get());

  [[maybe_unused]] auto result2 = GetLogicalProcessorInformationEx(RelationAll, info, &length);

  ASSERT(result2 != FALSE, "Failed to get processor information");

  size_t offset = 0;

  do
  {
    auto current = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.get() + offset);

    switch (current->Relationship)
    {
    case RelationCache:
    {
      CacheInfo cache;

      switch (current->Cache.Type)
      {
      case CacheUnified: cache.type = CacheType::Unified; break;
      case CacheInstruction: cache.type = CacheType::Instruction; break;
      case CacheData: cache.type = CacheType::Data; break;
      case CacheTrace: cache.type = CacheType::Trace; break;
      }

      cache.level = current->Cache.Level;
      cache.size = current->Cache.CacheSize;
      cache.line_size = current->Cache.LineSize;

      cpu_info.caches.push_back(cache);

      break;
    }
    case RelationProcessorCore:
    {
      ProcessorInfo processor;

      processor.mask = current->Processor.GroupMask->Mask;

      cpu_info.processors.push_back(processor);

      break;
    }
    }
    offset += current->Size;
  }
  while (offset < length);
#endif

  return cpu_info;
}

size_t GetAmountPhysicalProcessors()
{
  auto cpu_info = GetCPUInfo();

  if (!cpu_info.processors.empty()) return cpu_info.processors.size();

  return GetAmountLogicalProcessors(); // Fallback to logical processors
}

size_t GetAmountLogicalProcessors()
{
#if PLATFORM_WINDOWS
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return static_cast<size_t>(info.dwNumberOfProcessors);
#elif PLATFORM_LINUX
  return static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
#else
  return std::thread::hardware_concurrency(); // Should be considered a hint according to standard
#endif
}
} // namespace genebits::engine
