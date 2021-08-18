#include "genebits/engine/logging/stacktrace.h"

#include "genebits/engine/config/compiler.h"

#ifndef NDEBUG
#if PLATFORM_WINDOWS
// Lean windows include
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Include Dbghelp after Windows.h
#include <Dbghelp.h>

// Link Dbghelp
#pragma comment(lib, "Dbghelp.lib")
#endif
#endif

namespace genebits::engine
{
#ifndef NDEBUG

constexpr size_t cMaxFrames = 128;
constexpr size_t cMaxFunctionNameLength = 256;

#if PLATFORM_WINDOWS

StackTrace StackBackTrace(size_t max_frames)
{
  max_frames = max_frames < cMaxFrames ? max_frames : cMaxFrames;

  void* stack[cMaxFrames];

  HANDLE process = GetCurrentProcess();
  SymInitialize(process, NULL, TRUE);

  size_t frames = CaptureStackBackTrace(1, static_cast<DWORD>(max_frames), stack, NULL);

  SYMBOL_INFO symbol[sizeof(SYMBOL_INFO) + cMaxFunctionNameLength * sizeof(TCHAR)];
  symbol->MaxNameLen = cMaxFunctionNameLength;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  DWORD displacement;

  IMAGEHLP_LINE64 line;
  line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

  StackTrace stacktrace;

  stacktrace.frames.reserve(frames);

  for (size_t i = 0; i < frames; i++)
  {
    const auto address = reinterpret_cast<DWORD64>(stack[i]);

    SymFromAddr(process, address, NULL, symbol);

    if (SymGetLineFromAddr64(process, address, &displacement, &line))
    {
      StackFrame frame;

      frame.name = symbol->Name;
      frame.file_name = line.FileName;
      frame.line = line.LineNumber;

      stacktrace.frames.push_back(frame);
    }
  }

  return stacktrace;
}

#else

#define NO_BACKTRACE

// TODO implement for GCC and CLANG

// GCC:
// https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-program-crashes

#endif

#else
#define NO_BACKTRACE
#endif

#ifdef NO_BACKTRACE

StackTrace StackBackTrace(size_t)
{
  StackTrace stacktrace;

  return stacktrace;
}

#endif

} // namespace genebits::engine
