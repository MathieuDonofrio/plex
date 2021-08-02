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
    StaticEventBus<Event>::Subscribe({ &Impl::listen, this });
  }

  ~StaticListener()
  {
    StaticEventBus<Event>::Unsubscribe({ &Impl::listen, this });
  }
};

template<typename Event>
class VirtualListener
{
public:
  VirtualListener()
  {
    StaticEventBus<Event>::Subscribe({ &listen, this });
  }

  ~VirtualListener()
  {
    StaticEventBus<Event>::Unsubscribe({ &listen, this });
  }

  virtual void listen(const Event& event) = 0;
};
} // namespace genebits::engine

#endif
