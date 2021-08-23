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
// Defines all bitwise operators and equality operators with the underlying type for a member enum.
#define DEFINE_MEMBER_ENUM_FLAG_OPERATORS(EnumType)                                                             \
                                                                                                                \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");                \
                                                                                                                \
  friend constexpr EnumType operator|(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                             \
    using Underlying = std::underlying_type_t<EnumType>;                                                        \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));                  \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType operator&(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                             \
    using Underlying = std::underlying_type_t<EnumType>;                                                        \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));                  \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType operator^(const EnumType lhs, const EnumType rhs) noexcept                          \
  {                                                                                                             \
    using Underlying = std::underlying_type_t<EnumType>;                                                        \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));                  \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType& operator&=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                             \
    return (lhs = lhs & rhs);                                                                                   \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType& operator|=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                             \
    return (lhs = lhs | rhs);                                                                                   \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType& operator^=(EnumType& lhs, const EnumType rhs) noexcept                             \
  {                                                                                                             \
    return (lhs = lhs ^ rhs);                                                                                   \
  }                                                                                                             \
                                                                                                                \
  friend constexpr bool operator!=(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept     \
  {                                                                                                             \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) != rhs;                                           \
  }                                                                                                             \
                                                                                                                \
  friend constexpr bool operator==(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept     \
  {                                                                                                             \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) == rhs;                                           \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType operator~(const EnumType rhs) noexcept                                              \
  {                                                                                                             \
    return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(rhs));                          \
  }                                                                                                             \
                                                                                                                \
  friend constexpr EnumType operator&(const EnumType lhs, const std::underlying_type_t<EnumType>(rhs)) noexcept \
  {                                                                                                             \
    using Underlying = std::underlying_type_t<EnumType>;                                                        \
    return static_cast<EnumType>(static_cast<Underlying>(lhs) & rhs);                                           \
  }

// Utility for defining member (or scoped) enum flag operators.
// Defines equality operators with the underlying type for a member enum.
#define DEFINE_MEMBER_ENUM_OPERATORS(EnumType)                                                              \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");            \
  static_assert(!EnumFlag<EnumType>, "Type must not be an enum flag to define enum operators");             \
                                                                                                            \
  friend constexpr bool operator!=(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept \
  {                                                                                                         \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) != rhs;                                       \
  }                                                                                                         \
                                                                                                            \
  friend constexpr bool operator==(const EnumType lhs, const std::underlying_type_t<EnumType> rhs) noexcept \
  {                                                                                                         \
    return static_cast<std::underlying_type_t<EnumType>>(lhs) == rhs;                                       \
  }                                                                                                         \
                                                                                                            \
// Utility for creating bit flags in a more readable way.
#define BitFlag(Bit) (1 << Bit)

///
/// Utility function for finding the sizes of arrays in the context of a variadic macro at compile time.
///
/// @tparam T
/// @tparam size
/// @return The number of elements in the array
template<typename T, std::size_t size>
constexpr size_t GetArrayLength(const T (&)[size])
{
  return size;
}

///
/// Utility for defining a simple ToString() method for debugging member enum flags.
///
/// @param[in] EnumFlagType Typename of the member enum flag to create the function for.
/// @param[in] FunctionName Name of the function that will be created.
/// @param[in] MemberName Name of the member variable that holds the value of the enum flag.
/// @param[in] Variadic Unlimited sequence of string that depicts the string values of the enum \n in the same order
/// they are declared in the enum
///
/// @warning The enum flag must have increments in a power of 2 between flags and should start at 0.\n
/// (example: First = 0, Second = 1\<\<0, Third = 1\<\<1, Fourth = 1\<\<2)
///
#define DEFINE_MEMBER_ENUM_FLAG_TO_STRING(EnumFlagType, FunctionName, MemberName, ...)               \
  static_assert(EnumFlag<EnumFlagType>, "Type must be an enum flag to define enum flag ToString()"); \
                                                                                                     \
  [[nodiscard]] std::string FunctionName() const                                                     \
  {                                                                                                  \
    constexpr const char* string_values[] { __VA_ARGS__ };                                           \
    constexpr std::size_t array_size = GetArrayLength(string_values);                                \
                                                                                                     \
    std::string result;                                                                              \
                                                                                                     \
    for (uint32_t i = 0; i < array_size; ++i)                                                        \
    {                                                                                                \
      if (static_cast<bool>(MemberName & (1 << i)))                                                  \
      {                                                                                              \
        if (result.size() != 0) { result.append(", "); }                                             \
        result.append(string_values[i]);                                                             \
      }                                                                                              \
    }                                                                                                \
                                                                                                     \
    return result;                                                                                   \
  }

