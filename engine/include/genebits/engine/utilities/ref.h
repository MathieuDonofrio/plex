#ifndef GENEBITS_ENGINE_UTILITIES_REF_H
#define GENEBITS_ENGINE_UTILITIES_REF_H

#include <atomic>
#include <compare>
#include <concepts>
#include <cstdint>
#include <thread>
#include <type_traits>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/thread_validator.h"
#include "genebits/engine/utilities/type_traits.h"

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
/// Concept used to determine whether or not two references can be implicitly casted.
///
/// @tparam From Type casted from.
/// @tparam To Type to cast to.
///
template<typename From, typename To>
concept AssignableRef = std::is_base_of_v<To, From> &&(IntrusiveRefType<To> == IntrusiveRefType<From>);

///
/// Correctly defines methods for an atomic intrusive ref.
///
/// The provided atomic counter will be used to define methods.
///
#define ATOMIC_INTRUSIVE_REF_METHODS(_counter)                                                        \
  void IntrusiveAddRef() const volatile noexcept                                                      \
  {                                                                                                   \
    ASSERT((_counter).load(std::memory_order_relaxed) < static_cast<decltype((counter_).load())>(-1), \
      "Ref counter invalid");                                                                         \
    (_counter).fetch_add(1, std::memory_order_relaxed);                                               \
  }                                                                                                   \
                                                                                                      \
  bool IntrusiveDropRef() const volatile noexcept                                                     \
  {                                                                                                   \
    ASSERT((_counter).load(std::memory_order_relaxed) < static_cast<decltype((counter_).load())>(-1), \
      "Ref counter invalid");                                                                         \
    return (_counter).fetch_sub(1, std::memory_order_acq_rel) == 0;                                   \
  }                                                                                                   \
                                                                                                      \
  bool IntrusiveUniqueRef() const volatile noexcept                                                   \
  {                                                                                                   \
    return (_counter).load(std::memory_order_relaxed) == 0;                                           \
  }                                                                                                   \
                                                                                                      \
  size_t IntrusiveRefCount() const volatile noexcept                                                  \
  {                                                                                                   \
    return (_counter).load(std::memory_order_relaxed) + 1;                                            \
  }

///
/// Correctly defines methods for an intrusive ref.
///
/// The provided counter will be used to define methods.
///
#define INTRUSIVE_REF_METHODS(_counter, _context)                                    \
  constexpr void IntrusiveAddRef() const noexcept                                    \
  {                                                                                  \
    LOCAL_THREAD_ASSERT(_context);                                                   \
    ASSERT((_counter) < static_cast<decltype(_counter)>(-1), "Ref counter invalid"); \
    ++(_counter);                                                                    \
  }                                                                                  \
                                                                                     \
  constexpr bool IntrusiveDropRef() const noexcept                                   \
  {                                                                                  \
    LOCAL_THREAD_ASSERT(_context);                                                   \
    ASSERT((_counter) < static_cast<decltype(_counter)>(-1), "Ref counter invalid"); \
    return (_counter)-- == 0;                                                        \
  }                                                                                  \
                                                                                     \
  constexpr bool IntrusiveUniqueRef() const noexcept                                 \
  {                                                                                  \
    LOCAL_THREAD_ASSERT(_context);                                                   \
    return (_counter) == 0;                                                          \
  }                                                                                  \
                                                                                     \
  constexpr size_t IntrusiveRefCount() const noexcept                                \
  {                                                                                  \
    LOCAL_THREAD_ASSERT(_context);                                                   \
    return (_counter) + 1;                                                           \
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
  template<typename Type>
  friend class Ref;

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

  ATOMIC_INTRUSIVE_REF_METHODS(counter_);

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
  template<typename Type>
  friend class Ref;

  ///
  /// Default constructor.
  ///
  constexpr RefCounted() noexcept : counter_(0) {}

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
    return *this;
  }

  INTRUSIVE_REF_METHODS(counter_, this);

private:
  mutable FastRefCounter counter_;

  LOCAL_THREAD_DECLARE;
};

namespace details
{
  ///
  /// Holds shared information about the reference.
  ///
  /// This block is shared between all ref's that manage the same instance.
  ///
  struct RefControlBlock
  {
    FastRefCounter counter;
    void (*deleter)(void*, RefControlBlock*);

    // Validator initialized here for Ref to have same size in debug and release.
    LOCAL_THREAD_DECLARE;
  };

} // namespace details

