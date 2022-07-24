#ifndef PLEX_MATH_FUNCS_VEC_H
#define PLEX_MATH_FUNCS_VEC_H

#include "genebits/engine/math/funcs_common.h"
#include "genebits/engine/math/vec2.h"
#include "genebits/engine/math/vec3.h"
#include "genebits/engine/math/vec4.h"

namespace plex
{
#define VEC_UNROLLED_LOOP_ITERATION(L, I, expression) \
  if constexpr ((L) > (I))                            \
  {                                                   \
    const constexpr size_t i = (I);                   \
    expression;                                       \
  }

#define VEC_UNROLLED_LOOP(L, expression)          \
  VEC_UNROLLED_LOOP_ITERATION((L), 0, expression) \
  VEC_UNROLLED_LOOP_ITERATION((L), 1, expression) \
  VEC_UNROLLED_LOOP_ITERATION((L), 2, expression) \
  VEC_UNROLLED_LOOP_ITERATION((L), 3, expression)

///
/// Returns the absolute value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the absolute value of.
///
/// @return The absolute value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Abs(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Abs(v[i]));
  return result;
}

///
/// Returns the maximum value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the maximum value of.
///
/// @return The maximum value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Max(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Max(v1[i], v2[i]));
  return result;
}

///
/// Returns the minimum value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the minimum value of.
///
/// @return The minimum value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Min(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Min(v1[i], v2[i]));
  return result;
}

///
/// Returns the ceiled value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the ceiled value of.
///
/// @return The ceiled value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Ceil(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Ceil(v[i]));
  return result;
}

///
/// Returns the floored value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the floored value of.
///
/// @return The floored value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Floor(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Floor(v[i]));
  return result;
}

///
/// Returns the rounded value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the rounded value of.
///
/// @return The rounded value of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Round(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Round(v[i]));
  return result;
}

///
/// Returns the exponentiated value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the exponentiated value of.
///
/// @return The exponentiated value of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Exp(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Exp(v[i]));
  return result;
}

///
/// Returns the 2-exponentiated value of all components of the vector-type.
///
/// @param[in] x The vector-type to get the 2-exponentiated value of.
///
/// @return The 2-exponentiated value of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Exp2(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Exp2(v[i]));
  return result;
}

///
/// Returns the natural logarithm of all components of the vector-type.
///
/// @param[in] x The vector-type to get the natural logarithm of.
///
/// @return The natural logarithm of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Log(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Log(v[i]));
  return result;
}

///
/// Returns the base-2 logarithm of all components of the vector-type.
///
/// @param[in] x The vector-type to get the base-2 logarithm of.
///
/// @return The base-2 logarithm of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Log2(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Log2(v[i]));
  return result;
}

///
/// Returns the base-10 logarithm of all components of the vector-type.
///
/// @param[in] x The vector-type to get the base-2 logarithm of.
///
/// @return The base-10 logarithm of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Log10(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Log10(v[i]));
  return result;
}

///
/// Returns the base raised to the exponent of all components of the bases vector-type and the exponents vector-type.
///
/// @param[in] bases The vector-type to get the base raised to the exponent of.
/// @param[in] exponents The vector-type to get the exponent of.
///
/// @return The base raised to the exponent of all components of the bases vector-type and the exponents vector-type.
///
template<typename T, std::integral U, size_t L>
constexpr Vec<T, L> Pow(const Vec<T, L>& bases, const Vec<int, L>& exponents)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exponents[i]));
  return result;
}

///
/// Returns the base raised to the exponent of all components of the bases vector-type and exponent.
///
/// @param[in] bases The vector-type to get the base raised to the exponent of.
/// @param[in] exp The exponent.
///
/// @return The base raised to the exponent of all components of the bases vector-type and exponent.
///
template<typename T, std::integral U, size_t L>
constexpr Vec<T, L> Pow(const Vec<T, L>& bases, U exp)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exp));
  return result;
}

///
/// Returns the base raised to the exponent of all components of the bases vector-type and the exponents vector-type.
///
/// @param[in] bases The vector-type to get the base raised to the exponent of.
/// @param[in] exponents The vector-type to get the exponent of.
///
/// @return The base raised to the exponent of all components of the bases vector-type and the exponents vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Pow(const Vec<T, L>& bases, const Vec<T, L>& exponents)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exponents[i]));
  return result;
}

