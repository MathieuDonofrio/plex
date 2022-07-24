#include "genebits/engine/debug/logging.h"

#ifndef NDEBUG

#include <mutex>

#include "genebits/engine/debug/terminal_print.h"

namespace genebits::engine::debug
{
std::mutex logger_mutex;

void PrintPrefix(const LogLevel level)
{
  TPrint('[');

  switch (level)
  {
  case LogLevel::Trace:
  {
    TPrintColor(TColor::Cyan);
    TPrint(std::string_view { "TRACE" });
    break;
  }
  case LogLevel::Info:
  {
    TPrintColor(TColor::Blue);
    TPrint(std::string_view { "INFO " });
    break;
  }
  case LogLevel::Warn:
  {
    TPrintColor(TColor::Yellow);
    TPrint(std::string_view { "WARN " });
    break;
  }
  case LogLevel::Error:
  {
    TPrintColor(TColor::Red);
    TPrint(std::string_view { "ERROR" });
    break;
  }
  default: TPrint(std::string_view { "?????" }); break;
  }

  TPrintColorReset();

  TPrint(std::string_view { "] " });
}

void PrintStackTrace(const StackTrace stack_trace)
{
  TPrintColor(TColor::DarkRed);

  TPrint(std::string_view { "Backtrace:\n" });

  for (const auto& frame : stack_trace.frames)
  {
    TPrint(std::string_view { "\tat " });
    TPrint(frame.name);
    TPrint('(');
    TPrint(frame.file_name);
    TPrint(':');
    TPrint(std::to_string(frame.line));
    TPrint(std::string_view { ")\n" });
  }

  TPrintColorReset();
}

void Log(LogMetadata metadata, std::string_view message)
{
  std::lock_guard<std::mutex> lock(logger_mutex);

  PrintPrefix(metadata.level);

  TPrint(message);

  TPrintLine();

  const auto& stack_trace = metadata.stack_trace;

  if (!stack_trace.frames.empty())
  {
    // Prints red stacktrace
    PrintStackTrace(stack_trace);
  }

  TPrintFlush();
}
} // namespace genebits::engine::debug

#endif
