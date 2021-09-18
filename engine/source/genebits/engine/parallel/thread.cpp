#include "genebits/engine/parallel/thread.h"

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
#endif

namespace genebits::engine
{
namespace this_thread
{
  void SetName([[maybe_unused]] const char* name)
  {
#if PLATFORM_WINDOWS
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

NativeThreadHandle GetCurrentNativeThread()
{
#if PLATFORM_WINDOWS
  return GetCurrentThread();
#elif PLATFORM_LINUX
  return pthread_self();
#else
  return nullptr;
#endif
}

void SetThreadProcessor([[maybe_unused]] NativeThreadHandle handle, [[maybe_unused]] size_t cpu)
{
#if PLATFORM_WINDOWS
  uint64_t affinity = static_cast<uint64_t>(1) << cpu;

  auto result = SetThreadAffinityMask(handle, affinity);

  ASSERT(result != 0, "Error setting windows thread affinity");
#elif PLATFORM_LINUX
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu, &cpuset)

  auto result = pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpuset);

  ASSERT(result == 0, "Error setting pthread affinity");
#endif
}
size_t GetAmountPhysicalProcessors()
{
#if PLATFORM_WINDOWS

  DWORD length = 0;

  [[maybe_unused]] auto result1 = GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);

  ASSERT(!result1 && GetLastError() == ERROR_INSUFFICIENT_BUFFER, "Error while obtaining logical processor info");

  std::unique_ptr<uint8_t[]> buffer(new uint8_t[length]);
  const auto info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.get());

  [[maybe_unused]] auto result2 = GetLogicalProcessorInformationEx(RelationProcessorCore, info, &length);

  ASSERT(result2 != FALSE, "Failed to get logical processor information");

  size_t physical_processors = 0;

  size_t offset = 0;

  do
  {
    offset += reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.get() + offset)->Size;
    ++physical_processors;
  }
  while (offset < length);

  return physical_processors;
#else
  return GetAmountLogicalProcessors(); // Fallback to logical processors
#endif
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
