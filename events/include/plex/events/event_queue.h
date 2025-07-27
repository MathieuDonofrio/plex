#ifndef PLEX_EVENTS_EVENT_BUFFER_H
#define PLEX_EVENTS_EVENT_BUFFER_H

#include "plex/containers/deque.h"
#include "plex/containers/vector.h"

namespace plex
{
template<typename Event>
class EventQueue
{
public:
  ///
  /// Default constructor
  ///
  EventQueue() : pivot_(0) {}

  ///
  /// Frees up the memory persisted from the last call to update and updates all consumer pointers.
  ///
  /// Should be called once all consumers have had the chance to consume events at least once. It is guaranteed that all
  /// consumers have had a chance to consume events if all active systems have been called at least once.
  ///
  void Update()
  {
    const size_t old_pivot = pivot_;

    // Update all consumer pointers

    for (size_t* consume : consumers_)
    {
      const size_t old_consume = *consume;

      *consume = old_consume <= old_pivot ? 0 : old_consume - old_pivot; // max(0, old_consume - old_pivot)
    }

    // Free memory

    for (size_t i = 0; i != old_pivot; ++i)
    {
      queue_.pop_front();
    }

    pivot_ = queue_.size();
  }

  template<typename... Args>
  void Enqueue(Args... args)
  {
    queue_.emplace_back(std::forward<Args>(args)...);
  }

  void AddConsumer(size_t* consumer)
  {
    consumers_.push_back(consumer);
  }

  const Event& At(size_t index) const noexcept
  {
    return queue_[index];
  }

  [[nodiscard]] size_t size() const noexcept
  {
    return queue_.size();
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return queue_.empty();
  }

private:
  Deque<Event> queue_;
  size_t pivot_;

  Vector<size_t*> consumers_;
};
} // namespace plex

#endif
