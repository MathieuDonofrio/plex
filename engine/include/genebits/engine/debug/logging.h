#ifndef GENEBITS_ENGINE_DEBUG_LOGGING_H
#define GENEBITS_ENGINE_DEBUG_LOGGING_H

#include <format>

#include "genebits/engine/debug/stacktrace.h"

namespace genebits::engine::debug
{
///
/// Enumerator of the different log levels.
///
enum class LogLevel
{
  Trace,
  Info,
  Warn,
  Error
};

///
/// Metadata about a log.
///
struct LogMetadata
{
  LogLevel level;
  StackTrace stack_trace;
  const char* file_name;
  size_t line;
};

///
/// Returns the amount of stack frames to get when logging.
///
/// Usually only errors obtain a stacktrace when debugging.
///
/// @param[in] level The log level to check stacktrace size for.
///
/// @return The amount of stack frames to get for the log level.
///
[[nodiscard]] constexpr size_t GetStackTraceDepth(const LogLevel level)
{
  if (level == LogLevel::Warn) return 4;
  if (level == LogLevel::Error) return 16;
  return 0;
}

///
/// Logs message to terminal.
///
/// @param[in] metadata Log metadata.
/// @param[in] message The message to log.
///
void Log(LogMetadata metadata, std::string_view message);

} // namespace genebits::engine::debug

#define CREATE_LOG_METADATA(level)                                                                  \
  ::genebits::engine::debug::LogMetadata                                                            \
  {                                                                                                 \
    level, ::genebits::engine::debug::StackBackTrace(GetStackTraceDepth(level)), __FILE__, __LINE__ \
  }

#ifndef NDEBUG

#ifdef __cpp_lib_format
#define LOG(level, ...) ::genebits::engine::debug::Log(CREATE_LOG_METADATA(level), ::std::format(__VA_ARGS__))
#else
// Formatting not supported, print message without formatting
#define LOG(level, message, ...) ::genebits::engine::debug::Log(CREATE_LOG_METADATA(level), message)
#endif

#define LOG_TRACE(...) LOG(::genebits::engine::debug::LogLevel::Trace, __VA_ARGS__)
#define LOG_INFO(...) LOG(::genebits::engine::debug::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) LOG(::genebits::engine::debug::LogLevel::Warn, __VA_ARGS__)
#define LOG_ERROR(...) LOG(::genebits::engine::debug::LogLevel::Error, __VA_ARGS__)

#else

#define LOG(level, ...) ((void)0)

#define LOG_TRACE(...) ((void)0)
#define LOG_INFO(...) ((void)0)
#define LOG_WARN(...) ((void)0)
#define LOG_ERROR(...) ((void)0)

#endif

#endif
