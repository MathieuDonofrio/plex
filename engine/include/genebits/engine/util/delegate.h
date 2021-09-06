#ifndef GENEBITS_ENGINE_UTIL_DELEGATE_H
#define GENEBITS_ENGINE_UTIL_DELEGATE_H

#include <type_traits>
#include <utility>

#include "genebits/engine/debug/assertion.h"

namespace genebits::engine
{
///
/// Concept used to determine if an Invokable is eligible to be bound to an EventHandler.
///
/// This concept is more restrictive than invokable binding for other delegates like
/// std::function. The justification is because we do not want to create additional overhead
/// for something we will rarely use.
///
/// To meet the requirements of an event handler invokable, the invokable must define a valid invoke
/// operator with the event type as an argument. Also, the invokable cannot be larger than the size
/// of a pointer (8 bytes on 64 bit) and must be trivially destructible.
///
/// @tparam Invokable Type of the invokable (usually a lambda).
/// @tparam Args Argument types of the delegate.
///
template<typename Invocable, typename... Args>
concept DelegateInvocable = std::is_trivially_destructible_v<Invocable> && sizeof(Invocable) <= sizeof(void*)
                            && requires(Invocable invocable, Args... args)
{
  invocable(std::forward<Args>(args)...);
};

///
/// Highly optimized delegate. Supports binding free functions, member functions & some invocables.
///
/// Zero overhead for member functions (the most commonly used). 8 byte memory overhead
/// for free functions. Maximum of 8 byte memory overhead for invokable. No call overhead.
///
/// @tparam Args Argument types of the delegate.
///
template<typename... Args>
class Delegate
{
public:
  ///
  /// Default Constructor
  ///
  constexpr Delegate() noexcept : function_(nullptr), storage_(nullptr) {}

  ///
  /// Binds a free function.
  ///
  /// @tparam FreeFunction Compile-time free function pointer.
  ///
  template<void (*FreeFunction)(Args...)>
  constexpr void Bind() noexcept
  {
    storage_ = nullptr;

    function_ = [](void*, Args... args) { (*FreeFunction)(std::forward<Args>(args)...); };
  }

  ///
  /// Binds a member function.
  ///
  /// @tparam Type The type of class the member function is for.
  /// @tparam MemberFunction Compile-time member function pointer.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, void (Type::*MemberFunction)(Args...)>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, Args... args)
    { (static_cast<Type*>(storage)->*MemberFunction)(std::forward<Args>(args)...); };
  }

  ///
  /// Binds a const member function.
  ///
  /// @tparam Type The type of class the member function is for.
  /// @tparam MemberFunction Compile-time member function pointer.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, void (Type::*MemberFunction)(Args...) const>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, Args... args)
    { (static_cast<const Type*>(storage)->*MemberFunction)(std::forward<Args>(args)...); };
  }

  ///
  /// Binds an invokable.
  ///
  /// @see DelegateInvokable
  ///
  /// @tparam Invocable The type of the invocable.
  ///
  /// @param[in] invocable The instance of the invocable.
  ///
  template<DelegateInvocable<Args...> Invocable>
  constexpr void Bind(Invocable invocable) noexcept
  {
    new (&storage_) Invocable(std::move(invocable));

    function_ = [](void* storage, Args... args)
    { reinterpret_cast<Invocable*>(&storage)->operator()(std::forward<Args>(args)...); };
  }

  ///
  /// Invokes the stored function with the specified event.
  ///
  /// @warning
  ///     Undefined behaviour if no function is bound to this delegate.
  ///
  /// @param[in] args The arguments to invoke with.
  ///
  constexpr void Invoke(Args... args)
  {
    ASSERT(function_ != nullptr, "No bound function");

    function_(storage_, std::forward<Args>(args)...);
  }

  ///
  /// Equality operator.
  ///
  /// Checks if the delegates have the same content (function & storage).
  ///
  /// @param[in] other Other delegate to compare.
  ///
  /// @return True if the delegates are equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator==(const Delegate<Args...> other) const noexcept
  {
    return function_ == other.function_ && storage_ == other.storage_;
  }

  ///
  /// Inequality operator.
  ///
  /// Checks if the delegates have different content (function & storage).
  ///
  /// @param[in] other Other delegate to compare.
  ///
  /// @return True if the delegates are unequal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator!=(const Delegate<Args...> other) const noexcept
  {
    return function_ != other.function_ || storage_ != other.storage_;
  }

  ///
  /// Bool operator.
  ///
  /// @return True if the delegate is bound with a function, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return function_ != nullptr;
  }

private:
  void (*function_)(void*, Args...);
  void* storage_;
};

} // namespace genebits::engine

#endif
