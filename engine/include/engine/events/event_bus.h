#ifndef GENEBITS_ENGINE_UTIL_EVENTBUS_H
#define GENEBITS_ENGINE_UTIL_EVENTBUS_H

#include <memory>
#include <mutex>
#include <vector>

#include "engine/events/event_handler.h"
#include "engine/util/meta.h"

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

    assure<Event>()->Propagate(event);
  }

  template<typename Event>
  void Subscribe(EventHandler<Event> handler)
  {
    std::scoped_lock<std::mutex> lock_guard(lock_);

    assure<Event>()->Add(handler);
  }

  template<typename Event>
  void Unsubscribe(EventHandler<Event> handler)
  {
    std::scoped_lock<std::mutex> lock_guard(lock_);

    assure<Event>()->Remove(handler);
  }

  template<typename Event>
  [[nodiscard]] size_t Count()
  {
    return assure<Event>()->Count();
  }

private:
  template<typename Event>
  std::shared_ptr<EventHandlerPool<Event>> assure()
  {
    size_t index = Meta<EventBus>::UniqueId<Event>();

    if (index >= pools_.size())
    {
      pools_.resize(index + 1);
    }

    auto& pool = pools_[index];

    if (!pool)
    {
      pool = std::static_pointer_cast<void>(std::make_shared<EventHandlerPool<Event>>());
    }

    return std::static_pointer_cast<EventHandlerPool<Event>>(pool);
  }

private:
  std::vector<std::shared_ptr<void>> pools_;
  std::mutex lock_;
};

class StaticEventBus
{
public:
  template<typename Event>
  static void Publish(const Event& event)
  {
    bus.Publish(event);
  }

  template<typename Event>
  static void Subscribe(EventHandler<Event> handler)
  {
    bus.Subscribe(handler);
  }

  template<typename Event>
  static void Unsubscribe(EventHandler<Event> handler)
  {
    bus.Unsubscribe(handler);
  }

  template<typename Event>
  static [[nodiscard]] size_t Count()
  {
    return bus.Count();
  }

private:
  static EventBus bus;
};

} // namespace genebits::engine

#endif
