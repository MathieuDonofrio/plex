#ifndef GENEBITS_ENGINE_UTIL_EVENTHANDLER_H
#define GENEBITS_ENGINE_UTIL_EVENTHANDLER_H

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
/// @tparam Event Type of the event to invoke.
///
template<typename Invokable, typename Event>
concept EventHandlerInvokable = std::is_class_v<Invokable> && std::is_trivially_destructible_v<Invokable> && sizeof(
                                  Invokable)
                                  <= sizeof(void*)
                                && requires(Invokable invokable, const Event& event)
{
  invokable(event);
};

///
/// Highly optimized delegate for handling events. Supports binding free functions,
/// member functions & sometimes invokables.
///
/// Zero overhead for member functions (the most commonly used). 8 byte memory overhead
/// for free functions. Maximum of 8 byte memory overhead for invokable. No call overhead.
///
/// @tparam Event Type of event for delegate to handle.
///
template<typename Event>
class EventHandler
{
public:
  ///
  /// Default Constructor
  ///
  constexpr EventHandler() noexcept : function_(nullptr), storage_(nullptr) {}

  ///
  /// Binds a free function.
  ///
  /// Pointer size (8 bytes for 64 bit) of memory overhead and no call overhead.
  ///
  /// @tparam FreeFunction Compile-time free function pointer.
  ///
  template<void (*FreeFunction)(const Event&)>
  constexpr void Bind() noexcept
  {
    storage_ = nullptr;

    function_ = [](void*, const Event& event) { (*FreeFunction)(event); };
  }

  ///
  /// Binds a member function.
  ///
  /// No overhead.
  ///
  /// @tparam MemberFunction Compile-time member function pointer.
  /// @tparam Type The type of class the member function is for.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, void (Type::*MemberFunction)(const Event&)>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, const Event& event) { (static_cast<Type*>(storage)->*MemberFunction)(event); };
  }

  ///
  /// Binds a const member function.
  ///
  /// No overhead.
  ///
  /// @tparam MemberFunction Compile-time member function pointer.
  /// @tparam Type The type of class the member function is for.
  ///
  /// @param[in] instance The instance to call the member function for.
  ///
  template<typename Type, void (Type::*MemberFunction)(const Event&) const>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, const Event& event) { (static_cast<const Type*>(storage)->*MemberFunction)(event); };
  }

  ///
  /// Binds an invokable.
  ///
  /// An invokable, also known as a functor, can be bound to the event handler if it meets the conditions.
  /// of the EventHandlerInvokable concept. To summarize, the invokable must define a invoke operator with
  /// the event type as an argument, must be trivially destructible and smaller than the size of a pointer.
  ///
  /// No overhead if the invokable uses all the available size. Maximum of
  /// pointer size (8 bytes on 64 bit) of overhead.
  ///
  /// @see EventHandlerInvokable
  ///
  /// @tparam Invokable The type of the invokable.
  ///
  /// @param[in] invokable The instance of the invokable.
  ///
  template<EventHandlerInvokable<Event> Invokable>
  constexpr void Bind(Invokable&& invokable) noexcept
  {
    new (&storage_) Invokable(std::move(invokable));

    function_ = [](void* storage, const Event& event) { reinterpret_cast<Invokable*>(&storage)->operator()(event); };
  }

  ///
  /// Invokes the stored function with the specified event.
  ///
  /// @warning
  ///     Undefined behaviour if no function is bound to this event handler.
  ///
  /// @param[in] event The event to invoke with.
  ///
  constexpr void Invoke(const Event& event)
  {
    ASSERT(function_ != nullptr, "No bound function");

    function_(storage_, event);
  }

  ///
  /// Equality operator.
  ///
  /// Checks if the handlers have the same content (function & storage).
  ///
  /// @param[in] other Other handler to compare.
  ///
  /// @return True if the handlers are equal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator==(const EventHandler<Event> other) const noexcept
  {
    return function_ == other.function_ && storage_ == other.storage_;
  }

  ///
  /// Inequality operator.
  ///
  /// Checks if the handlers has different content (function & storage).
  ///
  /// @param[in] other Other handler to compare.
  ///
  /// @return True if the handlers are unequal, false otherwise.
  ///
  [[nodiscard]] constexpr bool operator!=(const EventHandler<Event> other) const noexcept
  {
    return function_ != other.function_ || storage_ != other.storage_;
  }

  ///
  /// Bool operator.
  ///
  /// @return True if the handler had been bound with a function, false otherwise.
  ///
  [[nodiscard]] constexpr operator bool() const noexcept
  {
    return function_ != nullptr;
  }

private:
  void (*function_)(void*, const Event&);
  void* storage_;
};

} // namespace genebits::engine

#endif
