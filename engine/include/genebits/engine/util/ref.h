#ifndef GENEBITS_ENGINE_UTIL_REF_H
#define GENEBITS_ENGINE_UTIL_REF_H

#include <atomic>
#include <concepts>
#include <cstdint>
#include <thread>
#include <type_traits>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/local_thread_validator.h"

namespace genebits::engine
{
// SharedPtr Overhead:
// - Compulsive atomic ref counting
// - Custom deleters
// - Weak pointer support
// - No support for intrusive reference counting

using RefCounter = uint_fast32_t;

template<typename Type>
concept IntrusiveRefType = requires(const Type instance, size_t amount)
{
  // Style Exception: STL
  // clang-format off

  instance.IntrusiveAddRef();
  {instance.IntrusiveDropRef() } -> std::convertible_to<bool>;
  {instance.IntrusiveUniqueRef()} -> std::convertible_to<bool>;
  {instance.IntrusiveRefCount()} -> std::convertible_to<size_t>;

  // clang-format on
};

class AtomicRefCounted
{
public:
  constexpr AtomicRefCounted() noexcept : counter_(0) {}

  constexpr AtomicRefCounted(const AtomicRefCounted&) noexcept : AtomicRefCounted() {}

  AtomicRefCounted& operator=(const AtomicRefCounted&) noexcept
  {
    return *this;
  }

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  constexpr ~AtomicRefCounted() noexcept
  {
    if (!std::is_constant_evaluated())
    {
      ASSERT(counter_.load(std::memory_order_relaxed) == static_cast<RefCounter>(-1), "Ref counter invalid");
    }
  }
#endif

private:
  template<typename Type>
  friend class Ref;

  void IntrusiveAddRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<RefCounter>(-1), "Ref counter invalid");

    counter_.fetch_add(1, std::memory_order_relaxed);
  }

  bool IntrusiveDropRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<RefCounter>(-1), "Ref counter invalid");

    return counter_.fetch_sub(1, std::memory_order_acq_rel) == 0;
  }

  bool IntrusiveUniqueRef() const volatile noexcept
  {
    return counter_.load(std::memory_order_relaxed) == 0;
  }

  size_t IntrusiveRefCount() const volatile noexcept
  {
    return counter_.load(std::memory_order_relaxed) + 1;
  }

private:
  mutable std::atomic<RefCounter> counter_;
};

class RefCounted
{
public:
  constexpr RefCounted() noexcept : counter_(0)
  {
    LOCAL_THREAD_ASSERT;
  }

  constexpr RefCounted(const RefCounted&) noexcept : RefCounted() {}

  constexpr RefCounted& operator=(const RefCounted&) noexcept
  {
    LOCAL_THREAD_ASSERT;
    return *this;
  }

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  constexpr ~RefCounted() noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ == static_cast<RefCounter>(-1), "Ref counter invalid");
  }
#endif

private:
  template<typename Type>
  friend class Ref;

  constexpr void IntrusiveAddRef() const noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ < static_cast<RefCounter>(-1), "Ref counter invalid");

    ++counter_;
  }

  constexpr bool IntrusiveDropRef() const noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ < static_cast<RefCounter>(-1), "Ref counter invalid");

    return counter_-- == 0;
  }

  constexpr bool IntrusiveUniqueRef() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return counter_ == 0;
  }

  constexpr size_t IntrusiveRefCount() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return counter_ + 1;
  }

private:
  mutable RefCounter counter_;

  LOCAL_THREAD_VALIDATOR_INIT;
};

template<typename Type>
class Ref
{
public:
  constexpr Ref() noexcept : ptr_(nullptr), counter_(nullptr)
  {
    LOCAL_THREAD_ASSERT;
  }

  constexpr Ref(Type* instance) noexcept : ptr_(instance), counter_(nullptr)
  {
    LOCAL_THREAD_ASSERT;
  }

  ~Ref()
  {
    LOCAL_THREAD_ASSERT;

    auto ptr = ptr_; // Make sure ptr is in register

    if (!ptr) return;

    if (!counter_) // Never referenced
    {
PtrDelete: // Reduces code size
      delete ptr;
    }
    else if (*counter_ == 0) // Counter shifted down to test against zero.
    {
      delete counter_;
      goto PtrDelete;
    }
    else [[likely]] // When referenced at least once, enters this case at least 50% of the time
    {
      // Decrement has its own case to avoid indirect write when deleting.
      --*counter_;
    }
  }

  constexpr Ref(Ref&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_)
  {
    LOCAL_THREAD_ASSERT;

    other.ptr_ = nullptr;
    // Let counter dangle
  }

  Ref(const Ref& other) : ptr_(other.ptr_), counter_(other.counter_)
  {
    LOCAL_THREAD_ASSERT;

    if (!ptr_) return;

    if (!counter_)
    {
      // Lazy ref counter allocation. Saves on allocation when never referenced.

      counter_ = other.counter_ = new RefCounter(1);
    }
    else
    {
      ++*counter_;
    }
  }

  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    if (ptr_) [[likely]]
    {
      // Counter is shifted down so 0 is 1

      return counter_ ? *counter_ + 1 : 1;
    }

    return 0;
  }

  [[nodiscard]] constexpr bool Unique() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_ && (!counter_ || *counter_ == 0);
  }

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return *ptr_;
  }

  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

  [[nodiscard]] constexpr Type* get() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

private:
  using RefCounter = uint_fast32_t;

  Type* ptr_;
  mutable RefCounter* counter_;

  LOCAL_THREAD_VALIDATOR_INIT;
};

template<IntrusiveRefType Type>
class Ref<Type>
{
public:
  constexpr Ref() noexcept : ptr_(nullptr) {}

  constexpr Ref(Type* instance) noexcept : ptr_(instance)
  {
    ASSERT(instance->IntrusiveUniqueRef(), "Instance already referenced, possible double delete");
  }

  ~Ref()
  {
    const auto ptr = ptr_;

    if (ptr && ptr->IntrusiveDropRef()) [[unlikely]]
    {
      //

      delete ptr;
    }
  }

  constexpr Ref(Ref&& other) noexcept : ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  Ref(const Ref& other) : ptr_(other.ptr_)
  {
    if (!ptr_) return;

    ptr_->IntrusiveAddRef();
  }

  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    return ptr_ ? ptr_->IntrusiveRefCount() : 0;
  }

  [[nodiscard]] constexpr bool Unique() const noexcept
  {
    return ptr_ && ptr_->IntrusiveUniqueRef();
  }

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    return *ptr_;
  }

  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr Type* get() const noexcept
  {
    return ptr_;
  }

private:
  Type* ptr_;
};

} // namespace genebits::engine

#endif
