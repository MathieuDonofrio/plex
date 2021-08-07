#ifndef GENEBITS_ENGINE_UTIL_EVENTBUS_H
#define GENEBITS_ENGINE_UTIL_EVENTBUS_H

#include <memory>
#include <mutex>
#include <vector>

#include "engine/events/event_handler.h"
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
      handlers_.push_back(handler);
    }

    void Remove(EventHandler<Event> handler)
    {
      auto it = std::find(handlers_.begin(), handlers_.end(), handler);

      [[likely]] if (it != handlers_.end())
      {
        [[likely]] if (handlers_.size() > 1)
        {
          std::iter_swap(it, handlers_.end() - 1);
          handlers_.pop_back();
        }
        else handlers_.clear();
      }
    }

    [[nodiscard]] size_t Count() const
    {
      return handlers_.size();
    }

  private:
    std::vector<EventHandler<Event>> handlers_;
  };
} // namespace

class EventBus
{
public:
  template<typename Event>
  void Publish(const Event& event)
  {
    std::scoped_lock<std::mutex> lock_guard(lock_);

    Assure<Event>()->Propagate(event);
  }

  template<typename Event>
  void Subscribe(EventHandler<Event> handler)
  {
    std::scoped_lock<std::mutex> lock_guard(lock_);

    Assure<Event>()->Add(handler);
  }

  template<typename Event>
  void Unsubscribe(EventHandler<Event> handler)
  {
    std::scoped_lock<std::mutex> lock_guard(lock_);

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
  std::mutex lock_;
};

} // namespace genebits::engine

#endif
