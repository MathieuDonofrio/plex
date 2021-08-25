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
void handler(int signal)
{
  std::cerr << "Error: Signal=" << signal << '\n';

  StackTrace stack_trace = StackBackTrace(18); // 16 (18 - 2)

  if (stack_trace.frames.size() > 2)
  {
    for (auto it = stack_trace.frames.begin() + 2; it != stack_trace.frames.end(); ++it)
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
  signal(SIGSEGV, handler); // On segmentation fault
  signal(SIGABRT_COMPAT, handler); // On abort (assert)

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
