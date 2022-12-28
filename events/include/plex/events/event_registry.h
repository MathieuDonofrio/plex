#ifndef PLEX_EVENTS_EVENT_REGISTRY_H
#define PLEX_EVENTS_EVENT_REGISTRY_H

#include "plex/events/event_queue.h"

namespace plex
{
class EventRegistry
{
private:
  struct Updater
  {
    void* queue;
    void (*update)(void*);
  };

public:
  template<typename EventType>
  void AddQueue(EventQueue<EventType>* queue)
  {
    Updater updater { queue, [](void* queue) { static_cast<EventQueue<EventType>*>(queue)->Update(); } };

    updaters_.push_back(updater);
  }

  void Update()
  {
    for (const Updater& updater : updaters_)
    {
      updater.update(updater.queue);
    }
  }

private:
  Vector<Updater> updaters_;
};
} // namespace plex

#endif
