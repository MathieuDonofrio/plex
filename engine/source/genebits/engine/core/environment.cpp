#include "genebits/engine/core/environment.h"

#include "genebits/engine/debug/stacktrace.h"

#ifndef NDEBUG

#include "genebits/engine/debug/terminal_logger.h"

#include <iostream>
#include <signal.h>

namespace
{
using namespace genebits::engine;

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
} // namespace
#endif

namespace genebits::engine
{
struct Environment::Pimpl
{
#ifndef NDEBUG
  TerminalLogger* terminal_logger;
#endif
};

Environment::Environment() : pimpl_(new Pimpl())
{
#ifndef NDEBUG
  signal(SIGSEGV, CrashSignalHandler); // On segmentation fault
  signal(SIGABRT_COMPAT, CrashSignalHandler); // On abort (assert)

  pimpl_->terminal_logger = new TerminalLogger(this->GetEventBus());
#endif
}

Environment::~Environment()
{
#ifndef NDEBUG
  delete pimpl_->terminal_logger;
#endif

  delete pimpl_;
}

namespace details
{
  Environment environment {};
}
} // namespace genebits::engine