///
/// Reference.
///
/// Smart pointer that retains shared ownership of an instance through a pointer. Several ref's may
/// own the same instance. The held instance is destroyed when the last ref owning the instance is destroyed.
///
/// This is essentially our replacement for std::shared_ptr however there is no support for weak pointers and its not
/// thread safe since we dont atomically increment the ref count. See the intrusive specialization if you want to
/// customize the behaviour of the reference, for instance, making the ref count atomic.
///
/// @tparam Type Type to hold instance of.
/// @tparam Deleter Deleter used to delete the instance.
///
template<typename Type>
class Ref
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Ref() noexcept : ptr_(nullptr), control_(nullptr) {}

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance and a custom deleter.
  ///
  /// @tparam T The type to store, must be same or derived from ref type.
  ///
  /// @param[in] instance The instance to construct the reference with.
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  template<typename T, typename Deleter>
  requires(std::is_base_of_v<Type, T>) Ref(T* instance, Deleter&& deleter)
  noexcept : ptr_(static_cast<Type*>(instance))
  {
    struct RefControlBlockWithDeleter : public details::RefControlBlock
    {
      Deleter custom_deleter;
    };

    control_ = static_cast<details::RefControlBlock*>(::operator new(sizeof(RefControlBlockWithDeleter)));
    control_->counter = 0;
    control_->deleter = [](void* ptr, details::RefControlBlock* control)
    {
      static_cast<RefControlBlockWithDeleter*>(control)->custom_deleter(static_cast<T*>(ptr));
      ::operator delete(control, sizeof(RefControlBlockWithDeleter));
    };

    new (control_ + 1) Deleter(std::move(deleter));

    LOCAL_THREAD_INIT(control_);
  }

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance.
  ///
  /// @tparam T The type to store, must be same or derived from ref type.
  ///
  /// @param[in] instance The instance to construct the reference with.
  ///
  template<typename T>
  requires(std::is_base_of_v<Type, T>) explicit Ref(T* instance) noexcept : ptr_(static_cast<Type*>(instance))
  {
    control_ = static_cast<details::RefControlBlock*>(::operator new(sizeof(details::RefControlBlock)));
    control_->counter = 0;

    control_->deleter = [](void* ptr, details::RefControlBlock* control)
    {
      delete static_cast<T*>(ptr);
      ::operator delete(control, sizeof(details::RefControlBlock));
    };

    LOCAL_THREAD_INIT(control_);
  }

  ///
  /// Nullptr constructor with custom deleter
  ///
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  template<typename Deleter>
  Ref(std::nullptr_t, Deleter&& deleter) noexcept : Ref(static_cast<Type*>(nullptr), std::forward<Deleter>(deleter))
  {}

  ///
  /// Nullptr constructor.
  ///
  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

  ///
  /// Destructor.
  ///
  ~Ref()
  {
    auto ptr = ptr_;

    if (!ptr) return;

    auto control = control_;

    LOCAL_THREAD_ASSERT(control_);

    if (control->counter == 0) // Counter shifted down to test against zero.
    {
      control->deleter(ptr, control);
    }
    else
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
  constexpr Ref(Ref<Type>&& other) noexcept : ptr_(other.ptr_), control_(other.control_)
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    other.ptr_ = nullptr;
    // Let control dangle
  }

  ///
  /// Move constructor.
  ///
  /// @tparam T Other type, must be same or derived from ref type and not intrusive.
  ///
  /// @param[in] other Reference to move.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref(Ref<T>&& other) noexcept : ptr_(static_cast<Type*>(other.ptr_)), control_(other.control_)
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    other.ptr_ = nullptr;
    // Let control dangle
  }

  ///
  /// Copy Constructor.
  ///
  /// @param other Reference to copy
  ///
  constexpr Ref(const Ref<Type>& other) : ptr_(other.ptr_), control_(other.control_)
  {
    if (ptr_) [[likely]]
    {
      LOCAL_THREAD_ASSERT(control_);

      ++control_->counter;
    }
  }

  ///
  /// Copy Constructor.
  ///
  /// @tparam T Other type, must be same or derived from ref type and not intrusive.
  ///
  /// @param other Reference to copy
  ///
  template<AssignableRef<Type> T>
  constexpr Ref(const Ref<T>& other) : ptr_(static_cast<Type*>(other.ptr_)), control_(other.control_)
  {
    if (ptr_) [[likely]]
    {
      LOCAL_THREAD_ASSERT(control_);

      ++control_->counter;
    }
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  constexpr Ref<Type>& operator=(Ref<Type>&& other)
  {
    Ref<Type>(std::move(other)).Swap(*this);
    return *this;
  }

  ///
  /// Move assignment operator.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref<Type>& operator=(Ref<T>&& other)
  {
    Ref<Type>(std::move(other)).Swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
  constexpr Ref<Type>& operator=(const Ref<Type>& other)
  {
    Ref<Type>(other).Swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref<Type>& operator=(const Ref<T>& other)
  {
    Ref<Type>(other).Swap(*this);
    return *this;
  }

  ///
  /// Exchanges the stored pointer values and ownerships.
  ///
  constexpr void Swap(Ref<Type>& other) noexcept
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));
    LOCAL_THREAD_ASSERT((other.ptr_ ? other.control_ : nullptr));

    std::swap(ptr_, other.ptr_);
    std::swap(control_, other.control_);
  }

  ///
  /// Returns the amount of ref's referring to the same managed instance.
  ///
  /// @return Amount of references.
  ///
  [[nodiscard]] constexpr size_t UseCount() const noexcept
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    if (ptr_) [[likely]]
    {
      // Counter is shifted down so 0 is 1
      return control_->counter + 1;
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
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    return ptr_ && control_->counter == 0;
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Reference to ptr value.
  ///
  [[nodiscard]] constexpr Type& operator*() const noexcept
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    return *ptr_;
  }

  ///
  /// Dereferences the stored ptr.
  ///
  /// @return Pointer to managed instance.
  ///
  [[nodiscard]] constexpr Type* operator->() const noexcept
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

    return ptr_;
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr Type* Get() const noexcept
  {
    LOCAL_THREAD_ASSERT((ptr_ ? control_ : nullptr));

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
  template<typename T>
  friend class Ref;

  template<typename T, typename... Args>
  friend Ref<T> MakeRef(Args&&... args);

  Type* ptr_;
  mutable details::RefControlBlock* control_;
};

