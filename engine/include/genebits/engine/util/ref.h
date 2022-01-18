#ifndef GENEBITS_ENGINE_UTIL_REF_H
#define GENEBITS_ENGINE_UTIL_REF_H

#include <atomic>
#include <concepts>
#include <cstdint>
#include <thread>
#include <type_traits>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/thread_validator.h"

namespace genebits::engine
{
// SharedPtr Overhead:
// - Compulsive atomic ref counting
// - Custom deleters
// - Weak pointer support
// - No support for intrusive reference counting

///
/// Default reference counter.
///
/// Chooses the fastest unsigned integer that is at least 16 bits.
///
/// We do not need a lot of bits for reference counting. Most ref counts de no exceed 64. Most common are 1-3.
/// R. Shahriyar. Down for the Count? https://dl.acm.org/doi/pdf/10.1145/2258996.2259008
///
using FastRefCounter = uint_fast16_t;

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

template<typename Type>
inline void RefDefaultDeleter(Type* instance)
{
  delete instance;
}

class AtomicRefCounted
{
public:
  constexpr AtomicRefCounted() noexcept : counter_(0) {}

  constexpr AtomicRefCounted(const AtomicRefCounted&) noexcept : AtomicRefCounted() {}

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  constexpr ~AtomicRefCounted() noexcept
  {
    if (!std::is_constant_evaluated())
    {
      ASSERT(counter_.load(std::memory_order_relaxed) == static_cast<FastRefCounter>(-1), "Ref counter invalid");
    }
  }
#endif

  AtomicRefCounted& operator=(const AtomicRefCounted&) noexcept
  {
    return *this;
  }

private:
  template<typename Type, void (*)(Type*)>
  friend class Ref;

  void IntrusiveAddRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    counter_.fetch_add(1, std::memory_order_relaxed);
  }

  bool IntrusiveDropRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<FastRefCounter>(-1), "Ref counter invalid");

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
  mutable std::atomic<FastRefCounter> counter_;
};

class RefCounted
{
public:
  constexpr RefCounted() noexcept : counter_(0)
  {
    LOCAL_THREAD_ASSERT;
  }

  constexpr RefCounted(const RefCounted&) noexcept : RefCounted() {}

#ifndef NDEBUG
  ///
  /// Destructor.
  ///
  constexpr ~RefCounted() noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ == static_cast<FastRefCounter>(-1), "Ref counter invalid");
  }
#endif

  constexpr RefCounted& operator=(const RefCounted&) noexcept
  {
    LOCAL_THREAD_ASSERT;
    return *this;
  }

private:
  template<typename Type, void (*)(Type*)>
  friend class Ref;

  constexpr void IntrusiveAddRef() const noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    ++counter_;
  }

  constexpr bool IntrusiveDropRef() const noexcept
  {
    LOCAL_THREAD_ASSERT;
    ASSERT(counter_ < static_cast<FastRefCounter>(-1), "Ref counter invalid");

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
  mutable FastRefCounter counter_;

  LOCAL_THREAD_VALIDATOR_INIT;
};

template<typename Type, void (*Deleter)(Type*) = RefDefaultDeleter<Type>>
class Ref
{
public:
  constexpr Ref() noexcept : ptr_(nullptr), counter_(nullptr)
  {
    LOCAL_THREAD_ASSERT;
  }

  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

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
      Deleter(ptr);
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

  constexpr Ref(Ref<Type, Deleter>&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_)
  {
    LOCAL_THREAD_ASSERT;

    other.ptr_ = nullptr;
    // Let counter dangle
  }

  Ref(const Ref<Type, Deleter>& other) : ptr_(other.ptr_), counter_(other.counter_)
  {
    LOCAL_THREAD_ASSERT;

    if (!ptr_) [[unlikely]]
      return;

    AddRef(other);
  }

  constexpr Ref<Type, Deleter>& operator=(Ref<Type, Deleter>&& other) noexcept
  {
    LOCAL_THREAD_ASSERT;

    if (other.ptr_ != ptr_) [[likely]]
    {
      ptr_ = other.ptr_;
      counter_ = other.counter_;

      other.ptr_ = nullptr;
    }

    return *this;
  }

