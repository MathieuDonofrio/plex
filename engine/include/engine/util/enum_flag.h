#ifndef GENEBITS_ENGINE_UTIL_ENUM_FLAG_H
#define GENEBITS_ENGINE_UTIL_ENUM_FLAG_H

#include <type_traits>

namespace genebits::engine
{

template<typename E>
struct EnableBitwiseOperators
{
  static constexpr bool enable = false;
};

#define ENUM_FLAG(Name, Type)            \
  enum class Name : Type;                \
  template<>                             \
  struct EnableBitwiseOperators<Name>    \
  {                                      \
    static constexpr bool enable = true; \
  };                                     \
  enum class Name : Type

template<typename E>
concept BitwiseOperationsEnabled = requires
{
  typename std::enable_if<EnableBitwiseOperators<E>::enable, E>::type;
};

template<class E>
concept EnumFlag = std::is_enum_v<E>&& BitwiseOperationsEnabled<E>;

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
