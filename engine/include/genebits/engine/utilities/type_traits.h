#ifndef GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H
#define GENEBITS_ENGINE_UTILITIES_TYPE_TRAITS_H

#include <memory>
#include <tuple>
#include <type_traits>

namespace genebits::engine
{
namespace details
{
#define TYPE_TRAITS_DETECTOR(Trait)                     \
  template<typename Type>                               \
  concept Detect_##Trait = requires()                   \
  {                                                     \
    std::same_as<typename Type::Trait, std::true_type>; \
  };

  TYPE_TRAITS_DETECTOR(IsRelocatable);
} // namespace details

///
/// Whether or not the type can be moved around in memory using a memory copy instead of constructors. This avoids
/// needing the call a constructor and destructor when simply moving something.
///
/// Most types should be relocatable. To not be relocatable the type would need to have a pointer that points to itself
/// internally or something crazy going on.
///
/// If a type is trivially copyable, it is also trivially relocatable, we approximate this to include types that are not
/// trivially assignable but are trivially copy/move constructable and destructible. This allows us to implicitly
/// capture many important cases.
///
/// @note The c++ standard may one day have a built-in way of doing this. Watch out for something like
/// std::is_trivially_relocatable<type>. Cppcon 2018: https://www.youtube.com/watch?v=xxta6LEn9Hk
///
/// @tparam Type Type to check for relocatability.
///
template<typename Type>
struct IsRelocatable
  : std::bool_constant<
      details::Detect_IsRelocatable<Type> || // Note that there is no way to propagate the trait using detect
      ((std::is_trivially_copy_constructible_v<
          Type> || std::is_trivially_move_constructible_v<Type>)&&std::is_trivially_destructible_v<Type>)>
{};

// Specialize commonly used STL types

template<typename... Types>
struct IsRelocatable<std::tuple<Types...>> : std::bool_constant<std::conjunction_v<IsRelocatable<Types>...>>
{};

template<typename First, typename Second>
struct IsRelocatable<std::pair<First, Second>> : std::conjunction<IsRelocatable<First>, IsRelocatable<Second>>
{};

template<typename Type>
struct IsRelocatable<std::unique_ptr<Type>> : std::true_type
{};

} // namespace genebits::engine

#endif
