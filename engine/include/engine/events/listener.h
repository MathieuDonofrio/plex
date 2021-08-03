#ifndef GENEBITS_ENGINE_UTIL_LISTENER_H
#define GENEBITS_ENGINE_UTIL_LISTENER_H

#include "engine/events/event_bus.h"

namespace genebits::engine
{

template<typename Impl /* Use Concepts */, typename Event>
class StaticListener
{
public:
  StaticListener()
  {
    StaticEventBus::Subscribe(handler());
  }

  ~StaticListener()
  {
    StaticEventBus::Unsubscribe(handler());
  }

private:
  EventHandler<Event> handler() const
  {
    EventHandler<Event> handler;
    handler.bind<&Impl::listen>(this);
  }
};

template<typename Event>
class Listener
{
public:
  Listener()
  {
    StaticEventBus::Subscribe(handler());
  }

  ~Listener()
  {
    StaticEventBus::Unsubscribe(handler());
  }

  virtual void listen(const Event& event) = 0;

private:
  EventHandler<Event> handler() const
  {
    EventHandler<Event> handler;
    handler.bind<&Listener::listen>(this);
  }
};
} // namespace genebits::engine

#endif
