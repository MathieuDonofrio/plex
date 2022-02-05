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
#if 1
// Include first
#include <coroutine>
#endif

#include <tuple>
#include <type_traits>

///
/// Used to define the default unhandled_exception for coroutine promises.
///
#define COROUTINE_UNHANDLED_EXCEPTION                         \
  void unhandled_exception() const noexcept                   \
  {                                                           \
    ASSERT(false, "Unhandled exception thrown in coroutine"); \
  }

namespace genebits::engine
{
///
/// Concept used to determine if a type is an awaiter. Meaning it must comply with the standard requirements of an
/// awaiter type.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept Awaiter = requires(Type awaiter, std::coroutine_handle<> handle)
{
  {
    awaiter.await_ready()
    } -> std::convertible_to<bool>;

  awaiter.await_resume();

  awaiter.await_suspend(handle);

  requires std::is_void_v<decltype(std::declval<Type>().await_suspend(
    handle))> || std::same_as<decltype(std::declval<Type>().await_suspend(handle)),
    bool> || std::same_as<decltype(std::declval<Type>().await_suspend(handle)), std::coroutine_handle<>>;
};

///
/// Concept used to determine if a type an awaitable. All awaiters are awaitables, and any type that implements the
/// co_await operator.
///
/// @tparam Type Type to check.
///
template<typename Type>
concept Awaitable = Awaiter<Type> || requires(Type awaitable)
{
  awaitable.operator co_await();
};

///
/// Concept that determines whether or not a type can provide a WhenReady awaitable.
///
/// This awaitable is normally used as a shortcut for ignoring the result.
///
/// @tparam Type Awaitable to check
///
template<typename Type>
concept WhenReadyAwaitable = requires(Type awaitable)
{
  {
    awaitable.WhenReady()
    } -> Awaitable;
};

///
/// Holds type traits of an awaitable
///
/// @tparam T Awaitable type.
///
template<Awaitable Type>
struct AwaitableTraits
{
  using AwaiterType = decltype(std::declval<Type>().operator co_await());
  using AwaitResultType = decltype(std::declval<AwaiterType>().await_resume());
};

///
/// Holds type traits of an awaitable.
///
/// Specialization for awaiters.
///
/// @tparam T Awaitable type.
///
template<Awaiter Type>
struct AwaitableTraits<Type>
{
  using AwaiterType = Type;
  using AwaitResultType = decltype(std::declval<AwaiterType>().await_resume());
};

///
/// Used as a tag for void awaitable results in tuples.
///
struct VoidAwaitResult
{};

///
/// Returns an aggregate of the result types of all awaitable.
///
/// Aggregate is a tuple.
///
/// @note If an awaitable has a void result, the result type in the tuple will be VoidAwaitResult.
///
/// @tparam Awaitables All the awaitables types to aggregate results for.
///
template<Awaitable... Awaitables>
using AgrAwaitResult =
  std::tuple<std::conditional_t<std::is_void_v<typename AwaitableTraits<Awaitables>::AwaitResultType>,
    VoidAwaitResult,
    typename std::remove_reference_t<typename AwaitableTraits<Awaitables>::AwaitResultType>>...>;

} // namespace genebits::engine

#endif // GENEBITS_AWAITABLE_H
