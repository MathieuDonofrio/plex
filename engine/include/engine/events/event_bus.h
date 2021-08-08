#ifndef GENEBITS_ENGINE_UTIL_EVENTBUS_H
#define GENEBITS_ENGINE_UTIL_EVENTBUS_H

#include <memory>
#include <mutex>

#include "engine/events/event_handler.h"
#include "engine/util/fast_vector.h"
#include "engine/util/meta.h"
#include "engine/util/type_map.h"

namespace genebits::engine
{
namespace
{
  template<typename Event>
  class EventHandlerPool
  {
  public:
    void Propagate(const Event& event)
    {
      for (EventHandler<Event>& handler : handlers_)
      {
        handler.Invoke(event);
      }
    }

    void Add(EventHandler<Event> handler)
    {
      handlers_.PushBack(handler);
    }

    void Remove(EventHandler<Event> handler)
    {
      for (size_t i = 0; i != handlers_.Size(); i++)
      {
        if (handlers_[i] == handler)
        {
          handlers_.Erase(i);
          return;
        }
      }
    }

    [[nodiscard]] size_t Count() const
    {
      return handlers_.Size();
    }

  private:
    FastVector<EventHandler<Event>> handlers_;
  };
} // namespace

class EventBus
{
public:
  template<typename Event>
  void Publish(const Event& event)
  {
    Assure<Event>()->Propagate(event);
  }

  template<typename Event>
  void Subscribe(EventHandler<Event> handler)
  {
    Assure<Event>()->Add(handler);
  }

  template<typename Event>
  void Unsubscribe(EventHandler<Event> handler)
  {
    Assure<Event>()->Remove(handler);
  }

  template<typename Event>
  [[nodiscard]] size_t Count()
  {
    return Assure<Event>()->Count();
  }

private:
  template<typename Event>
  std::shared_ptr<EventHandlerPool<Event>> Assure() noexcept
  {
    auto& pool = pools_.template Assure<Event>();

    if (!pool)
    {
      pool = std::static_pointer_cast<void>(std::make_shared<EventHandlerPool<Event>>());
    }

    return std::static_pointer_cast<EventHandlerPool<Event>>(pool);
  }

private:
  TypeMap<std::shared_ptr<void>> pools_;
};

} // namespace genebits::engine

#endif
