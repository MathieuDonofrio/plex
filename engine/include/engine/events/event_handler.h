#ifndef GENEBITS_ENGINE_UTIL_EVENTHANDLER_H
#define GENEBITS_ENGINE_UTIL_EVENTHANDLER_H

#include <type_traits>

namespace genebits::engine
{
template<typename Invokable, typename Event>
concept EventHandlerInvokable = std::is_class_v<Invokable> && sizeof(Invokable) <= sizeof(void*) && requires(Invokable invokable, const Event& event)
{
  invokable(event);
};

template<typename Event>
class EventHandler
{
public:
  constexpr EventHandler() noexcept
    : function_(nullptr), storage_(nullptr)
  {
  }

  template<auto FreeFunction>
  requires std::is_invocable_v<decltype(FreeFunction), const Event&>
  constexpr void Bind() noexcept
  {
    storage_ = nullptr;

    function_ = [](void*, const Event& event)
    {
      std::invoke(FreeFunction, event);
    };
  }

  template<auto MemberFunction, typename Type>
  requires std::is_member_function_pointer_v<decltype(MemberFunction)>
  constexpr void Bind(Type* instance) noexcept
  {
    storage_ = instance;

    function_ = [](void* storage, const Event& event)
    {
      std::invoke(MemberFunction, static_cast<Type*>(storage), event);
    };
  }

  template<EventHandlerInvokable<Event> Invokable>
  constexpr void Bind(Invokable invokable) noexcept
  {
    new (&storage_) Invokable(std::move(invokable));

    function_ = [](void* storage, const Event& event)
    {
      reinterpret_cast<Invokable*>(&storage)->operator()(event);
    };
  }

  constexpr void Invoke(const Event& event)
  {
    function_(storage_, event);
  }

  [[nodiscard]] constexpr bool operator==(const EventHandler<Event> handler) const noexcept
  {
    return function_ == handler.function_ && storage_ == handler.storage_;
  }

  [[nodiscard]] constexpr bool operator!=(const EventHandler<Event> handler) const noexcept
  {
    return function_ != handler.function_ || storage_ != handler.storage_;
  }

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
