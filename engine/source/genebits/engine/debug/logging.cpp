#include "genebits/engine/debug/logging.h"

#ifndef NDEBUG

#include <iostream>
#include <mutex>

#include "genebits/engine/os/color_print.h"

namespace genebits::engine::debug
{
std::mutex logger_mutex;

void PrintPrefix(const LogLevel level, bool color)
{
  using namespace terminal;

  std::cout << '[';

  switch (level)
  {
  case LogLevel::Trace:
  {
    if (color) PrintTerminalColor(Color::Cyan);
    std::cout << "TRACE";
    break;
  }
  case LogLevel::Info:
  {
    if (color) PrintTerminalColor(Color::Blue);
    std::cout << "INFO ";
    break;
  }
  case LogLevel::Warn:
  {
    if (color) PrintTerminalColor(Color::Yellow);
    std::cout << "WARN ";
    break;
  }
  case LogLevel::Error:
  {
    if (color) PrintTerminalColor(Color::Red);
    std::cout << "ERROR";
    break;
  }
  default: std::cout << "?????"; break;
  }

  if (color) PrintTerminalColor(Color::LightGray);

  std::cout << "] ";
}

void PrintStackTrace(const StackTrace stack_trace, bool color)
{
  using namespace terminal;

  if (color) PrintTerminalColor(Color::DarkRed);

  std::cout << "Backtrace:\n";

  for (const auto& frame : stack_trace.frames)
  {
    std::cout << "\tat " << frame.name << '(' << frame.file_name << ':' << frame.line << ")\n";
  }

  if (color) PrintTerminalColor(Color::LightGray);
}

void Log(LogMetadata metadata, std::string_view message)
{
  using namespace terminal;

  bool color = IsColorTerminal();

  std::lock_guard<std::mutex> lock(logger_mutex);

  PrintPrefix(metadata.level, color);

  std::cout << message << '\n';

  const auto& stack_trace = metadata.stack_trace;

  if (!stack_trace.frames.empty())
  {
    PrintStackTrace(stack_trace, color);
  }

  std::cout << std::flush;
}
} // namespace genebits::engine::debug

#endif
