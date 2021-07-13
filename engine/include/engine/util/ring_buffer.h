#ifndef GENEBITS_ENGINE_UTIL_RING_BUFFER_H
#define GENEBITS_ENGINE_UTIL_RING_BUFFER_H

#include <mutex>

#include "engine/util/concepts.h"
#include "engine/util/concurrency.h"

namespace genebits::engine
{
///
/// High performance circular buffer.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
/// @tparam Guarantee The concurrency guarantee of the ring buffer.
///
template<POD Type, size_t Capacity, ConcurrencyGuarantee Guarantee = ConcurrencyGuarantee::None>
requires POT<Capacity> class RingBuffer;

///
/// High performance circular buffer. Not guaranteed to be thread-safe.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
///
template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyGuarantee::None>
{
public:
  static constexpr size_t cMask = Capacity - 1;

  ///
  /// Default Constructor.
  ///
  constexpr RingBuffer()
    : head_(0), tail_(0)
  {
  }

  ///
  /// Adds the item to the head of the ring buffer.
  ///
  /// @param[in] item The item to add.
  ///
  /// @return True if the item was enqueued, false otherwise.
  ///
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

  ///
  /// Removes the item to the tail of the ring buffer.
  ///
  /// @param[out] item The dequeued item.
  ///
  /// @return True if the item was dequeued, false otherwise.
  ///
  constexpr bool Dequeue(Type& item) noexcept
  {
    if (tail_ == head_) return false;

    item = buffer_[tail_ & cMask];

    ++tail_;

    return true;
  }

  ///
  /// Clears the ring buffer.
  ///
  constexpr void Clear() noexcept
  {
    tail_ = 0;
    head_ = 0;
  }

  ///
  /// Returns the amount of items in the ring buffer.
  ///
  /// @return Size of the ring buffer.
  ///
  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return head_ - tail_;
  }

  ///
  /// Returns whether or not the ring buffer is empty
  ///
  /// return True if the ring buffer is empty, false otherwise.
  ///
  [[nodiscard]] constexpr bool Empty() const noexcept
  {
    return head_ == tail_;
  }

private:
  size_t head_;
  size_t tail_;
  Type buffer_[Capacity];
};

namespace details
{
  ///
  /// Base class for the concurrent version of the ring buffer. Used to reduce boiler plate.
  ///
  /// @tparam Type Type to enqueue & dequeue. Must be plain old data.
  /// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
  /// @tparam Guarantee The concurrency guarantee of the ring buffer.
  ///
  template<typename Type, size_t Capacity, ConcurrencyGuarantee Guarantee>
  class ConcurrentRingBuffer
  {
  public:
    static constexpr size_t cCachePaddedBufferSize = Capacity * sizeof(Type) < cCacheLineSize
                                                       ? cCacheLineSize / sizeof(Type)
                                                       : Capacity;
    static constexpr size_t cMask = Capacity - 1;

    ///
    /// Default Constructor.
    ///
    ConcurrentRingBuffer()
      : head_(0), tail_(0)
    {
    }

  protected:
    ///
    /// Enqueues the item.
    ///
    /// @tparam LoadOrder Memory order for loading the tail.
    /// @tparam StoreOrder Memory order for storing the head.
    ///
    /// @param[in] item Item to enqueue.
    ///
    /// @return True if item was enqueued, false otherwise.
    ///
    template<std::memory_order LoadOrder, std::memory_order StoreOrder>
    bool Enqueue(Type item) noexcept
    {
      const size_t current_head = head_.load(std::memory_order_relaxed);
      const size_t next_head = current_head + 1;

      if (next_head != tail_.load(LoadOrder))
      {
        buffer_[current_head & cMask] = item;

        head_.store(next_head, StoreOrder);

        return true;
      }

      return false;
    }

    ///
    /// Dequeues the item.
    ///
    /// @tparam LoadOrder Memory order for loading the head.
    /// @tparam StoreOrder Memory order for storing the tail.
    ///
    /// @param[out] item Dequeued item.
    ///
    /// @return True if item was dequeued, false otherwise.
    ///
    template<std::memory_order LoadOrder, std::memory_order StoreOrder>
    bool Dequeue(Type& item) noexcept
    {
      const size_t current_tail = tail_.load(std::memory_order_relaxed);

      if (current_tail == head_.load(LoadOrder)) return false;

      item = buffer_[current_tail & cMask];

      tail_.store(current_tail + 1, StoreOrder);

      return true;
    }

