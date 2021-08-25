#ifndef GENEBITS_ENGINE_CORE_ENVIRONMENT_H
#define GENEBITS_ENGINE_CORE_ENVIRONMENT_H

#include "genebits/engine/events/event_bus.h"

namespace genebits::engine
{
///
/// Class that holds instances of managers for the engine.
///
/// This class is usually provided as a singleton.
///
class Environment final
{
public:
  ///
  /// Default constructor
  ///
  Environment();

  ///
  /// Destructor
  ///
  ~Environment();

  ///
  /// Returns the event bus for the environment. The event bus is used to
  /// subscribe event handlers to and publish events.
  ///
  /// @return Environment event bus.
  ///
  [[nodiscard]] EventBus& GetEventBus() noexcept
  {
    return event_bus_;
  }

private:
  EventBus event_bus_;

  // Pimpl for hidden dependencies
  struct Pimpl;
  Pimpl* pimpl_;
};

namespace details
{
  extern Environment environment;
}

///
/// Returns the singleton instance of the environment. The environment holds various
/// managers for the engine.
///
/// @return Global singleton environment instance.
///
inline Environment& GetEnvironment()
{
  return details::environment;
}
} // namespace genebits::engine

#endif
