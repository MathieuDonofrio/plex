#ifndef GENEBITS_ENGINE_UTIL_REF_H
#define GENEBITS_ENGINE_UTIL_REF_H

#include <atomic>
#include <compare>
#include <concepts>
#include <cstdint>
#include <thread>
#include <type_traits>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/thread_validator.h"

namespace genebits::engine
{
///
/// Default reference counter.
///
/// Chooses the fastest unsigned integer that is at least 16 bits.
///
/// We do not need a lot of bits for reference counting. Most ref counts de no exceed 64. Most common are 1-3.
/// R. Shahriyar. Down for the Count? https://dl.acm.org/doi/pdf/10.1145/2258996.2259008
///
using FastRefCounter = uint_fast16_t;

///
/// Concept used to determine whether or not a type can be intrusively referenced.
///
/// Intrusive referencing allows for better performance in most cases as well as more flexibility.
///
/// @tparam Type The type to be referenced.
///
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

///
/// Default deleter for references, simply calls delete.
///
/// @tparam Type Type of the reference.
///
/// @param[in] instance Instance to be deleted.
///
template<typename Type>
inline void RefDefaultDeleter(Type* instance)
{
  delete instance;
}

///
/// Base class for atomic intrusively referenced types. References created on AtomicRefCounted type will be intrusive.
///
/// Using atomic ref counting is thread safe and works in a similar way as std::shared_ptr. This comes at the cost
/// of some performance penalties in some cases.
///
class AtomicRefCounted
{
public:
  ///
  /// Default constructor.
  ///
  constexpr AtomicRefCounted() noexcept : counter_(0) {}

  ///
  /// Copy constructor.
  ///
  constexpr AtomicRefCounted(const AtomicRefCounted&) noexcept : AtomicRefCounted() {}

  ///
  /// Copy assignment.
  ///
  /// @return this.
  ///
  AtomicRefCounted& operator=(const AtomicRefCounted&) noexcept
  {
    return *this;
  }

  ///
  /// Adds a reference.
  ///
  void IntrusiveAddRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    counter_.fetch_add(1, std::memory_order_relaxed);
  }

  ///
  /// Drops a reference and returns whether or not there are no more references.
  ///
  /// @return True if there are no more references, false otherwise.
  ///
  bool IntrusiveDropRef() const volatile noexcept
  {
    ASSERT(counter_.load(std::memory_order_relaxed) < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    return counter_.fetch_sub(1, std::memory_order_acq_rel) == 0;
  }

  ///
  /// Whether or not there is only 1 reference.
  ///
  /// @return True if there is only 1 reference, false otherwise.
  ///
  bool IntrusiveUniqueRef() const volatile noexcept
  {
    return counter_.load(std::memory_order_relaxed) == 0;
  }

  ///
  /// Returns the amount of references.
  ///
  /// @return Amount of references.
  ///
  size_t IntrusiveRefCount() const volatile noexcept
  {
    return counter_.load(std::memory_order_relaxed) + 1;
  }

private:
  mutable std::atomic<FastRefCounter> counter_;
};

///
/// Base class for intrusively referenced types. References created on RefCounted type will be intrusive.
///
/// @warning Not thread safe.
///
class RefCounted
{
public:
  ///
  /// Default constructor.
  ///
  constexpr RefCounted() noexcept : counter_(0)
  {
    LOCAL_THREAD_ASSERT(this);
  }

  ///
  /// Copy constructor.
  ///
  constexpr RefCounted(const RefCounted&) noexcept : RefCounted() {}

  ///
  /// Copy assignment.
  ///
  /// @return this.
  ///
  constexpr RefCounted& operator=(const RefCounted&) noexcept
  {
    LOCAL_THREAD_ASSERT(this);
    return *this;
  }