///
/// Utility for defining a simple ToString() method for debugging member enum and member enum class.
///
/// @param[in] EnumType Typename of the member enum flag to create the function for.
/// @param[in] FunctionName Name of the function that will be created.
/// @param[in] MemberName Name of the member variable that holds the value of the enum flag.
/// @param[in] Variadic Unlimited sequence of string that depicts the string values of the enum \n in the same order
/// they are declared in the enum
///
/// @warning The enum must be continuous and start at 0 (good: 0,1,2,3... bad: 1,3,5).
///
#define DEFINE_MEMBER_ENUM_TO_STRING(EnumType, FunctionName, MemberName, ...)                     \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");  \
  static_assert(!EnumFlag<EnumType>, "Use DEFINE_ENUM_FLAG_TO_STRING macro for enum flags type"); \
                                                                                                  \
  [[nodiscard]] std::string FunctionName() const                                                  \
  {                                                                                               \
    constexpr const char* string_values[] { __VA_ARGS__ };                                        \
    constexpr std::size_t array_size = GetArrayLength(string_values);                             \
    return string_values[static_cast<std::uint32_t>(MemberName)];                                 \
  }

///
/// Utility for defining a simple ToString() method for debugging non member enum flags.
///
/// @param[in] EnumFlagType Typename of the enum flag to create the function for.
/// @param[in] FunctionName Name of the function that will be created.
/// @param[in] ParameterName of the member variable that holds the value of the enum flag.
/// @param[in] Variadic Unlimited sequence of string that depicts the string values of the enum \n in the same order
/// they are declared in the enum
///
/// @warning The enum flag must have increments in a power of 2 between flags and should start at 0.\n
/// (example: First = 0, Second = 1\<\<0, Third = 1\<\<1, Fourth = 1\<\<2)
///
#define DEFINE_ENUM_FLAG_TO_STRING(EnumFlagType, FunctionName, ParameterName, ...)                   \
  static_assert(EnumFlag<EnumFlagType>, "Type must be an enum flag to define enum flag ToString()"); \
                                                                                                     \
  [[nodiscard]] std::string FunctionName(const EnumFlagType ParameterName) const                     \
  {                                                                                                  \
    constexpr const char* string_values[] { __VA_ARGS__ };                                           \
    constexpr std::size_t array_size = GetArrayLength(string_values);                                \
                                                                                                     \
    std::string result;                                                                              \
                                                                                                     \
    for (uint32_t i = 0; i < array_size; ++i)                                                        \
    {                                                                                                \
      if (static_cast<bool>(ParameterName & (1 << i)))                                               \
      {                                                                                              \
        if (result.size() != 0) { result.append(", "); }                                             \
        result.append(string_values[i]);                                                             \
      }                                                                                              \
    }                                                                                                \
                                                                                                     \
    return result;                                                                                   \
  }

///
/// Utility for defining a simple ToString() method for debugging enum and enum class that are not members of something.
///
/// @param[in] EnumType Typename of the enum flag to create the function for.
/// @param[in] FunctionName Name of the function that will be created.
/// @param[in] ParameterName Name of the function parameter that holds the value of the enum flag.
/// @param[in] Variadic Unlimited sequence of string that depicts the string values of the enum \n in the same order
/// they are declared in the enum
///
/// @warning The enum must be continuous and start at 0 (good: 0,1,2,3... bad: 1,3,5).
///
#define DEFINE_ENUM_TO_STRING(EnumType, FunctionName, ParameterName, ...)                          \
  static_assert(std::is_enum_v<EnumType>, "Type must be an enum to define enum flag operators");   \
  static_assert(!EnumFlag<EnumType>, "Use DEFINE_ENUM_FLAG_TO_STRING macro for enums flags type"); \
                                                                                                   \
  [[nodiscard]] std::string FunctionName(const EnumType ParameterName) const                       \
  {                                                                                                \
    constexpr const char* string_values[] { __VA_ARGS__ };                                         \
    constexpr std::size_t array_size = GetArrayLength(string_values);                              \
    return string_values[static_cast<std::uint32_t>(ParameterName)];                               \
  }

} // namespace genebits::engine
#endif