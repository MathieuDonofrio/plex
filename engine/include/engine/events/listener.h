#ifndef GENEBITS_ENGINE_UTIL_LISTENER_H
#define GENEBITS_ENGINE_UTIL_LISTENER_H

#include <type_traits>

#include "engine/core/environment.h"
#include "engine/events/event_bus.h"

namespace genebits::engine
{
///
/// Concept to verify that a listener class can listen to a specified event type.
///
/// Meets the concept requirements if the listener type defines a listen method
/// that takes a event of the event type as an argument.
///
/// @tparam Listener Listener to check
/// @tparam Event Event type to check.
///
template<typename Listener, typename Event>
concept EventListen = requires(Listener listener, const Event& event)
{
  listener.listen(event);
};

///
/// Utility listener class to remove a lot of a boiler plate for listening to events. Uses RAII for
/// auto subscription and unsubscription to the event bus.
///
/// The implementation of the listener must contain a listen method for every event type. This is
/// checked at compile time.
///
/// You can default construct a listener that uses the environment event bus or you can explicitly specify
/// own event bus in the constructor.
///
/// @tparam Impl Implementation of the listener (parent).
/// @tparam Events Event types to subscribe to.
///
template<typename Impl, typename... Events>
class Listener
{
public:
  ///
  /// Parametric constructor.
  ///
  /// @param bus The bus to subscribe all event handlers to.
  ///
  constexpr explicit Listener(EventBus& bus) noexcept
    : bus_(&bus)
  {
    (bus_->Subscribe(GetEventHandler<Events>()), ...);
  }

  ///
  /// Default constructor. Uses the environment event bus.
  ///
  Listener() noexcept
    : Listener(GetEnvironment().GetEventBus())
  {
  }

  ///
  /// Destructor.
  ///
  ~Listener() noexcept
  {
    (bus_->Unsubscribe(GetEventHandler<Events>()), ...);
  }

  ///
  /// Returns the event handler for the specified event type.
  ///
  /// @tparam Event Event type to get handler for.
  ///
  /// @return Event handler for event type.
  ///
  template<typename Event>
  requires EventListen<Impl, Event>
    EventHandler<Event> GetEventHandler()
  noexcept
  {
    static_assert(std::is_base_of_v<Listener<Impl, Events...>, Impl>, "Listener must be base of implementation");

    EventHandler<Event> handler;
    handler.template Bind<&Impl::listen, Impl>(static_cast<Impl*>(this));
    return handler;
  }

private:
  EventBus* bus_;
};

} // namespace genebits::engine

#endif
