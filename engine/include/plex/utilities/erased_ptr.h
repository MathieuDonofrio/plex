#ifndef PLEX_UTILITIES_ERASED_PTR_H
#define PLEX_UTILITIES_ERASED_PTR_H

#include "genebits/engine/utilities/type_traits.h"

namespace plex
{
///
/// Concept used to determine whether or not two erased pointers can be implicitly casted.
///
/// @tparam From Type casted from.
/// @tparam To Type to cast to.
///
template<typename From, typename To>
concept AssignableErasedPtr = std::is_void_v<To> || std::is_base_of_v<To, From>;

namespace details
{
  ///
  /// Base for erased ptr.
  ///
  /// @tparam Type Pointer type.
  ///
  template<typename Type>
  class ErasedPtrBase
  {
  public:
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

  protected:
    Type* ptr_;
  };

  ///
  /// Base for erased ptr.
  ///
  /// Specialization for void
  ///
  /// @tparam Type Pointer type.
  ///
  template<>
  class ErasedPtrBase<void>
  {
  protected:
    void* ptr_;
  };
} // namespace details

///
/// Type erasure pointer.
///
/// Stores an instance as a base type to lose the type information.
///
/// @tparam Type Pointer type.
///
template<typename Type>
class ErasedPtr : public details::ErasedPtrBase<Type>
{
public:
  ///
  /// Default Constructor
  ///
  constexpr ErasedPtr() noexcept : ptr_(nullptr), deleter_(nullptr) {}

  ///
  /// Parametric Constructor
  ///
  /// @param[in] instance Instance to handle.
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  constexpr ErasedPtr(Type* instance, void (*deleter)(void*)) noexcept : ptr_(instance), deleter_(deleter) {}

  ///
  /// Parametric Constructor
  ///
  /// @tparam T The type to store, must be same or derived from erased type base.
  ///
  /// @param[in] instance Instance to handle.
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  template<AssignableErasedPtr<Type> T>
  constexpr ErasedPtr(T* instance, void (*deleter)(void*)) noexcept : ptr_(instance), deleter_(deleter)
  {}

  ///
  /// Parametric Constructor. Uses default deleter.
  ///
  /// @tparam T The type to store, must be same or derived from erased type base.
  ///
  /// @param[in] instance Instance to handle.
  ///
  template<AssignableErasedPtr<Type> T>
  constexpr explicit ErasedPtr(T* instance) noexcept
    : ErasedPtr(instance, [](void* instance) { delete static_cast<T*>(instance); })
  {}

  ///
  /// Parametric Constructor. Uses default deleter.
  ///
  /// @param[in] instance Instance to handle.
  ///
  constexpr explicit ErasedPtr(Type* instance) noexcept
    : ErasedPtr(instance, [](void* instance) { delete static_cast<Type*>(instance); })
  {}

  ///
  /// Nullptr constructor with custom deleter
  ///
  /// @param[in] deleter The custom deleter to delete the managed instance with.
  ///
  constexpr ErasedPtr(std::nullptr_t, void (*deleter)(void*)) noexcept : ErasedPtr(nullptr, deleter) {}

  ///
  /// Nullptr constructor.
  ///
  constexpr ErasedPtr(std::nullptr_t) noexcept : ErasedPtr() {}

  ///
  /// Destructor
  ///
  ~ErasedPtr()
  {
    if (ptr_) deleter_(ptr_);
  }

  ErasedPtr(const ErasedPtr& other) = delete;
  ErasedPtr& operator=(const ErasedPtr& other) = delete;

  ///
  /// Move constructor.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  constexpr ErasedPtr(ErasedPtr<Type>&& other) noexcept : ErasedPtr(other.ptr_, other.deleter_)
  {
    other.ptr_ = nullptr;
    // Let deleter dangle
  }

  ///
  /// Move constructor.
  ///
  /// @tparam T Other type.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  template<AssignableErasedPtr<Type> T>
  constexpr ErasedPtr(ErasedPtr<T>&& other) noexcept : ErasedPtr(other.ptr_, other.deleter_)
  {
    other.ptr_ = nullptr;
    // Let deleter dangle
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  /// @return Erased ptr that was assigned.
  ///
  constexpr ErasedPtr& operator=(ErasedPtr<Type>&& other) noexcept
  {
    ErasedPtr<Type>(std::move(other)).swap(*this);
    return *this;
  }

  ///
  /// Move assignment operator.
  ///
  /// @tparam T Other type.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  /// @return Erased ptr that was assigned.
  ///
  template<AssignableErasedPtr<Type> T>
  constexpr ErasedPtr& operator=(ErasedPtr<T>&& other) noexcept
  {
    ErasedPtr<Type>(std::move(other)).swap(*this);
    return *this;
  }

  ///
  /// Exchanges the stored pointer values and ownerships.
  ///
  constexpr void swap(ErasedPtr<Type>& other) noexcept
  {
    std::swap(ptr_, other.ptr_);
    std::swap(deleter_, other.deleter_);
  }

  ///
  /// Returns the stored pointer.
  ///
  /// @return Stored pointer.
  ///
  [[nodiscard]] constexpr Type* get() const noexcept
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
    return get();
  }

  ///
  /// Returns whether or not the erased pointer contains a handled pointer.
  ///
  /// @return True is has handled pointer, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return ptr_;
  }

private:
  template<typename T>
  friend class ErasedPtr;

  template<typename T, typename... Args>
  friend ErasedPtr<T> MakeErased(Args&&... args);

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
/// @return True if both ptr's are equal, false otherwise.
///
template<typename T, typename U>
constexpr bool operator==(const ErasedPtr<T>& lhs, const ErasedPtr<U>& rhs) noexcept
{
  return lhs.get() == rhs.get();
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
constexpr std::strong_ordering operator<=>(const ErasedPtr<T>& lhs, const ErasedPtr<U>& rhs) noexcept
{
  return std::compare_three_way()(lhs.get(), rhs.get());
}

///
/// Equality operator. Compare against nullptr.
///
/// @tparam T Type of lhs reference
///
/// @param lhs Lhs reference to compare.
///
/// @return True erased ptr's managed pointer is equal to nullptr.
///
template<typename T>
constexpr bool operator==(const ErasedPtr<T>& lhs, std::nullptr_t) noexcept
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
constexpr std::strong_ordering operator<=>(const ErasedPtr<T>& lhs, std::nullptr_t) noexcept
{
  return std::compare_three_way()(lhs.get(), static_cast<T*>(nullptr));
}

///
/// Factory method for erased ptr.
///
/// Creates an instance of Type and wraps it in a erased ptr using the args
/// as the parameter list of the constructor.
///
/// @tparam Type Type of reference
/// @tparam Args Constructor argument types.
///
/// @param[in] args Arguments to invoke constructor with.
///
/// @return ErasedPtr of type.
///
template<typename Type, typename... Args>
ErasedPtr<Type> MakeErased(Args&&... args)
{
  return ErasedPtr<Type>(new Type(std::forward<Args>(args)...));
}

template<typename Type>
struct IsTriviallyRelocatable<ErasedPtr<Type>> : public std::true_type
{};
} // namespace plex

namespace std
{
template<typename T>
constexpr void swap(plex::ErasedPtr<T>& lhs, plex::ErasedPtr<T>& rhs) noexcept
{
  lhs.swap(rhs);
}

template<typename T>
struct hash<plex::ErasedPtr<T>>
{
  constexpr size_t operator()(const plex::ErasedPtr<T>& obj) const noexcept
  {
    return std::hash<T*>()(obj.get());
  }
};
} // namespace std

#endif
