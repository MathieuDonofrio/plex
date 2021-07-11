#ifndef GENEBITS_ENGINE_UTIL_RING_BUFFER_H
#define GENEBITS_ENGINE_UTIL_RING_BUFFER_H

#include <mutex>

#include "engine/util/concepts.h"
#include "engine/util/concurrency.h"

namespace genebits::engine
{
template<POD Type, size_t Capacity, ConcurrencyModel Concurrency = ConcurrencyModel::None>
requires POT<Capacity> class RingBuffer;

template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyModel::None>
{
public:
  static constexpr size_t cMask = Capacity - 1;

  constexpr bool Enqueue(Type item) noexcept
  {
    const size_t next_head = head_ + 1;

    if (next_head != tail_)
    {
      buffer_[head_ & cMask] = item;

      head_ = next_head;

      return true;
    }

    return false;
  }

  constexpr bool Dequeue(Type& item) noexcept
  {
    if (tail_ == head_) return false;

    item = buffer_[tail_ & cMask];

    ++tail_;

    return true;
  }

  constexpr void Clear() noexcept
  {
    tail_ = 0;
    head_ = 0;
  }

  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return head_ - tail_;
  }

  [[nodiscard]] constexpr bool Empty() const noexcept
  {
    return head_ == tail_;
  }

private:
  Type buffer_[Capacity];
  size_t head_;
  size_t tail_;
};

template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyModel::ProducerConsumer>
{
public:
  static constexpr size_t cCachePaddedBufferSize = Capacity * sizeof(Type) < cCacheLineSize
                                                     ? cCacheLineSize / sizeof(Type)
                                                     : Capacity;
  static constexpr size_t cMask = Capacity - 1;

  bool Enqueue(Type item) noexcept
  {
    const size_t current_head = head_.load(std::memory_order_relaxed);
    const size_t next_head = current_head + 1;

    if (next_head != tail_.load(std::memory_order_acquire))
    {
      buffer_[current_head & cMask] = item;

      head_.store(next_head, std::memory_order_release);

      return true;
    }

    return false;
  }

  bool Dequeue(Type& item) noexcept
  {
    const size_t current_tail = tail_.load(std::memory_order::relaxed);

    if (current_tail == head_.load(std::memory_order::acquire)) return false;

    item = buffer_[current_tail & cMask];

    tail_.store(current_tail + 1, std::memory_order_release);

    return true;
  }

  void Clear() noexcept
  {
    tail_.store(0, std::memory_order::relaxed);
    head_.store(0, std::memory_order::relaxed);
  }

  [[nodiscard]] size_t Size() const noexcept
  {
    return head_.load(std::memory_order::acquire) - tail_.load(std::memory_order::relaxed);
  }

  [[nodiscard]] bool Empty() const noexcept
  {
    return head_.load(std::memory_order::acquire) == tail_.load(std::memory_order::relaxed);
  }

private:
  std::atomic<size_t> head_;

  // Separate head and tail by buffer to put them on different cachelines
  // and avoid cache synchronization.
  Type buffer_[cCachePaddedBufferSize];

  std::atomic<size_t> tail_;
};

} // namespace genebits::engine

#endif
