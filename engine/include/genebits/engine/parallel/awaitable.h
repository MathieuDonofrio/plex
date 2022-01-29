#ifndef GENEBITS_ENGINE_PARALLEL_AWAITABLE_H
#define GENEBITS_ENGINE_PARALLEL_AWAITABLE_H

// Patch for CLion IDE errors.
// TODO Remove once fixed
#ifndef __cpp_impl_coroutine
#define __cpp_impl_coroutine
#endif
#ifndef __cpp_lib_coroutine
#define __cpp_lib_coroutine
#endif

#include <coroutine>

namespace genebits::engine
{
///
/// Concept used to determine if a type is an awaiter. Meaning it must comply with the standard requirements of an
/// awaiter type.
///
/// @tparam T Type to check.
///
template<typename T>
concept Awaiter = requires(T awaiter, std::coroutine_handle<> handle)
{
  {
    awaiter.await_ready()
    } -> std::convertible_to<bool>;

  awaiter.await_resume();

  awaiter.await_suspend(handle);

  requires std::is_void_v<decltype(std::declval<T>().await_suspend(
    handle))> || std::same_as<decltype(std::declval<T>().await_suspend(handle)),
    bool> || std::same_as<decltype(std::declval<T>().await_suspend(handle)), std::coroutine_handle<>>;
};

///
/// Concept used to determine if a type an awaitable. All awaiters are awaitables, and any type that implements the
/// co_await operator.
///
/// @tparam T Type to check.
///
template<typename T>
concept Awaitable = Awaiter<T> || requires(T awaitable)
{
  awaitable.operator co_await();
};

///
/// Holds type traits of an awaitable
///
/// @tparam T Awaitable type.
///
template<Awaitable T>
struct AwaitableTraits
{
  using AwaiterType = decltype(std::declval<T>().operator co_await());
  using AwaitResultType = decltype(std::declval<AwaiterType>().await_resume());
};

///
/// Holds type traits of an awaitable.
///
/// Specialization for awaiters.
///
/// @tparam T Awaitable type.
///
template<Awaiter T>
struct AwaitableTraits<T>
{
  using AwaiterType = T;
  using AwaitResultType = decltype(std::declval<AwaiterType>().await_resume());
};

} // namespace genebits::engine

#endif // GENEBITS_AWAITABLE_H
