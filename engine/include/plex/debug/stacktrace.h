#ifndef PLEX_DEBUG_STACKTRACE_H
#define PLEX_DEBUG_STACKTRACE_H

#include <string>
#include <vector>

namespace plex::debug
{
///
/// Data of a stack frame.
///
struct StackFrame
{
  std::string name;
  std::string file_name;
  size_t line;
};

///
/// Data of a stack trace.
///
struct StackTrace
{
  std::vector<StackFrame> frames;
};

///
/// Returns the stack trace of the last stack frames since call.
///
/// @note
///     Stack trace is always empty when not running in debug.
///
/// @param[in] max_amount Maximum amount of stack frames to get.
///
/// @return Stack trace of the last frames.
///
StackTrace StackBackTrace(size_t max_amount);

} // namespace plex::debug

#endif
