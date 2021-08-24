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

[[nodiscard]] constexpr size_t GetStackFramesAmount(const LogLevel leve)
{
  switch (leve)
  {
  case LogLevel::Trace: return 0;
  case LogLevel::Info: return 4;
  case LogLevel::Warn: return 8;
  case LogLevel::Error: return 16;
  default: return 0;
  }
}

inline void PublishLog(std::string&& message, LogMetadata metadata, EventBus& bus = GetEnvironment().GetEventBus())
{
  LogEvent event;

  event.log.metadata = std::move(metadata);
  event.log.message = std::move(message);

  bus.Publish(event);
}

} // namespace genebits::engine

#define CREATE_LOG_METADATA(level)                                                             \
  ::genebits::engine::LogMetadata                                                              \
  {                                                                                            \
    level, ::genebits::engine::StackBackTrace(GetStackFramesAmount(level)), __FILE__, __LINE__ \
  }

#ifndef NDEBUG

#define LOG(level, ...) ::genebits::engine::PublishLog(::std::format(__VA_ARGS__), CREATE_LOG_METADATA(level))

#define LOG_TRACE(...) LOG(::genebits::engine::LogLevel::Trace, __VA_ARGS__)
#define LOG_INFO(...) LOG(::genebits::engine::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) LOG(::genebits::engine::LogLevel::Warn, __VA_ARGS__)
#define LOG_ERROR(...) LOG(::genebits::engine::LogLevel::Error, __VA_ARGS__)

#else

#define LOG(level, ...)

#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)

#endif

#endif
