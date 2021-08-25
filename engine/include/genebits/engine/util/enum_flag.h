#ifndef GENEBITS_ENGINE_UTIL_ENUM_FLAG_H
#define GENEBITS_ENGINE_UTIL_ENUM_FLAG_H

#include <concepts>
#include <type_traits>

namespace genebits::engine
{

///
/// Concept used to determine if an enum is enum flag.
///
/// To be an enum flag, the enum must define all bitwise operators.
///
/// @tparam EnumType Enum type to check
///
template<typename EnumType>
concept EnumFlag = std::is_enum_v<EnumType> && requires(EnumType lhs, EnumType rhs)
{
  {
    lhs | rhs
    } -> std::convertible_to<EnumType>;
  {
    lhs& rhs
    } -> std::convertible_to<EnumType>;
  {
    lhs ^ rhs
    } -> std::convertible_to<EnumType>;
  {
    lhs |= rhs
    } -> std::convertible_to<EnumType&>;
  {
    lhs &= rhs
    } -> std::convertible_to<EnumType&>;
  {
    lhs ^= rhs
    } -> std::convertible_to<EnumType&>;
  ~lhs;
};

// Utility for defining enum flag operators.
// Defines all bitwise operators and equality operators with the underlying type.
#define DEFINE_ENUM_FLAG_OPERATORS(EnumType)                                                             \
                                                                                                         \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");         \
                                                                                                         \
  constexpr EnumType operator|(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                      \
    using Underlying = std::underlying_type_t<EnumType>;                                                 \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));           \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType operator&(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                      \
    using Underlying = std::underlying_type_t<EnumType>;                                                 \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));           \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType operator^(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                      \
    using Underlying = std::underlying_type_t<EnumType>;                                                 \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));           \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType& operator&=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                      \
    return (lhs = lhs & rhs);                                                                            \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType& operator|=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                      \
    return (lhs = lhs | rhs);                                                                            \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType& operator^=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                      \
    return (lhs = lhs ^ rhs);                                                                            \
  }                                                                                                      \
                                                                                                         \
  constexpr bool operator!=(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept     \
  {                                                                                                      \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) != rhs;                                    \
  }                                                                                                      \
                                                                                                         \
  constexpr bool operator==(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept     \
  {                                                                                                      \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) == rhs;                                    \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType operator~(const EnumType rhs) noexcept                                              \
  {                                                                                                      \
    return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(rhs));                   \
  }                                                                                                      \
                                                                                                         \
  constexpr EnumType operator&(const EnumType lhs, const std::underlying_type_t<EnumType>(rhs)) noexcept \
  {                                                                                                      \
    using Underlying = std::underlying_type_t<EnumType>;                                                 \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) & rhs);                                    \
  }

// Utility for defining member (or scoped) enum flag operators.
// Defines equality operators with the underlying type for a member enum.
#define DEFINE_ENUM_OPERATORS(EnumType)                                                              \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");     \
  static_assert(!EnumFlag<EnumType>, "Type must not be an enum flag to define enum operators");      \
                                                                                                     \
  constexpr bool operator!=(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept \
  {                                                                                                  \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) != rhs;                                \
  }                                                                                                  \
                                                                                                     \
  constexpr bool operator==(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept \
  {                                                                                                  \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) == rhs;                                \
  }                                                                                                  \
                                                                                                     \
// Utility for creating bit flags in a more readable way.
#define BitFlag(Bit) (1 << Bit)

} // namespace genebits::engine
#endif