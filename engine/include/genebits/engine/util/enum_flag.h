#ifndef GENEBITS_ENGINE_UTIL_ENUM_FLAG_H
#define GENEBITS_ENGINE_UTIL_ENUM_FLAG_H

#include <type_traits>

namespace genebits::engine
{

template<typename E>
struct EnableBitwiseOperators;

#define ENUM_FLAGS(Name, Type)                          \
  enum class Name : Type;                               \
  template<>                                            \
  struct genebits::engine::EnableBitwiseOperators<Name> \
  {};                                                   \
  enum class Name : Type

template<class E>
concept EnumFlags = std::is_enum_v<E> && requires
{
  EnableBitwiseOperators<E> {};
};

template<EnumFlags EnumType>
constexpr EnumType operator|(const EnumType lhs, const EnumType rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<EnumType>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<EnumFlags EnumType>
constexpr EnumType operator&(const EnumType lhs, const EnumType rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<EnumType>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<EnumFlags EnumType>
constexpr EnumType operator~(const EnumType rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<EnumType>(~static_cast<underlying>(rhs));
}

template<EnumFlags EnumType>
constexpr bool operator!=(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<underlying>(lhs) != rhs;
}

template<EnumFlags EnumType>
constexpr bool operator==(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept
{
  using underlying = std::underlying_type_t<EnumType>;
  return static_cast<underlying>(lhs) == rhs;
}

template<EnumFlags EnumType>
constexpr EnumType& operator&=(EnumType& rhs, const EnumType lhs) noexcept
{
  rhs = rhs & lhs;
  return rhs;
}

template<EnumFlags EnumType>
constexpr EnumType& operator|=(EnumType& rhs, const EnumType lhs) noexcept
{
  rhs = rhs | lhs;
  return rhs;
}

} // namespace genebits::engine
#endif