///
/// Reference.
///
/// Smart pointer that retains shared ownership of an instance through a pointer. Several ref's may
/// own the same instance. The held instance is destroyed when the last ref owning the instance is destroyed.
///
/// Specialization for intrusive ref types. Will use the intrusive methods from the instance to handle control.
/// Most of the time using this specialization is slightly faster than using the general purpose ref.
///
/// @tparam Type Type to hold instance of.
/// @tparam Deleter Deleter used to delete the instance.
///
template<IntrusiveRefType Type>
class Ref<Type>
{
public:
  ///
  /// Default constructor.
  ///
  constexpr Ref() noexcept : ptr_(nullptr), deleter_(nullptr) {}

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance and a custom deleter
  ///
  /// @tparam T The type to store, must be same or derived from ref type.
  ///
  /// @param[in] instance The instance to construct the reference with.
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  template<typename T>
  requires(std::is_base_of_v<Type, T>) constexpr Ref(T* instance, void (*deleter)(void*)) noexcept
    : ptr_(static_cast<Type*>(instance)), deleter_(deleter)
  {
#ifndef NDEBUG
    if (!std::is_constant_evaluated())
    {
      ASSERT(!instance || instance->IntrusiveUniqueRef(), "Instance already referenced, possible double delete");
    }
#endif
  }

  ///
  /// Constructor.
  ///
  /// Constructs the reference with a managed instance
  ///
  /// @tparam T The type to store, must be same or derived from ref type.
  ///
  /// @param[in] instance The instance to construct the reference with.
  ///
  template<typename T>
  constexpr explicit Ref(T* instance) noexcept : Ref(instance, [](void* ptr) { delete static_cast<T*>(ptr); })
  {}

  ///
  /// Nullptr constructor with custom deleter
  ///
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  constexpr Ref(std::nullptr_t, void (*deleter)(void*)) noexcept : Ref(static_cast<Type*>(nullptr), deleter) {}

  ///
  /// Nullptr constructor.
  ///
  constexpr Ref(std::nullptr_t) noexcept : Ref() {}

  ///
  /// Destructor
  ///
  ~Ref()
  {
    const auto ptr = ptr_;

    if (ptr && ptr->IntrusiveDropRef()) [[unlikely]]
      deleter_(ptr);
  }

  ///
  /// Move constructor.
  ///
  /// @param[in] other Reference to move.
  ///
  constexpr Ref(Ref<Type>&& other) noexcept : ptr_(other.ptr_), deleter_(other.deleter_)
  {
    other.ptr_ = nullptr;
    // Let deleter dangle
  }

  ///
  /// Move constructor.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param[in] other Reference to move.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref(Ref<T>&& other) noexcept : ptr_(static_cast<Type*>(other.ptr_)), deleter_(other.deleter_)
  {
    other.ptr_ = nullptr;
    // Let deleter dangle
  }

  ///
  /// Copy Constructor.
  ///
  /// @param other Reference to copy
  ///
  constexpr Ref(const Ref<Type>& other) : ptr_(other.ptr_), deleter_(other.deleter_)
  {
    if (ptr_) ptr_->IntrusiveAddRef();
  }

