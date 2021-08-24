#ifndef GENEBITS_ENGINE_LOGGING_LOGGER_H
#define GENEBITS_ENGINE_LOGGING_LOGGER_H

#include <format>
#include <source_location>

#include "genebits/engine/core/environment.h"
#include "genebits/engine/debug/stacktrace.h"
#include "genebits/engine/events/event_bus.h"

namespace genebits::engine
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
  char* file_name;
  size_t line;
};

///
/// Data structure for log.
///
struct Log
{
  LogMetadata metadata;
  std::string message;
};

///
/// Log event.
///
/// Published when the logger makes a log.
///
struct LogEvent
{
  Log log;
};

///
/// Returns the amount frames to get when creating the stack trace for logs.
///
/// Increases the amount of frames based on the severity of the log, error being the highest.
///
/// @param level Level of the log.
///
/// @return The amount of stack frames for the log level.
///
constexpr size_t GetLogAmountStackFrames(LogLevel level)
{
  switch (level)
  {
  case LogLevel::Trace: return 0;
  case LogLevel::Info: return 3;
  case LogLevel::Warn: return 5;
  case LogLevel::Error: return 10;
  }

  return 0;
}

///
/// Creates the log metadata for the log level.
///
/// @tparam level Log level
///
/// @param location Current source location.
/// @param stack_trace Current stack trace.
///
/// @return Metadata for the log level.
///
template<LogLevel level>
LogMetadata CreateLogMetadata(StackTrace stack_trace = StackBackTrace(GetLogAmountStackFrames(level)),
  const std::source_location location = std::source_location::current())
{
  LogMetadata log_metadata;

  log_metadata.level = level;
  log_metadata.stack_trace = std::move(stack_trace);
  // log_metadata.file_name = location.file_name();
  // log_metadata.line = location.line();

  return log_metadata;
}

template<LogLevel level>
void PublishLog(std::string&& message,
  EventBus& bus = GetEnvironment().GetEventBus(),
  LogMetadata metadata = CreateLogMetadata<level>())
{
  LogEvent event;

  event.log.metadata = std::move(metadata);
  event.log.message = std::move(message);

  bus.Publish(event);
}

} // namespace genebits::engine

#ifndef NDEBUG
#define TRACE(...) ::genebits::engine::PublishLog<::genebits::engine::LogLevel::Trace>(::std::format(__VA_ARGS__))
#define INFO(...) ::genebits::engine::PublishLog<::genebits::engine::LogLevel::Info>(::std::format(__VA_ARGS__))
#define WARN(...) ::genebits::engine::PublishLog<::genebits::engine::LogLevel::Warn>(::std::format(__VA_ARGS__))
#define ERROR(...) ::genebits::engine::PublishLog<::genebits::engine::LogLevel::Error>(::std::format(__VA_ARGS__))
#else
#define TRACE(...)
#define INFO(...)
#define WARN(...)
#define ERROR(...)
#endif

#endif
