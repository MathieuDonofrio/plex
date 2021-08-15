#ifndef GENEBITS_ENGINE_UTIL_ERASEDPTR_H
#define GENEBITS_ENGINE_UTIL_ERASEDPTR_H

namespace genebits::engine
{
namespace
{
  ///
  /// Template for default deleter for erased pointers.
  ///
  /// @tparam Base Base used to erase type with.
  /// @tparam Type Exact type of the instance to delete.
  ///
  /// @param[in] instance Instance to delete.
  ///
  template<typename Base, typename Type>
  void DefaultDeleter(Base* instance)
  {
    delete static_cast<Type*>(instance);
  }
} // namespace

///
/// Type erasure pointer.
///
/// Stores an instance as a base type to lose the type information.
///
/// @tparam Base Base used to erase type with.
///
template<typename Base = void>
class ErasedPtr
{
public:
  ///
  /// Default Constructor
  ///
  constexpr ErasedPtr() noexcept : instance_(nullptr), deleter_(nullptr) {}

  ///
  /// Parametric Constructor
  ///
  /// @param[in] instance Instance to handle.
  /// @param[in] deleter Deleter to delete the instance.
  ///
  constexpr ErasedPtr(Base* instance, void (*deleter)(Base*)) noexcept : instance_(instance), deleter_(deleter) {}

  ///
  /// Parametric Constructor. Uses default deleter.
  ///
  /// @param[in] instance Instance to handle.
  ///
  template<typename Type>
  constexpr ErasedPtr(Type* instance) noexcept : ErasedPtr(instance, DefaultDeleter<Base, Type>)
  {}

  ///
  /// Move constructor.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  constexpr ErasedPtr(ErasedPtr<Base>&& other) noexcept : ErasedPtr(other.instance_, other.deleter_)
  {
    other.instance_ = nullptr;
    other.deleter_ = nullptr;
  }

  ///
  /// Move assignment operator.
  ///
  /// @param[in] other Other erased ptr to move into this one.
  ///
  /// @return Erased ptr that was assigned.
  ///
  constexpr ErasedPtr& operator=(ErasedPtr<Base>&& other) noexcept
  {
    if (deleter_) deleter_(instance_);

    instance_ = other.instance_;
    other.instance_ = nullptr;
    deleter_ = other.deleter_;
    other.deleter_ = nullptr;

    return *this;
  }

  ErasedPtr(const ErasedPtr& other) = delete;
  ErasedPtr& operator=(const ErasedPtr& other) = delete;

  ///
  /// Destructor
  ///
  ~ErasedPtr()
  {
    if (deleter_) deleter_(instance_);
  }

  ///
  /// Resets the handled instance. Destroys the current instance if any.
  ///
  /// @param[in] instance Instance to handle.
  /// @param[in] deleter Deleter to delete the instance.
  ///
  constexpr void Reset(Base* instance, void (*deleter)(Base*)) noexcept
  {
    if (deleter_) deleter_(instance_);

    instance_ = instance;
    deleter_ = deleter;
  }

  ///
  /// Resets the handled instance. Destroys the current instance if any. Uses
  /// the default deleter.
  ///
  template<typename Type>
  constexpr void Reset(Type* instance) noexcept
  {
    Reset(instance, DefaultDeleter<Base, Type>);
  }

  ///
  /// Casts the instance to the specified type using static cast.
  ///
  /// @warning
  ///     The cast must be the exact type. If this is not possible, consider using
  ///     a polymorphic base and DynamicCast.
  ///
  /// @tparam Type Exact type to cast to.
  ///
  /// @return Casted type.
  ///
  template<typename Type>
  [[nodiscard]] constexpr Type* Cast() noexcept
  {
    return static_cast<Type*>(instance_);
  }

  ///
  /// Casts the instance to the specified type using static cast.
  ///
  /// @warning
  ///     The cast must be the exact type. If this is not possible, consider using
  ///     a polymorphic base and DynamicCast.
  ///
  /// @tparam Type Exact type to cast to.
  ///
  /// @return Casted type.
  ///
  template<typename Type>
  [[nodiscard]] constexpr const Type* Cast() const noexcept
  {
    return static_cast<Type*>(instance_);
  }

  ///
  /// Casts the instance to the specified type using dynamic cast. Can cast anywhere
  /// in the hierarchy as long as the base is polymorphic.
  ///
  /// @note
  ///     Only use this if necessary. Regular cast may be more appropriate and
  ///     performant.
  ///
  /// @tparam Type Type to cast to.
  ///
  /// @return If cast is successful, the casted type, otherwise nullptr.
  ///
  template<typename Type>
  [[nodiscard]] constexpr Type* DynamicCast() noexcept
  {
    return dynamic_cast<Type*>(instance_);
  }

  ///
  /// Casts the instance to the specified type using dynamic cast. Can cast anywhere
  /// in the hierarchy as long as the base is polymorphic.
  ///
  /// @note
  ///     Only use this if necessary. Regular cast may be more appropriate and
  ///     performant.
  ///
  /// @tparam Type Type to cast to.
  ///
  /// @return If cast is successful, the casted type, otherwise nullptr.
  ///
  template<typename Type>
  [[nodiscard]] constexpr const Type* DynamicCast() const noexcept
  {
    return dynamic_cast<Type*>(instance_);
  }

  ///
  /// Returns whether or not the erased pointer contains a handled pointer.
  ///
  /// @return True is has handled pointer, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return instance_;
  }

private:
  Base* instance_;
  void (*deleter_)(Base*);
};
} // namespace genebits::engine

#endif