  ///
  /// Copy Constructor.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param other Reference to copy
  ///
  template<AssignableRef<Type> T>
  constexpr Ref(const Ref<T>& other) : ptr_(static_cast<Type*>(other.ptr_)), deleter_(other.deleter_)
  {
    if (ptr_) ptr_->IntrusiveAddRef();
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  constexpr Ref<Type>& operator=(Ref<Type>&& other) noexcept
  {
    Ref<Type>(std::move(other)).Swap(*this);
    return *this;
  }

  ///
  /// Move assignment operator.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param[in] other Reference to move.
  ///
  /// @return This reference.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref<Type>& operator=(Ref<T>&& other) noexcept
  {
    Ref<Type>(std::move(other)).Swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
  constexpr Ref<Type>& operator=(const Ref<Type>& other)
  {
    Ref<Type>(other).Swap(*this);
    return *this;
  }

  ///
  /// Copy assignment operator.
  ///
  /// @tparam T Other type, must be same or derived from ref type and intrusive.
  ///
  /// @param[in] other Reference to copy
  ///
  /// @return This reference.
  ///
  template<AssignableRef<Type> T>
  constexpr Ref<Type>& operator=(const Ref<T>& other)
  {
    Ref<Type>(other).Swap(*this);
    return *this;
  }

  ///
  /// Exchanges the stored pointer values and ownerships.
  ///
  constexpr void Swap(Ref<Type>& other) noexcept
  {
    std::swap(ptr_, other.ptr_);
    std::swap(deleter_, other.deleter_);
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
  template<typename T>
  friend class Ref;

  Type* ptr_;
  void (*deleter_)(void*);
};

///
/// Equality Operator.
///
/// @tparam T Type of lhs reference
/// @tparam U Type of rhs reference
///
/// @param[in] lhs Lhs reference to compare.
/// @param[in] rhs Rhs reference to compare.
///
/// @return True if both ref's are equal, false otherwise.
///
template<typename T, typename U>
constexpr bool operator==(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

///
/// Three Way Compare Operator.
///
/// @tparam T Type of lhs reference
/// @tparam U Type of rhs reference
///
/// @param[in] lhs Lhs reference to compare.
/// @param[in] rhs Rhs reference to compare.
///
/// @return Strong ordering.
///
template<typename T, typename U>
constexpr std::strong_ordering operator<=>(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
{
  return std::compare_three_way()(lhs.Get(), rhs.Get());
}

///
/// Equality operator. Compare against nullptr.
///
/// @tparam T Type of lhs reference
///
/// @param lhs Lhs reference to compare.
///
/// @return True ref's managed pointer is equal to nullptr.
///
template<typename T>
constexpr bool operator==(const Ref<T>& lhs, std::nullptr_t) noexcept
{
  return !lhs;
}

///
/// Three Way Compare Operator. Compare against nullptr.
///
/// @tparam T Type of lhs reference
///
/// @param[in] lhs Lhs reference to compare.
///
/// @return Strong ordering.
///
template<typename T>
constexpr std::strong_ordering operator<=>(const Ref<T>& lhs, std::nullptr_t) noexcept
{
  return std::compare_three_way()(lhs.Get(), static_cast<T*>(nullptr));
}

///
/// Factory method for ref.
///
/// Creates an instance of Type and wraps it in a reference using the args
/// as the parameter list of the constructor.
///
/// Recommended to always use MakeRef for optimal construction.
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
  if constexpr (!IntrusiveRefType<Type>)
  {
    struct RefControlBlockAndType : public details::RefControlBlock
    {
      Type instance;
    };

    Ref<Type> ref;
    ref.control_ = static_cast<details::RefControlBlock*>(::operator new(sizeof(RefControlBlockAndType)));
    ref.ptr_ = reinterpret_cast<Type*>(ref.control_ + 1);

    new (ref.ptr_) Type(std::forward<Args>(args)...);

    ref.control_->counter = 0;
    ref.control_->deleter = [](void* ptr, details::RefControlBlock* control)
    {
      static_cast<Type*>(ptr)->~Type();
      ::operator delete(control, sizeof(RefControlBlockAndType));
    };

    LOCAL_THREAD_INIT(ref.control_);

    return ref;
  }
  else
  {
    return Ref<Type>(new Type(std::forward<Args>(args)...));
  }
}

template<typename Type>
struct IsTriviallyRelocatable<Ref<Type>> : std::true_type
{};
} // namespace genebits::engine

namespace std
{
template<typename T>
constexpr void swap(genebits::engine::Ref<T>& lhs, genebits::engine::Ref<T>& rhs) noexcept
{
  lhs.Swap(rhs);
}

template<typename T>
struct hash<genebits::engine::Ref<T>>
{
  constexpr size_t operator()(const genebits::engine::Ref<T>& obj) const noexcept
  {
    return std::hash<T*>()(obj.Get());
  }
};
} // namespace std

#endif
