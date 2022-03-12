#ifndef GENEBITS_ENGINE_UTIL_EVENTBUS_H
#define GENEBITS_ENGINE_UTIL_EVENTBUS_H

#include <memory>
#include <mutex>

#include "genebits/engine/containers/type_map.h"
#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/delegate.h"
#include "genebits/engine/utilities/erased_ptr.h"

namespace genebits::engine
{
///
/// Delegate alias for handling events.
///
/// @see Delegate
///
/// @tparam Event Type of event for the delegate to handle.
///
template<typename Event>
using EventHandler = Delegate<void(const Event&)>;

namespace details
{
  ///
  /// Pool of event handlers.
  ///
  /// @tparam Event Event type to handle
  ///
  template<typename Event>
  class EventHandlerPool
  {
  public:
    ///
    /// Propagates the event by invoking every handler sequentially.
    ///
    /// @param[in] event Event to propagate.
    ///
    void Propagate(const Event& event) noexcept
    {
      for (EventHandler<Event>& handler : handlers_)
      {
        handler.Invoke(event);
      }
    }

    ///
    /// Adds the event handler to the pool.
    ///
    /// @param[in] handler Handler to add.
    ///
    void Add(EventHandler<Event> handler) noexcept
    {
      handlers_.PushBack(handler);
    }

    ///
    /// Removes the event handler from the pool.
    ///
    /// @param[in] handler Handle to remove
    ///
    void Remove(EventHandler<Event> handler) noexcept
    {
      handlers_.UnorderedErase(std::ranges::find(handlers_, handler));
    }

    ///
    /// Returns the amount of handlers currently in the pool.
    ///
    /// @return Amount of stored handlers.
    ///
    [[nodiscard]] size_t Count() const noexcept
    {
      return handlers_.size();
    }

  private:
    Vector<EventHandler<Event>> handlers_;
  };
} // namespace details

///
/// Event bus is used as a container for event handlers of any event type.
///
/// You can subscribe event handlers to the bus and publish events.
///
class EventBus
{
public:
  ///
  /// Publishes the specified event to all event handlers for the event type.
  ///
  /// @tparam Event Type of event to publish.
  ///
  /// @param[in] event Event to publish.
  ///
  template<typename Event>
  void Publish(const Event& event) noexcept
  {
    Assure<Event>()->Propagate(event);
  }

  ///
  /// Subscribes the event handler to the bus. Subscribed event handlers will
  /// receive all published event of the same type.
  ///
  /// @tparam Event Type of event.
  ///
  /// @param[in] handler Handler of the event.
  ///
  template<typename Event>
  void Subscribe(EventHandler<Event> handler) noexcept
  {
    Assure<Event>()->Add(handler);
  }

  ///
  /// Unsubscribes the event handle to from the bus.
  ///
  /// The event handler will no longer receive published events in from this
  /// event bus.
  ///
  /// @tparam Event Type of event.
  ///
  /// @param[in] handler Handler of the event.
  ///
  template<typename Event>
  void Unsubscribe(EventHandler<Event> handler) noexcept
  {
    Assure<Event>()->Remove(handler);
  }

  ///
  /// Returns the amount of event handlers currently subscribed to the templated
  /// event type.
  ///
  /// @tparam Event Type of event to get handlers count for.
  ///
  /// @return Amount of event handlers subscribed for the event type.
  ///
  template<typename Event>
  [[nodiscard]] size_t Count()
  {
    return Assure<Event>()->Count();
  }

private:
  ///
  /// Safely returns the event handler pool for the event type. Properly initializes the pool if
  /// it has never been accessed before.
  ///
  /// @tparam Event Event type to get handlers pool for.
  ///
  /// @return Event handler pool for the event type.
  ///
  template<typename Event>
  details::EventHandlerPool<Event>* Assure() noexcept
  {
    auto& pool = pools_.Assure<Event>();

    if (!pool) [[unlikely]] { pool = std::move(MakeErased<details::EventHandlerPool<Event>>()); }

    return static_cast<details::EventHandlerPool<Event>*>(pool.Get());
  }

private:
  TypeMap<ErasedPtr<void>> pools_;
};

} // namespace genebits::engine

#endif
