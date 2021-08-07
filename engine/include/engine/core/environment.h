#ifndef GENEBITS_ENGINE_CORE_ENVIRONMENT_H
#define GENEBITS_ENGINE_CORE_ENVIRONMENT_H

#include "engine/events/event_bus.h"

namespace genebits::engine
{
class Environment final
{
public:
  [[nodiscard]] EventBus& GetEventBus() noexcept
  {
    return event_bus_;
  }

  [[nodiscard]] const EventBus& GetEventBus() const noexcept
  {
    return event_bus_;
  }

private:
  EventBus event_bus_;
};

namespace details
{
  extern Environment environment;
}

[[maybe_unused]] inline Environment& GetEnvironment()
{
  return details::environment;
}
} // namespace genebits::engine

#endif
