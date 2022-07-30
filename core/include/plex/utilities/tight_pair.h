#ifndef PLEX_UTILITIES_TIGHT_PAIR_H
#define PLEX_UTILITIES_TIGHT_PAIR_H

#include <cstdint>

#include "plex/debug/assertion.h"
#include "plex/utilities/type_traits.h"

namespace plex
{
namespace details
{
  // Tag used to identify TightPair derived classes.
  struct TightPairBase
  {};
} // namespace details

// clang-format off
///
/// Concept to determine whether a type is eligible for empty base class optimization for a tight pair.
///
/// The type must meet the following requirements:
/// - Must be a class
/// - Must be empty
/// - Type does not inherit from a tight pair
///
template<typename Type>
concept TightPairEBCO = std::is_class_v<Type> && std::is_empty_v<Type>
  && !std::is_base_of_v<details::TightPairBase, Type>;
// clang-format on

namespace details
{
  // Element storage

  template<typename Type, size_t I>
  struct TightPairElementStorage
  {
    Type _element;
    // clang-format off
    constexpr const Type& Get() const& { return _element; }
    constexpr Type& Get() & { return _element; }
    // clang-format on
  };

  // Specialization for empty base class optimization.
  // Uses inheritance rather than storing the element as a member variable.
  template<TightPairEBCO Type, size_t I>
  struct TightPairElementStorage<Type, I> : public Type
  {
    constexpr TightPairElementStorage() = default;
    constexpr TightPairElementStorage(Type) {};

    // clang-format off
    constexpr const Type& Get() const& { return *this; }
    constexpr Type& Get() & { return *this; }
    // clang-format on
  };
} // namespace details

///
/// Utility class to perform empty base optimization.
///
/// Tight pair can contain empty and non empty types. If a type is empty it will be inherited.
///
/// @tparam FirstType First type of pair.
/// @tparam SecondType Second type of pair.
///
template<typename FirstType, typename SecondType>
class TightPair : private details::TightPairElementStorage<FirstType, 0>,
                  private details::TightPairElementStorage<SecondType, 1>
{
private:
  using FirstStorage = details::TightPairElementStorage<FirstType, 0>;
  using SecondStorage = details::TightPairElementStorage<SecondType, 1>;

public:
  ///
  /// Default constructor.
  ///
  constexpr TightPair() = default;

  ///
  /// Parametric Constructor.
  ///
  /// @param[in] first_arg First argument.
  /// @param[in] second_arg Second argument.
  ///
  constexpr TightPair(const FirstType& first_arg, const SecondType& second_arg)
    : FirstStorage { first_arg }, SecondStorage { second_arg }
  {}

  ///
  /// Forwarding Constructor.
  ///
  /// @tparam FirstArg Type of first argument.
  /// @tparam SecondArg Type of second argument.
  ///
  /// @param[in] first_arg First argument.
  /// @param[in] second_arg Second argument.
  ///
  template<std::convertible_to<FirstType> FirstArg = FirstType, std::convertible_to<SecondType> SecondArg = SecondType>
  constexpr TightPair(FirstArg&& first_arg, SecondArg&& second_arg)
    : FirstStorage { std::forward<FirstArg>(first_arg) }, SecondStorage { std::forward<SecondArg>(second_arg) }
  {}

  ///
  /// Returns a const reference to the first element of the pair.
  ///
  /// @return First element.
  ///
  [[nodiscard]] constexpr const FirstType& First() const noexcept
  {
    return FirstStorage::Get();
  }

  ///
  /// Returns a reference to the first element of the pair.
  ///
  /// @return First element.
  ///
  [[nodiscard]] constexpr FirstType& First() noexcept
  {
    return FirstStorage::Get();
  }

  ///
  /// Returns a const reference to the second element of the pair.
  ///
  /// @return Second element.
  ///
  [[nodiscard]] constexpr const SecondType& Second() const noexcept
  {
    return SecondStorage::Get();
  }

  ///
  /// Returns a reference to the second element of the pair.
  ///
  /// @return Second element.
  ///
  [[nodiscard]] constexpr SecondType& Second() noexcept
  {
    return SecondStorage::Get();
  }
};

template<typename FirstType, typename SecondType>
struct IsTriviallyRelocatable<TightPair<FirstType, SecondType>>
  : public std::bool_constant<std::conjunction_v<IsTriviallyRelocatable<FirstType>, IsTriviallyRelocatable<SecondType>>>
{};
} // namespace plex

#endif
