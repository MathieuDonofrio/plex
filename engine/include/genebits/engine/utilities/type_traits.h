#ifndef GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H
#define GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H

#include <memory>
#include <tuple>
#include <type_traits>

namespace plex
{
namespace details
{
#define TYPE_TRAITS_DETECTOR(Trait) \
  template<typename Type>           \
  concept Detect_##Trait = std::same_as<typename Type::Trait, std::true_type>;

  TYPE_TRAITS_DETECTOR(IsTriviallyRelocatable);
  TYPE_TRAITS_DETECTOR(IsThreadSafe);
} // namespace details

///
/// Whether or not the type can be moved around in memory using a memory copy instead of constructors. This avoids
/// needing the call a constructor and destructor when simply moving something.
///
/// Most types should be relocatable. To not be relocatable the type would need to have a pointer that points to
/// itself internally or something crazy going on.
///
/// If a type is trivially copyable, it is also trivially relocatable, we approximate this to include types that are
/// not trivially assignable but are trivially copy/move constructable and destructible. This allows us to implicitly
/// capture many important cases.
///
/// @note The c++ standard may one day have a built-in way of doing this. Watch out for something like
/// std::is_trivially_relocatable<type>. Proposal:
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1144r5.html Cppcon 2018:
/// https://www.youtube.com/watch?v=xxta6LEn9Hk
///
/// @note For a type to be trivially relocatable, it must either have the trivial properties required or have a
/// specialization for this struct or have a using tag IsTriviallyRelocatable = std::true_type.
///
/// @tparam Type Type to check for relatability.
///
template<typename Type>
struct IsTriviallyRelocatable
  : public std::bool_constant<
      details::Detect_IsTriviallyRelocatable<Type> || // Note that there is no way to inherit the trait, watch
                                                      // P1144r5 for this feature in the future.
      std::is_empty_v<Type> || // For cases like std::allocator
      ((std::is_trivially_copy_constructible_v<
          Type> || std::is_trivially_move_constructible_v<Type>)&&std::is_trivially_destructible_v<Type>)>
{};

// Specialize commonly used STL types

template<typename... Types>
struct IsTriviallyRelocatable<std::tuple<Types...>>
  : public std::bool_constant<std::conjunction_v<IsTriviallyRelocatable<Types>...>>
{};

template<typename First, typename Second>
struct IsTriviallyRelocatable<std::pair<First, Second>>
  : public std::conjunction<IsTriviallyRelocatable<First>, IsTriviallyRelocatable<Second>>
{};

template<typename Type>
struct IsTriviallyRelocatable<std::unique_ptr<Type>> : public std::true_type
{};

///
/// Trait used to detect whether of not a type is thread safe.
///
/// This means that no matter how the type is used, member access, methods... it will be thread safe.
///
/// @note For a type to be thread safe, it must either have the trivial properties required or have a
/// specialization for this struct or have a using tag IsThreadSafe = std::true_type.
///
/// @tparam[in] Type Type to check for thread-safety.
///
template<typename Type>
struct IsThreadSafe : public std::bool_constant<details::Detect_IsThreadSafe<Type> // Using tag
                                                || std::is_empty_v<Type> // If there is no data it must be thread safe
                        >
{};

///
/// Returns whether or not all the types in the variadic template are unique.
///
/// @tparam Types Types to check for uniqueness.
///
template<typename... Types>
struct IsUniqueTypes : std::true_type
{};

template<typename Type, typename... Types>
struct IsUniqueTypes<Type, Types...>
  : std::conjunction<std::negation<std::disjunction<std::is_same<Type, Types>...>>, IsUniqueTypes<Types...>>
{};

///
/// Returns whether or not all the types in the variadic template are unique.
///
/// @tparam Types Types to check for uniqueness.
///
template<typename... Types>
concept UniqueTypes = IsUniqueTypes<Types...>::value;
} // namespace plex

#endif