///
/// Returns the base raised to the exponent of all components of the bases vector-type and exponent.
///
/// @param[in] bases The vector-type to get the base raised to the exponent of.
/// @param[in] exp The exponent.
///
/// @return The base raised to the exponent of all components of the bases vector-type and exponent.
///
template<typename T, size_t L>
inline Vec<T, L> Pow(const Vec<T, L>& bases, T exp)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exp));
  return result;
}

///
/// Returns the square root of all components of the vector-type.
///
/// @param[in] x The vector-type to get the square root of.
///
/// @return The square root of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> Sqrt(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Sqrt(v[i]));
  return result;
}

#ifdef ISA_SSE3
template<>
constexpr Vec<float, 4> Sqrt(const Vec<float, 4>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<float, 4> result;
    VEC_UNROLLED_LOOP(4, result[i] = Sqrt(v[i]));
    return result;
  }
  else
  {
    Vec<float, 4> result;
    _mm_store_ps(result.data(), _mm_sqrt_ps(_mm_load_ps(v.data())));
    return result;
  }
}
#endif

#ifdef ISA_SSE3
template<size_t L>
requires(L == 2 || L == 4)
constexpr Vec<double, L> Sqrt(const Vec<double, L>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<double, L> result;
    VEC_UNROLLED_LOOP(L, result[i] = Sqrt(v[i]));
    return result;
  }
  else
  {
#ifdef ISA_AVX
    if constexpr (L == 4)
    {
      Vec<double, 4> result;
      _mm256_store_pd(result.data(), _mm256_sqrt_pd(_mm256_load_pd(v.data())));
      return result;
    }
    else
#endif
    {
      alignas(16) Vec<double, L> result;
      _mm_store_pd(result.data(), _mm_sqrt_pd(_mm_load_pd(v.data())));
      if constexpr (L == 4) _mm_store_pd(result.data() + 2, _mm_sqrt_pd(_mm_load_pd(v.data() + 2)));
      return result;
    }
  }
}
#endif

///
/// Returns the approximate reciprocal square root of all components of the vector-type.
///
/// @param[in] x The vector-type to get the reciprocal square root of.
///
/// @return The approximate reciprocal square root of all components of the vector-type.
///
template<typename T, size_t L>
constexpr Vec<T, L> RSqrt(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = RSqrt(v[i]));
  return result;
}

#ifdef ISA_SSE3
template<>
constexpr Vec<float, 4> RSqrt(const Vec<float, 4>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<float, 4> result;
    VEC_UNROLLED_LOOP(4, result[i] = RSqrt(v[i]));
    return result;
  }
  else
  {
    Vec<float, 4> result;
    _mm_store_ps(result.data(), _mm_rsqrt_ps(_mm_load_ps(v.data())));
    return result;
  }
}
#endif

///
/// Returns the sin of all components of the vector-type.
///
/// @param[in] x The vector-type to get the sin of.
///
/// @return The sin of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Sin(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Sin(v[i]));
  return result;
}

///
/// Returns the cos of all components of the vector-type.
///
/// @param[in] x The vector-type to get the cos of.
///
/// @return The cos of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Cos(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Cos(v[i]));
  return result;
}

///
/// Returns the tan of all components of the vector-type.
///
/// @param[in] x The vector-type to get the tan of.
///
/// @return The tan of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> Tan(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Tan(v[i]));
  return result;
}

///
/// Returns the arcsin of all components of the vector-type.
///
/// @param[in] x The vector-type to get the arcsin of.
///
/// @return The arcsin of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> ASin(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ASin(v[i]));
  return result;
}

///
/// Returns the arccos of all components of the vector-type.
///
/// @param[in] x The vector-type to get the arccos of.
///
/// @return The arccos of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> ACos(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ACos(v[i]));
  return result;
}

///
/// Returns the arctan of all components of the vector-type.
///
/// @param[in] x The vector-type to get the arctan of.
///
/// @return The arctan of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> ATan(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ATan(v[i]));
  return result;
}

///
/// Returns the arctan2 of all components of the vector-type.
///
/// @param[in] x The vector-type to get the arctan2 of.
///
/// @return The arctan2 of all components of the vector-type.
///
template<typename T, size_t L>
inline Vec<T, L> ATan(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ATan(v1[i], v2[i]));
  return result;
}

#undef VEC_UNROLLED_LOOP
#undef VEC_UNROLLED_LOOP_ITERATION

} // namespace plex

#endif
