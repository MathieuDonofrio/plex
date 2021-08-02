#ifndef GENEBITS_ENGINE_UTIL_LISTENER_H
#define GENEBITS_ENGINE_UTIL_LISTENER_H

#include "engine/events/event_bus.h"

namespace genebits::engine
{

template<typename Impl /* Use Concepts */, typename Event>
class StaticListener
{
public:
  // Experimental: dont use yet

  StaticListener()
  {
    StaticEventBus::Subscribe<Event>({ &Impl::listen, this });
  }

  ~StaticListener()
  {
    StaticEventBus::Unsubscribe<Event>({ &Impl::listen, this });
  }
};

template<typename Event>
class Listener
{
public:
  Listener()
  {
    StaticEventBus::Subscribe({ &Listener<Event>::listen, this });
  }

  ~Listener()
  {
    StaticEventBus::Unsubscribe({ &Listener<Event>::listen, this });
  }

  virtual void listen(const Event& event) = 0;
};
} // namespace genebits::engine

#endif
