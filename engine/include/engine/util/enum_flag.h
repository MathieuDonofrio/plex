#ifndef GENEBITS_ENGINE_UTIL_ENUM_FLAG_H
#define GENEBITS_ENGINE_UTIL_ENUM_FLAG_H

#include <type_traits>

namespace genebits::engine
{

template<typename E>
struct EnableBitwiseOperators;

#define ENUM_FLAG(Name, Type)         \
  enum class Name : Type;             \
  template<>                          \
  struct EnableBitwiseOperators<Name> \
  {};                                 \
  enum class Name : Type

// Attempting to call IsTypeComplete will use one of the two definition based on whether the type is complete or not
// This definition gets chosen if the sizeof() function can be called on the type
template<class T, std::size_t = sizeof(T)>
std::true_type IsTypeComplete(T*);

// If a type is incomplete, sizeof() cannot be called on the type, so this definition is chosen
std::false_type IsTypeComplete(...);

// By calling IsTypeComplete with a fake instance of the type using declval, one of the two definition of IsTypeComplete is chosen.
// Finally, by using decltype we can know which definition of IsTypeComplete has been chosen based on the different return types
template<class T>
using IsComplete = decltype(IsTypeComplete(std::declval<T*>()));

template<class E>
concept EnumFlag = std::is_enum_v<E>&& IsComplete<EnableBitwiseOperators<E>>::value;

template<typename EnumType>
requires(EnumFlag<EnumType>) constexpr EnumType operator|(EnumType lhs, EnumType rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<EnumType>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename EnumType>
requires(EnumFlag<EnumType>) constexpr EnumType operator&(EnumType lhs, EnumType rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<EnumType>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename EnumType>
requires(EnumFlag<EnumType>) constexpr bool operator!=(EnumType lhs, std::underlying_type_t<EnumType> rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<underlying>(lhs) != rhs;
}

template<typename EnumType>
requires(EnumFlag<EnumType>) constexpr bool operator==(EnumType lhs, std::underlying_type_t<EnumType> rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<underlying>(lhs) == rhs;
}

} // namespace genebits::engine
#endif
