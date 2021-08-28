#include "genebits/engine/debug/terminal_logger.h"

#include "genebits/engine/debug/print.h"

namespace genebits::engine
{
void PrintPrefix(const LogLevel level)
{
  Print('[');

  switch (level)
  {
  case LogLevel::Trace:
  {
    PrintColor(TColor::Cyan);
    Print(std::string_view { "TRACE" });
    break;
  }
  case LogLevel::Info:
  {
    PrintColor(TColor::Blue);
    Print(std::string_view { "INFO " });
    break;
  }
  case LogLevel::Warn:
  {
    PrintColor(TColor::Yellow);
    Print(std::string_view { "WARN " });
    break;
  }
  case LogLevel::Error:
  {
    PrintColor(TColor::Red);
    Print(std::string_view { "ERROR" });
    break;
  }
  default: Print(std::string_view { "?????" }); break;
  }

  PrintColorReset();

  Print(std::string_view { "] " });
}

void PrintStackTrace(const StackTrace stack_trace)
{
  PrintColor(TColor::DarkRed);

  Print(std::string_view { "Backtrace:\n" });

  for (const auto& frame : stack_trace.frames)
  {
    Print(std::string_view { "\tat " });
    Print(frame.name);
    Print('(');
    Print(frame.file_name);
    Print(':');
    Print(std::to_string(frame.line));
    Print(std::string_view { ")\n" });
  }

  PrintColorReset();
}

void TerminalLogger::Listen(const LogEvent& event)
{
  std::scoped_lock<std::mutex> lock(mutex_);

  PrintPrefix(event.log.metadata.level);

  Print(event.log.message);

  PrintLine();

  const auto& stack_trace = event.log.metadata.stack_trace;

  if (!stack_trace.frames.empty())
  {
    // Prints red stacktrace
    PrintStackTrace(stack_trace);
  }

  PrintFlush();
}
} // namespace genebits::engine
