#ifndef GENEBITS_ENGINE_DEBUG_TERMINAL_LOGGER_H
#define GENEBITS_ENGINE_DEBUG_TERMINAL_LOGGER_H

#include <mutex>

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/events/listener.h"

namespace genebits::engine
{
///
/// Terminal logger.
///
/// Logs messages to stdout with colors and stack traces.
///
/// Used for active debugging purposes.
///
class TerminalLogger : public Listener<TerminalLogger, LogEvent>
{
public:
  ///
  /// Constructor.
  ///
  /// @tparam HotEvent Hot event types for the event bus.
  ///
  /// @param[in] bus Bus to subscribe logger to.
  ///
  explicit TerminalLogger(EventBus& bus) : Listener<TerminalLogger, LogEvent>(bus) {};

  ///
  /// Receives log events.
  ///
  /// @param[in] event Received event.
  ///
  void Listen(const LogEvent& event);

private:
  std::mutex mutex_;
};
} // namespace genebits::engine

#endif
