#include "genebits/engine/parallel/threading.h"

#include "genebits/engine/config/compiler.h"
#include "genebits/engine/debug/assertion.h"

#if PLATFORM_WINDOWS
// Lean windows include
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif PLATFORM_LINUX
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#endif

namespace genebits::engine
{
#ifdef PLATFORM_LINUX
uint32_t GetProcCPUKeyValue(std::string key)
{
  int NumCPUs = 0;
  int MaxID = -1;
  std::ifstream f("/proc/cpuinfo");

  ASSERT(f.is_open(), "failed to open /proc/cpuinfo");


  std::string ln;
  while (std::getline(f, ln))
  {
    if (ln.empty()) continue;
    size_t SplitIdx = ln.find(':');
    std::string value;
#if defined(__s390__)
    // s390 has another format in /proc/cpuinfo
    // it needs to be parsed differently
    if (SplitIdx != std::string::npos) value = ln.substr(Key.size() + 1, SplitIdx - Key.size() - 1);
#else
    if (SplitIdx != std::string::npos) value = ln.substr(SplitIdx + 1);
#endif
    if (ln.size() >= key.size() && ln.compare(0, key.size(), key) == 0)
    {
      NumCPUs++;
      if (!value.empty())
      {
        int CurID = std::stoi(value);
        MaxID = std::max(CurID, MaxID);
      }
    }
  }

  ASSERT(!f.bad(), "Failure reading /proc/cpuinfo");

  ASSERT(f.eof(), "Failed to read to end of /proc/cpuinfo");
  f.close();

  return NumCPUs;
}
#endif

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
#elif PLATFORM_LINUX
  uint32_t num_physical_cores = GetProcCPUKeyValue("processor");


  for (uint32_t i = 0; i < num_physical_cores/2; ++i)
  {
    ProcessorInfo proc_info;
    proc_info.mask = 0b11 << i;
    cpu_info.processors.push_back(proc_info);
    cpu_info.caches.push_back(CacheInfo{});
  }

#endif

  return cpu_info;
}

bool SetThreadAffinity([[maybe_unused]] std::thread::native_handle_type handle, [[maybe_unused]] uint64_t mask)
{
#if PLATFORM_WINDOWS
  return SetThreadAffinityMask(handle, mask) != 0;
#elif PLATFORM_LINUX
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  for (size_t i = 0; i < 64; i++)
  {
    if (mask & (1 << i)) CPU_SET(i, &cpuset);
  }

  return !pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpuset);
#endif
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
  return GetProcCPUKeyValue("processor");
#else
  return std::thread::hardware_concurrency(); // Should be considered a hint according to standard
#endif
}

namespace this_thread
{
  std::thread::native_handle_type NativeHandle()
  {
#if PLATFORM_WINDOWS
    return GetCurrentThread();
#elif PLATFORM_LINUX
    return pthread_self();
#else
    return nullptr;
#endif
  }

  void SetName([[maybe_unused]] const char* name)
  {
#if PLATFORM_WINDOWS
    // Why windows??

    typedef struct tagTHREADNAME_INFO
    {
      unsigned long dwType; // must be 0x1000
      const char* szName; // pointer to name (in user addr space)
      unsigned long dwThreadID; // thread ID (-1=caller thread)
      unsigned long dwFlags; // reserved for future use, must be zero
    } THREADNAME_INFO;

    THREADNAME_INFO threadName;
    threadName.dwType = 0x1000;
    threadName.szName = name;
    threadName.dwThreadID = GetCurrentThreadId();
    threadName.dwFlags = 0;
    __try
    {
      RaiseException(0x406D1388, 0, sizeof(threadName) / sizeof(ULONG_PTR), (ULONG_PTR*)&threadName);
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {}
#elif PLATFORM_LINUX
    pthread_setname_np(pthread_self(), name);
#endif
  }

} // namespace this_thread
} // namespace genebits::engine
