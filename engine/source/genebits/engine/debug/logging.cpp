#include "genebits/engine/debug/logging.h"

#ifndef NDEBUG

#include "genebits/engine/debug/terminal_logger.h"

#include <iostream>
#include <signal.h>

namespace
{
using namespace genebits::engine;
using namespace genebits::engine::debug;

///
/// Handler called when the application crashes.
///
/// @param[in] signal The crash signal
///
void CrashSignalHandler(int signal)
{
  // We must skip same frames because they give us useless stack frames about signal handling.
  static constexpr size_t cSkipFrames = 2;

  std::cerr << "Error: Signal=" << signal << '\n';

  StackTrace stack_trace = StackBackTrace(16 + cSkipFrames);

  if (stack_trace.frames.size() > cSkipFrames)
  {
    for (auto it = stack_trace.frames.begin() + cSkipFrames; it != stack_trace.frames.end(); ++it)
    {
      std::cerr << "\tat " << it->name << '(' << it->file_name << ':' << it->line << ")\n";
    }
  }

  std::cerr << std::flush;

  exit(1);
}

TerminalLogger InitializeDebugTerminalLogging()
{
  signal(SIGSEGV, CrashSignalHandler); // On segmentation fault
  signal(SIGABRT_COMPAT, CrashSignalHandler); // On abort (assert)

  return TerminalLogger(GetLoggingEventBus());
}

[[maybe_unused]] auto terminal_logger = InitializeDebugTerminalLogging();
} // namespace
#endif

namespace genebits::engine::debug
{
EventBus& GetLoggingEventBus()
{
  static EventBus bus;

  return bus;
}
} // namespace genebits::engine::debug