  ///
  /// Adds a reference.
  ///
  constexpr void IntrusiveAddRef() const noexcept
  {
    LOCAL_THREAD_ASSERT(this);
    ASSERT(counter_ < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    ++counter_;
  }

  ///
  /// Drops a reference and returns whether or not there are no more references.
  ///
  /// @return True if there are no more references, false otherwise.
  ///
  constexpr bool IntrusiveDropRef() const noexcept
  {
    LOCAL_THREAD_ASSERT(this);
    ASSERT(counter_ < static_cast<FastRefCounter>(-1), "Ref counter invalid");

    return counter_-- == 0;
  }

  ///
  /// Whether or not there is only 1 reference.
  ///
  /// @return True if there is only 1 reference, false otherwise.
  ///
  constexpr bool IntrusiveUniqueRef() const noexcept
  {
    LOCAL_THREAD_ASSERT(this);

    return counter_ == 0;
  }

  ///
  /// Returns the amount of references.
  ///
  /// @return Amount of references.
  ///
  constexpr size_t IntrusiveRefCount() const noexcept
  {
    LOCAL_THREAD_ASSERT(this);

    return counter_ + 1;
  }

private:
  mutable FastRefCounter counter_;

  LOCAL_THREAD_VALIDATOR_INIT;
};

///
/// Reference.
///
/// Smart pointer that retains shared ownership of an instance through a pointer. Several ref's may
/// own the same instance. The held instance is destroyed when the last ref owning the instance is destroyed.
///
/// @tparam Type Type to hold instance of.
/// @tparam Deleter Deleter used to delete the instance.
///
template<typename Type, void (*Deleter)(Type*) = RefDefaultDeleter<Type>>
class Ref
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Ref() noexcept : ptr_(nullptr), control_(nullptr)
  {
    LOCAL_THREAD_ASSERT(control_);
  }

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance.
  ///
  /// @param[in] instance The instance to construct the reference with.
  ///
  explicit Ref(Type* instance) noexcept : ptr_(instance), control_(nullptr)
  {
    LOCAL_THREAD_ASSERT(control_);
  }

  ///
  /// Nullptr constructor. Delegates to default constructor.
  ///
  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

  ///
  /// Destructor.
  ///
  ~Ref()
  {
    LOCAL_THREAD_ASSERT(control_);

    auto ptr = ptr_; // Make sure ptr is in register

    if (!ptr) return;

    if (!control_) // Never referenced
    {
PtrDelete: // Reduces code size
      Deleter(ptr);
    }
    else if (control_->counter == 0) // Counter shifted down to test against zero.
    {
      delete control_;
      goto PtrDelete;
    }
    else [[likely]] // When referenced at least once, enters this case at least 50% of the time
    {
      // Decrement has its own case to avoid indirect write when deleting.
      --control_->counter;
    }
  }

  ///
  /// Move constructor.
  ///
  /// @param[in] other Reference to move.
  ///
  constexpr Ref(Ref<Type, Deleter>&& other) noexcept : ptr_(other.ptr_), control_(other.control_)
  {
    LOCAL_THREAD_ASSERT(control_);

    other.ptr_ = nullptr;
    // Let control dangle
  }

  ///
  /// Copy Constructor.
  ///
  /// @param other Reference to copy
  ///
  Ref(const Ref<Type, Deleter>& other) : ptr_(other.ptr_), control_(other.control_)
  {
    LOCAL_THREAD_ASSERT(control_);

    if (!ptr_) [[unlikely]]
      return;

    AddRef(other);
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  constexpr Ref<Type, Deleter>& operator=(Ref<Type, Deleter>&& other) noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    if (other.ptr_ != ptr_) [[likely]]
    {
      ptr_ = other.ptr_;
      control_ = other.control_;

      other.ptr_ = nullptr;
    }

    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
  Ref<Type, Deleter>& operator=(const Ref<Type, Deleter>& other) noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    if (ptr_ != other.ptr_) [[likely]]
    {
      ptr_ = other.ptr_;

      if (ptr_) [[likely]]
        AddRef(other);
    }

    return *this;
  }

  ///
  /// Exchanges the stored pointer values and ownerships.
  ///
  constexpr void Swap(Ref<Type, Deleter>& other) noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    void* tmp = other.ptr_;
    other.ptr_ = ptr_;
    ptr_ = static_cast<Type*>(tmp);
    tmp = other.control_;
    other.control_ = control_;
    control_ = static_cast<ControlBlock*>(tmp);
  }

