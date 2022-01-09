#ifndef GENEBITS_ENGINE_DEBUG_LOGGING_H
#define GENEBITS_ENGINE_DEBUG_LOGGING_H

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
  const char* file_name;
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
/// Singleton event bus used for logging.
///
/// @return Event bus used for logging.
///
EventBus& GetLoggingEventBus();

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
/// Constructs & publishes a log event on the event bus.
///
/// @param[in] message The log message.
/// @param[in] metadata The log metadata.
/// @param[in] bus The event bus to publish on.
///
inline void PublishLog(std::string&& message, LogMetadata&& metadata, EventBus& bus = GetLoggingEventBus())
{
  LogEvent event;

  event.log.metadata = std::move(metadata);
  event.log.message = std::move(message);

  bus.Publish(event);
}

} // namespace genebits::engine

#define CREATE_LOG_METADATA(level)                                                           \
  ::genebits::engine::LogMetadata                                                            \
  {                                                                                          \
    level, ::genebits::engine::StackBackTrace(GetStackTraceDepth(level)), __FILE__, __LINE__ \
  }

#ifndef NDEBUG

#ifdef cpp_lib_format
#include <format>
#define FORMAT(...) ::std::format(VA_ARGS__)
#else
#define FORMAT(string, ...) string
#endif

#define LOG(level, ...) ::genebits::engine::PublishLog(FORMAT(__VA_ARGS__), CREATE_LOG_METADATA(level))

#define LOG_TRACE(...) LOG(::genebits::engine::LogLevel::Trace, __VA_ARGS__)
#define LOG_INFO(...) LOG(::genebits::engine::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) LOG(::genebits::engine::LogLevel::Warn, __VA_ARGS__)
#define LOG_ERROR(...) LOG(::genebits::engine::LogLevel::Error, __VA_ARGS__)

#else

#define LOG(level, ...) ((void)0)

#define LOG_TRACE(...) ((void)0)
#define LOG_INFO(...) ((void)0)
#define LOG_WARN(...) ((void)0)
#define LOG_ERROR(...) ((void)0)

#endif

#endif
