#ifndef PLEX_UTILITIES_PUPLE_H
#define PLEX_UTILITIES_PUPLE_H

#include <tuple>
#include <type_traits>

#include "plex/utilities/type_traits.h"

namespace plex
{
namespace details
{
  template<typename Type>
  struct PuplePointer
  {
    Type* _pointer;
  };

  template<typename... Types>
  class PupleBase : public PuplePointer<Types>...
  {
  public:
    constexpr PupleBase() noexcept = default;

    constexpr PupleBase(Types*... args) noexcept : PuplePointer<Types> { args }... {}
  };

  template<typename Type>
  class PupleBase<Type> : protected PuplePointer<Type>
  {
  public:
    constexpr PupleBase() noexcept = default;

    constexpr PupleBase(Type* arg) noexcept : PuplePointer<Type> { arg } {}

    // clang-format off
    [[nodiscard]] const Type* operator->() const noexcept { return this->_pointer; }
    [[nodiscard]] Type* operator->() noexcept { return this->_pointer; }

    [[nodiscard]] const Type& operator*() const noexcept { return *this->_pointer; }
    [[nodiscard]] Type& operator*() noexcept { return *this->_pointer; }

    constexpr operator Type*() const noexcept { return this->_pointer; }
    constexpr operator Type() const noexcept { return *this->_pointer; }
    // clang-format on
  };

  template<>
  class PupleBase<>
  {};

} // namespace details

///
/// Tuple of pointers.
///
/// Lightweight compile-time and zero overhead.
///
/// Support for structured bindings of dereferenced pointers, and for Puple with a single type, supports fancy pointer
/// syntax.
///
/// @note All types must be unique.
///
/// @tparam Types Set of types.
///
template<typename... Types>
requires UniqueTypes<Types...>
class Puple : public details::PupleBase<Types...>
{
public:
  constexpr Puple() noexcept = default;

  constexpr Puple(Types*... args) noexcept : details::PupleBase<Types...> { args... } {}

  template<size_t I>
  [[nodiscard]] constexpr ::std::tuple_element_t<I, Puple<Types...>>*& get_pointer() noexcept
  {
    static_assert(I < sizeof...(Types), "Index out of bounds");
    return details::PuplePointer<::std::tuple_element_t<I, Puple<Types...>>>::_pointer;
  }

  template<size_t I>
  [[nodiscard]] constexpr ::std::tuple_element_t<I, Puple<Types...>>* get_pointer() const noexcept
  {
    static_assert(I < sizeof...(Types), "Index out of bounds");
    return details::PuplePointer<::std::tuple_element_t<I, Puple<Types...>>>::_pointer;
  }

  template<typename T>
  [[nodiscard]] constexpr T*& get_pointer() noexcept
  {
    return details::PuplePointer<T>::_pointer;
  }

  template<typename T>
  [[nodiscard]] constexpr T* get_pointer() const noexcept
  {
    return details::PuplePointer<T>::_pointer;
  }

  template<size_t Index>
  [[nodiscard]] constexpr std::tuple_element_t<Index, ::plex::Puple<Types...>>& get() noexcept
  {
    return *get_pointer<Index>();
  }

  template<size_t Index>
  [[nodiscard]] constexpr const std::tuple_element_t<Index, ::plex::Puple<Types...>>& get() const noexcept
  {
    return *get_pointer<Index>();
  }

  template<typename Type>
  [[nodiscard]] constexpr Type& get() noexcept
  {
    return *get_pointer<Type>();
  }

  template<typename Type>
  [[nodiscard]] constexpr const Type& get() const noexcept
  {
    return *get_pointer<Type>();
  }
};
} // namespace plex

#define DEFINE_PUPLE_LIKE(PupleType)                                                                                   \
  namespace std                                                                                                        \
  {                                                                                                                    \
    template<typename... Types>                                                                                        \
    struct tuple_size<PupleType<Types...>>                                                                             \
    {                                                                                                                  \
      static constexpr size_t value = sizeof...(Types);                                                                \
    };                                                                                                                 \
                                                                                                                       \
    template<size_t Index, typename Type, typename... Types>                                                           \
    struct tuple_element<Index, PupleType<Type, Types...>> : public tuple_element<Index - 1, PupleType<Types...>>      \
    {};                                                                                                                \
                                                                                                                       \
    template<typename Type, typename... Types>                                                                         \
    struct tuple_element<0, PupleType<Type, Types...>>                                                                 \
    {                                                                                                                  \
      using type = Type;                                                                                               \
    };                                                                                                                 \
                                                                                                                       \
    template<size_t Index, typename... Types>                                                                          \
    [[nodiscard]] constexpr std::tuple_element_t<Index, PupleType<Types...>>& get(PupleType<Types...>& puple) noexcept \
    {                                                                                                                  \
      return puple.template get<Index>();                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    template<size_t Index, typename... Types>                                                                          \
    [[nodiscard]] constexpr const std::tuple_element_t<Index, PupleType<Types...>>& get(                               \
      const PupleType<Types...>& puple) noexcept                                                                       \
    {                                                                                                                  \
      return puple.template get<Index>();                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    template<size_t Index, typename... Types>                                                                          \
    [[nodiscard]] constexpr std::tuple_element_t<Index, PupleType<Types...>>& get(                                     \
      PupleType<Types...>&& puple) noexcept                                                                            \
    {                                                                                                                  \
      return puple.template get<Index>();                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    template<size_t Index, typename... Types>                                                                          \
    [[nodiscard]] constexpr const std::tuple_element_t<Index, PupleType<Types...>>& get(                               \
      const PupleType<Types...>&& puple) noexcept                                                                      \
    {                                                                                                                  \
      return puple.template get<Index>();                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    template<typename Type, typename... Types>                                                                         \
    [[nodiscard]] constexpr Type& get(PupleType<Types...>& puple) noexcept                                             \
    {                                                                                                                  \
      return puple.template get<Type>();                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    template<typename Type, typename... Types>                                                                         \
    [[nodiscard]] constexpr const Type& get(const PupleType<Types...>& puple) noexcept                                 \
    {                                                                                                                  \
      return puple.template get<Type>();                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    template<typename Type, typename... Types>                                                                         \
    [[nodiscard]] constexpr Type& get(PupleType<Types...>&& puple) noexcept                                            \
    {                                                                                                                  \
      return puple.template get<Type>();                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    template<typename Type, typename... Types>                                                                         \
    [[nodiscard]] constexpr const Type& get(const PupleType<Types...>&& puple) noexcept                                \
    {                                                                                                                  \
      return puple.template get<Type>();                                                                               \
    }                                                                                                                  \
  } // namespace std

DEFINE_PUPLE_LIKE(::plex::Puple)

#endif