  public:
    ///
    /// Clears the ring buffer.
    ///
    void Clear() noexcept
    {
      tail_.store(0, std::memory_order_seq_cst);
      head_.store(0, std::memory_order_seq_cst);
    }

    ///
    /// Returns the amount of items in the ring buffer.
    ///
    /// @return Size of the ring buffer.
    ///
    [[nodiscard]] size_t Size() const noexcept
    {
      return head_.load(std::memory_order_relaxed) - tail_.load(std::memory_order_relaxed);
    }

    ///
    /// Returns whether or not the ring buffer is empty
    ///
    /// return True if the ring buffer is empty, false otherwise.
    ///
    [[nodiscard]] bool Empty() const noexcept
    {
      return head_.load(std::memory_order_relaxed) == tail_.load(std::memory_order_relaxed);
    }

  protected:
    std::atomic<size_t> head_;

    // Separate head and tail by buffer to put them on different cachelines
    // and avoid cache synchronization.
    Type buffer_[cCachePaddedBufferSize];

    std::atomic<size_t> tail_;
  };
} // namespace details

///
/// High performance circular buffer. Thread-safe guaranteed for a single producer and single
/// consumer thread.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
///
template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToOne>
  : public details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToOne>
{
private:
  using Base = details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToOne>;

public:
  bool Enqueue(Type item) noexcept
  {
    return Base::Enqueue<std::memory_order::acquire, std::memory_order::release>(item);
  }

  bool Dequeue(Type& item) noexcept
  {
    return Base::Dequeue<std::memory_order::acquire, std::memory_order::release>(item);
  }
};

///
/// High performance circular buffer. Thread-safe guaranteed for a single producer and multiple
/// consumer threads.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
///
template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToMany>
  : public details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToMany>
{
private:
  using Base = details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::OneToMany>;

public:
  ///
  /// Adds the item to the head of the ring buffer.
  ///
  /// @param[in] item The item to add.
  ///
  /// @return True if the item was enqueued, false otherwise.
  ///
  bool Enqueue(Type item) noexcept
  {
    return Base::Enqueue<std::memory_order::acquire, std::memory_order::release>(item);
  }

  ///
  /// Removes the item to the tail of the ring buffer.
  ///
  /// @param[out] item The dequeued item.
  ///
  /// @return True if the item was dequeued, false otherwise.
  ///
  bool Dequeue(Type& item) noexcept
  {
    return Base::Dequeue<std::memory_order::acquire, std::memory_order::seq_cst>(item);
  }
};

///
/// High performance circular buffer. Thread-safe guaranteed for a many producers and multiple
/// consumer threads.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
///
template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToOne>
  : public details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToOne>
{
private:
  using Base = details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToOne>;

public:
  ///
  /// Adds the item to the head of the ring buffer.
  ///
  /// @param[in] item The item to add.
  ///
  /// @return True if the item was enqueued, false otherwise.
  ///
  bool Enqueue(Type item) noexcept
  {
    return Base::Enqueue<std::memory_order::acquire, std::memory_order::seq_cst>(item);
  }

  ///
  /// Removes the item to the tail of the ring buffer.
  ///
  /// @param[out] item The dequeued item.
  ///
  /// @return True if the item was dequeued, false otherwise.
  ///
  bool Dequeue(Type& item) noexcept
  {
    return Base::Dequeue<std::memory_order::acquire, std::memory_order::release>(item);
  }
};

///
/// High performance circular buffer. Always guaranteed to be thread-safe.
///
/// @tparam Type Type to enqueue & dequeue. Must be plain old data.
/// @tparam Capacity The maximum capacity of the ring buffer. Must be a power of two.
///
template<typename Type, size_t Capacity>
class RingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToMany>
  : public details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToMany>
{
private:
  using Base = details::ConcurrentRingBuffer<Type, Capacity, ConcurrencyGuarantee::ManyToMany>;

public:
  ///
  /// Adds the item to the head of the ring buffer.
  ///
  /// @param[in] item The item to add.
  ///
  /// @return True if the item was enqueued, false otherwise.
  ///
  bool Enqueue(Type item) noexcept
  {
    return Base::Enqueue<std::memory_order::seq_cst, std::memory_order::seq_cst>(item);
  }

  ///
  /// Removes the item to the tail of the ring buffer.
  ///
  /// @param[out] item The dequeued item.
  ///
  /// @return True if the item was dequeued, false otherwise.
  ///
  bool Dequeue(Type& item) noexcept
  {
    return Base::Dequeue<std::memory_order::seq_cst, std::memory_order::seq_cst>(item);
  }
};

} // namespace genebits::engine

#endif
