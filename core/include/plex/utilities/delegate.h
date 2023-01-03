#ifndef PLEX_UTILITIES_DELEGATE_H
#define PLEX_UTILITIES_DELEGATE_H

#include <type_traits>
#include <utility>

#include "plex/debug/assertion.h"

namespace plex
{
// clang-format off

///
/// Concept used to determine if an Invokable is eligible to be bound to an Delegate.
///
/// This concept is more restrictive than invokable binding for other delegates like
/// std::function. The justification is because we do not want to create additional overhead
/// for something we will rarely use.
///
/// To meet the requirements of an delegate invokable, the invokable must define a valid invoke
/// operator with the argument types. Also, the invokable cannot be larger than the size
/// of a pointer (8 bytes on 64 bit) and must be trivially destructible.
///
/// @tparam Invokable Type of the invokable (usually a lambda).
/// @tparam Args Argument types of the delegate.
///
template<typename Invocable, typename Return, typename... Args>
concept DelegateInvocable = std::is_trivially_destructible_v<Invocable> && sizeof(Invocable) <= sizeof(void*)
                            && requires(Invocable invocable, Args... args)
{
  {
    invocable(std::forward<Args>(args)...)
    } -> std::convertible_to<Return>;
};

// clang-format on

#define DELEGATE_INVOKE_AND_TRY_RETURN(expression)                        \
  if constexpr (std::is_same_v<decltype(expression), void>) (expression); \
  else                                                                    \
    return (expression);

template<typename>
class Delegate;

///
/// Highly optimized delegate. Supports binding free functions, member functions & some invocables.
///
/// Zero overhead for member functions (the most commonly used). 8 byte memory overhead
/// for free functions. Maximum of 8 byte memory overhead for invokable. No call overhead.
///
/// @tparam Args Argument types of the delegate.
///
template<typename Return, typename... Args>
class Delegate<Return(Args...)>
{
public:
  ///
  /// Default Constructor
  ///
  constexpr Delegate() noexcept : function_(nullptr), storage_(nullptr) {}

  ///
  /// Binds a free function.
  ///
  /// @tparam FreeFunction CompileToSpv-time free function pointer.
  ///
  template<Return (*FreeFunction)(Args...)>
  constexpr void Bind() noexcept
  {
    storage_ = nullptr;

    function_ = [](void*, Args... args)
    {
      // Invoke free function
      DELEGATE_INVOKE_AND_TRY_RETURN((*FreeFunction)(std::forward<Args>(args)...))
    };
  }

  ///
  /// Binds a member function.
  ///
  /// @tparam Type The type of class the member function is for.
  /// @tparam MemberFunction CompileToSpv-time member function pointer.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, Return (Type::*MemberFunction)(Args...)>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, Args... args)
    {
      // Invoke member function
      DELEGATE_INVOKE_AND_TRY_RETURN((static_cast<Type*>(storage)->*MemberFunction)(std::forward<Args>(args)...))
    };
  }

  ///
  /// Binds a const member function.
  ///
  /// @tparam Type The type of class the member function is for.
  /// @tparam MemberFunction CompileToSpv-time member function pointer.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, Return (Type::*MemberFunction)(Args...) const>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, Args... args)
    {
      // Invoke const member function
      DELEGATE_INVOKE_AND_TRY_RETURN((static_cast<const Type*>(storage)->*MemberFunction)(std::forward<Args>(args)...))
    };
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
  template<DelegateInvocable<Return, Args...> Invocable>
  constexpr void Bind(Invocable invocable) noexcept
  {
    new (&storage_) Invocable(std::move(invocable));

    function_ = [](void* storage, Args... args)
    {
      // Invoke invokable
      DELEGATE_INVOKE_AND_TRY_RETURN(reinterpret_cast<Invocable*>(&storage)->operator()(std::forward<Args>(args)...))
    };
  }

  ///
  /// Invokes the stored function with the given arguments.
  ///
  /// @warning
  ///     Undefined behaviour if no function is bound to this delegate.
  ///
  /// @param[in] args The arguments to invoke with.
  ///
  constexpr Return Invoke(Args... args)
  {
    ASSERT(function_ != nullptr, "No bound function");

    DELEGATE_INVOKE_AND_TRY_RETURN(function_(storage_, std::forward<Args>(args)...))
  }

  ///
  /// Invokes the stored function with the given arguments.
  ///
  /// @warning
  ///     Undefined behaviour if no function is bound to this delegate.
  ///
  /// @param[in] args The arguments to invoke with.
  ///
  constexpr Return operator()(Args... args)
  {
    DELEGATE_INVOKE_AND_TRY_RETURN(Invoke(std::forward<Args>(args)...))
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
  [[nodiscard]] constexpr bool operator==(const Delegate<Return(Args...)> other) const noexcept
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
  [[nodiscard]] constexpr bool operator!=(const Delegate<Return(Args...)> other) const noexcept
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
  Return (*function_)(void*, Args...);
  void* storage_;
};

#undef DELEGATE_INVOKE_AND_TRY_RETURN

} // namespace plex

#endif
