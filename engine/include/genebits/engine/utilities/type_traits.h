#ifndef GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H
#define GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H

#include <memory>
#include <tuple>
#include <type_traits>

namespace genebits::engine
{
namespace details
{
#define TYPE_TRAITS_DETECTOR(Trait) \
  template<typename Type>           \
  concept Detect_##Trait = std::same_as<typename Type::Trait, std::true_type>;

  TYPE_TRAITS_DETECTOR(IsTriviallyRelocatable);
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
/// @tparam Type Type to check for relatability.
///
template<typename Type>
struct IsTriviallyRelocatable
  : std::bool_constant<
      details::Detect_IsTriviallyRelocatable<Type> || // Note that there is no way to inherit the trait, watch
                                                      // P1144r5 for this feature in the future.
      std::is_empty_v<Type> || // For cases like std::allocator
      ((std::is_trivially_copy_constructible_v<
          Type> || std::is_trivially_move_constructible_v<Type>)&&std::is_trivially_destructible_v<Type>)>
{};

// Specialize commonly used STL types

template<typename... Types>
struct IsTriviallyRelocatable<std::tuple<Types...>>
  : std::bool_constant<std::conjunction_v<IsTriviallyRelocatable<Types>...>>
{};

template<typename First, typename Second>
struct IsTriviallyRelocatable<std::pair<First, Second>>
  : std::conjunction<IsTriviallyRelocatable<First>, IsTriviallyRelocatable<Second>>
{};

template<typename Type>
struct IsTriviallyRelocatable<std::unique_ptr<Type>> : std::true_type
{};

} // namespace genebits::engine

#endif
