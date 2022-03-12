#include "genebits/engine/os/thread.h"

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
bool SetThreadAffinity([[maybe_unused]] std::thread::native_handle_type handle, [[maybe_unused]] uint64_t mask)
{
#if PLATFORM_WINDOWS
  return SetThreadAffinityMask(handle, mask) != 0;
#elif PLATFORM_LINUX
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  for (size_t i = 0; i < 64; i++)
  {
    if (mask & (1 << i)) CPU_SET(i, &cpuset)
  }

  return !pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpuset);
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