  ///
  /// Returns the amount of ref's referring to the same managed instance.
  ///
  /// @return Amount of references.
  ///
  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    if (ptr_) [[likely]]
    {
      // Counter is shifted down so 0 is 1

      return control_ ? control_->counter + 1 : 1;
    }

    return 0;
  }

  ///
  /// Checks whether or not the managed instance is only managed by the current ref.
  ///
  /// @return True if this is the only ref, false otherwise.
  ///
  [[nodiscard]] constexpr bool Unique() const noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    return ptr_ && (!control_ || control_->counter == 0);
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Reference to ptr value.
  ///
  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    return *ptr_;
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Pointer to managed instance.
  ///
  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    return ptr_;
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr Type* Get() const noexcept
  {
    LOCAL_THREAD_ASSERT(control_);

    return ptr_;
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr operator Type*() const noexcept
  {
    return Get();
  }

  ///
  /// Returns whether or not the stored pointer is not null.
  ///
  /// @return True if managed instance is not null, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return static_cast<bool>(Get());
  }

private:
  ///
  /// Increments the internal reference count.
  ///
  /// @param other Reference that was added.
  ///
  void AddRef(const Ref<Type, Deleter>& other)
  {
    if (!control_) [[likely]] // 1 references is more likely than 2+ references
    {
      // Lazy ref counter allocation. Saves on allocation when never referenced.
      control_ = other.control_ = new ControlBlock;
      control_->counter = 1;
    }
    else
    {
      ++control_->counter;
    }
  }

private:
  ///
  /// Holds shared information about the reference.
  ///
  /// This block is shared between all ref's that manage the same instance.
  ///
  struct ControlBlock
  {
    FastRefCounter counter;

    // Validator initialized here for Ref to have same size in debug and release.
    LOCAL_THREAD_VALIDATOR_INIT;
  };

private:
  Type* ptr_;
  mutable ControlBlock* control_;
};

