#ifndef GENEBITS_ENGINE_UTIL_ENUM_FLAG_H
#define GENEBITS_ENGINE_UTIL_ENUM_FLAG_H

#include <type_traits>

namespace genebits::engine
{

/*
 * Define a template specialization using this template to enable bitwise operators for that enum
 *
 * template<>
 * struct EnableBitwiseOperators<Enum>
 * {
 *  static constexpr bool enable = true;
 * };
 *
 * */
template<typename E>
struct EnableBitwiseOperators
{
  static constexpr bool enable = false;
};

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

} // namespace genebits::engine
#endif
