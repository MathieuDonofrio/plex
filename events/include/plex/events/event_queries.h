#ifndef PLEX_EVENTS_EVENT_QUERIES_H
#define PLEX_EVENTS_EVENT_QUERIES_H

#include "plex/events/event_queue.h"
#include "plex/events/event_registry.h"
#include "plex/system/query.h"

namespace plex
{
namespace details
{
  template<typename>
  struct EventCursor
  {
    size_t index;
  };
} // namespace details

template<typename Type>
class Event
{
public:
  using EventType = std::remove_const_t<Type>;

  static Event<Type> Fetch(void*, Context& global_context, Context& local_context)
  {
    if (!global_context.Contains<EventQueue<EventType>>()) [[unlikely]]
    {
      global_context.Emplace<EventQueue<EventType>>();

      EventRegistry& registry = global_context.template Get<EventRegistry>();

      registry.AddQueue(&global_context.Get<EventQueue<EventType>>());
    }

    if (!local_context.Contains<details::EventCursor<EventType>>()) [[unlikely]]
    {
      local_context.Emplace<details::EventCursor<EventType>>(details::EventCursor<EventType> { 0 });

      details::EventCursor<EventType>& cursor = local_context.template Get<details::EventCursor<EventType>>();

      global_context.template Get<EventQueue<EventType>>().AddConsumer(&cursor.index);
    }

    auto& queue = global_context.template Get<EventQueue<EventType>>();
    auto& cursor = local_context.template Get<details::EventCursor<EventType>>();

    return { queue, cursor };
  }

  static consteval std::array<QueryDataAccess, 1> GetDataAccess() noexcept
  {
    return { QueryDataAccess {
      TypeName<EventType>(),
      TypeName<EventRegistry>(), // Access entire data source
      std::is_const_v<Type>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Type>::value // Check ThreadSafe trait to see if the access is thread-safe.
    } };
  }

public:
  void Send(const EventType& event)
  requires(!std::is_const_v<Type>)
  {
    queue_.Enqueue(event);
  }

  [[nodiscard]] bool HasNext() const noexcept
  {
    return local_cursor_.index < queue_.size();
  }

  const EventType& Next() const noexcept
  {
    return queue_.At(local_cursor_.index++);
  }

  void Skip() noexcept
  {
    ++local_cursor_.index;
  }

private:
  Event(EventQueue<EventType>& queue, details::EventCursor<EventType>& local_cursor) noexcept
    : queue_(queue), local_cursor_(local_cursor)
  {}

private:
  EventQueue<EventType>& queue_;
  details::EventCursor<EventType>& local_cursor_;
};
} // namespace plex

#endif