  Ref<Type, Deleter>& operator=(const Ref<Type, Deleter>& other) noexcept
  {
    LOCAL_THREAD_ASSERT;

    if (ptr_ != other.ptr_) [[likely]]
    {
      ptr_ = other.ptr_;

      if (ptr_) [[likely]]
        AddRef(other);
    }

    return *this;
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

  [[nodiscard]] constexpr operator Type*() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

  [[nodiscard]] constexpr Type* Get() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    LOCAL_THREAD_ASSERT;

    return ptr_;
  }

  [[nodiscard]] constexpr bool operator!=(std::nullptr_t) noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) noexcept
  {
    return !ptr_;
  }

private:
  using RefCounter = uint_fast32_t;

  void AddRef(const Ref<Type, Deleter>& other)
  {
    if (!counter_) [[likely]] // 1 references is more likely than 2+ references
    {
      // Lazy ref counter allocation. Saves on allocation when never referenced.
      counter_ = other.counter_ = new RefCounter(1);
    }
    else
    {
      ++*counter_;
    }
  }

private:
  Type* ptr_;
  mutable RefCounter* counter_;

  LOCAL_THREAD_VALIDATOR_INIT;
};

template<IntrusiveRefType Type, void (*Deleter)(Type*)>
class Ref<Type, Deleter>
{
public:
  constexpr Ref() noexcept : ptr_(nullptr) {}

  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

  constexpr Ref(Type* instance) noexcept : ptr_(instance)
  {
    ASSERT(instance->IntrusiveUniqueRef(), "Instance already referenced, possible double delete");
  }

  ~Ref()
  {
    const auto ptr = ptr_;

    if (ptr && ptr->IntrusiveDropRef()) [[unlikely]]
      Deleter(ptr);
  }

  constexpr Ref(Ref<Type, Deleter>&& other) noexcept : ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  Ref(const Ref<Type, Deleter>& other) : ptr_(other.ptr_)
  {
    if (!ptr_) return;

    ptr_->IntrusiveAddRef();
  }

  Ref<Type, Deleter>& operator=(Ref<Type, Deleter>&& other)
  {
    if (ptr_ != other.ptr_) [[likely]]
    {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
  }

  Ref<Type, Deleter>& operator=(const Ref<Type, Deleter>& other)
  {
    if (ptr_ != other.ptr_) [[likely]]
    {
      ptr_ = other.ptr_;

      if (ptr_) [[likely]]
        ptr_->IntrusiveAddRef();
    }

    return *this;
  }

  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    return ptr_ ? ptr_->IntrusiveRefCount() : 0;
  }

  [[nodiscard]] constexpr bool Unique() const noexcept
  {
    return ptr_ && ptr_->IntrusiveUniqueRef();
  }

  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    return *ptr_;
  }

  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr operator Type*() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr Type* Get() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr bool operator!=(std::nullptr_t) noexcept
  {
    return ptr_;
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) noexcept
  {
    return !ptr_;
  }

private:
  Type* ptr_;
};

template<typename T, typename U, void (*TDeleter)(T*), void (*UDeleter)(U*)>
[[nodiscard]] constexpr bool operator==(const Ref<T, TDeleter>& lhs, const Ref<U, UDeleter>& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

template<typename T, typename U, void (*TDeleter)(T*), void (*UDeleter)(U*)>
[[nodiscard]] constexpr bool operator!=(const Ref<T, TDeleter>& lhs, const Ref<U, UDeleter>& rhs) noexcept
{
  return lhs.Get() != rhs.Get();
}

// TODO casting ?
// Casting from intrusive to non-intrusive is probably not doable

template<typename Type, typename... Args>
Ref<Type> MakeRef(Args&&... args)
{
  return Ref<Type>(new Type(std::forward<Args>(args)...));
}

} // namespace genebits::engine

#endif