///
/// Reference.
///
/// Smart pointer that retains shared ownership of an instance through a pointer. Several ref's may
/// own the same instance. The held instance is destroyed when the last ref owning the instance is destroyed.
///
/// Specialization for intrusive ref types. Will use the intrusive methods from the instance to handle control.
/// Most of the time using this specialization is faster than using the general purpose ref.
///
/// @tparam Type Type to hold instance of.
/// @tparam Deleter Deleter used to delete the instance.
///
template<IntrusiveRefType Type, void (*Deleter)(Type*)>
class Ref<Type, Deleter>
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Ref() noexcept : ptr_(nullptr) {}

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance.
  ///
  /// @param[in] instance The instance to construct the reference with.
  ///
  explicit Ref(Type* instance) noexcept : ptr_(instance)
  {
    ASSERT(!instance || instance->IntrusiveUniqueRef(), "Instance already referenced, possible double delete");
  }

  ///
  /// Nullptr constructor. Delegates to default constructor.
  ///
  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

  ///
  /// Destructor
  ///
  ~Ref()
  {
    const auto ptr = ptr_;

    if (ptr && ptr->IntrusiveDropRef()) [[unlikely]]
      Deleter(ptr);
  }

  ///
  /// Move constructor.
  ///
  /// @param[in] other Reference to move.
  ///
  constexpr Ref(Ref<Type, Deleter>&& other) noexcept : ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  ///
  /// Copy Constructor.
  ///
  /// @param other Reference to copy
  ///
  Ref(const Ref<Type, Deleter>& other) : ptr_(other.ptr_)
  {
    if (!ptr_) return;

    ptr_->IntrusiveAddRef();
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  Ref<Type, Deleter>& operator=(Ref<Type, Deleter>&& other) noexcept
  {
    if (ptr_ != other.ptr_) [[likely]]
    {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }

    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
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

  ///
  /// Exchanges the stored pointer values and ownerships.
  ///
  constexpr void Swap(Ref<Type, Deleter>& other) noexcept
  {
    Type* tmp = other.ptr_;
    other.ptr_ = ptr_;
    ptr_ = tmp;
  }

  ///
  /// Returns the amount of ref's referring to the same managed instance.
  ///
  /// @return Amount of references.
  ///
  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    return ptr_ ? ptr_->IntrusiveRefCount() : 0;
  }

  ///
  /// Checks whether or not the managed instance is only managed by the current ref.
  ///
  /// @return True if this is the only ref, false otherwise.
  ///
  [[nodiscard]] constexpr bool Unique() const noexcept
  {
    return ptr_ && ptr_->IntrusiveUniqueRef();
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Reference to ptr value.
  ///
  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    return *ptr_;
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Pointer to managed instance.
  ///
  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    return ptr_;
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr operator Type*() const noexcept
  {
    return ptr_;
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr Type* Get() const noexcept
  {
    return ptr_;
  }

  ///
  /// Returns whether or not the stored pointer is not null.
  ///
  /// @return True if managed instance is not null, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return ptr_;
  }

private:
  Type* ptr_;
};

///
/// Equality Operator.
///
/// @tparam T Type of lhs reference
/// @tparam U Type of rhs reference
/// @tparam TDeleter Deleter type of lhs reference
/// @tparam UDeleter Deleter type of rhs reference
///
/// @param[in] lhs Lhs reference to compare.
/// @param[in] rhs Rhs reference to compare.
///
/// @return True if both ref's are equal, false otherwise.
///
template<typename T, typename U, void (*TDeleter)(T*), void (*UDeleter)(U*)>
constexpr bool operator==(const Ref<T, TDeleter>& lhs, const Ref<U, UDeleter>& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

///
/// Three War Compare Operator.
///
/// @tparam T Type of lhs reference
/// @tparam U Type of rhs reference
/// @tparam TDeleter Deleter type of lhs reference
/// @tparam UDeleter Deleter type of rhs reference
///
/// @param[in] lhs Lhs reference to compare.
/// @param[in] rhs Rhs reference to compare.
///
/// @return Strong ordering.
///
template<typename T, typename U, void (*TDeleter)(T*), void (*UDeleter)(U*)>
constexpr std::strong_ordering operator<=>(const Ref<T, TDeleter>& lhs, const Ref<U, UDeleter>& rhs) noexcept
{
  return std::compare_three_way()(lhs.Get(), rhs.Get());
}

///
/// Equality operator. Compare against nullptr.
///
/// @tparam T Type of lhs reference
/// @tparam TDeleter Deleter type of lhs reference
///
/// @param lhs Lhs reference to compare.
///
/// @return True ref's managed pointer is equal to nullptr.
///
template<typename T, void (*TDeleter)(T*)>
constexpr bool operator==(const Ref<T, TDeleter>& lhs, nullptr_t) noexcept
{
  return !lhs;
}

///
/// Three Way Compare Operator. Compare against nullptr.
///
/// @tparam T Type of lhs reference
/// @tparam TDeleter Deleter type of lhs reference
///
/// @param[in] lhs Lhs reference to compare.
///
/// @return Strong ordering.
///
template<typename T, void (*TDeleter)(T*)>
constexpr std::strong_ordering operator<=>(const Ref<T, TDeleter>& lhs, nullptr_t) noexcept
{
  return std::compare_three_way()(lhs.Get(), static_cast<T*>(nullptr));
}

///
/// Constructs an instance of Type and wraps it in a reference using the args
/// as the parameter list of the constructor.
///
/// @tparam Type Type of reference
/// @tparam Args Constructor argument types.
///
/// @param[in] args Arguments to invoke constructor with.
///
/// @return Ref of and instance of Type.
///
template<typename Type, typename... Args>
Ref<Type> MakeRef(Args&&... args)
{
  return Ref<Type, RefDefaultDeleter>(new Type(std::forward<Args>(args)...));
}

} // namespace genebits::engine

namespace std
{
template<typename T, void (*TDeleter)(T*)>
constexpr void swap(genebits::engine::Ref<T, TDeleter>& lhs, genebits::engine::Ref<T, TDeleter>& rhs) noexcept
{
  lhs.Swap(rhs);
}
} // namespace std

#endif
