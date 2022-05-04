#ifndef GENEBITS_ENGINE_MATH_FUNCS_VEC_H
#define GENEBITS_ENGINE_MATH_FUNCS_VEC_H

#include "genebits/engine/math/funcs_common.h"
#include "genebits/engine/math/vec2.h"
#include "genebits/engine/math/vec3.h"
#include "genebits/engine/math/vec4.h"

namespace genebits::engine
{
#define VEC_UNROLLED_LOOP_ITERATION(L, I, expression) \
  if constexpr (L > I)                                \
  {                                                   \
    const constexpr size_t i = I;                     \
    expression;                                       \
  }

#define VEC_UNROLLED_LOOP(L, expression)        \
  VEC_UNROLLED_LOOP_ITERATION(L, 0, expression) \
  VEC_UNROLLED_LOOP_ITERATION(L, 1, expression) \
  VEC_UNROLLED_LOOP_ITERATION(L, 2, expression) \
  VEC_UNROLLED_LOOP_ITERATION(L, 3, expression)

template<typename T, size_t L>
constexpr Vec<T, L> Abs(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Abs(v[i]));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Max(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Max(v1[i], v2[i]));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Min(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Min(v1[i], v2[i]));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Ceil(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Ceil(v[i]));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Floor(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Floor(v[i]));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Round(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Round(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Exp(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Exp(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Exp2(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Exp2(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Log(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Log(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Log2(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Log2(v[i]));
  return result;
}

template<typename T, std::integral U, size_t L>
constexpr Vec<T, L> Pow(const Vec<T, L>& bases, const Vec<int, L>& exponents)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exponents[i]));
  return result;
}

template<typename T, std::integral U, size_t L>
constexpr Vec<T, L> Pow(const Vec<T, L>& bases, U exp)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exp));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Pow(const Vec<T, L>& bases, const Vec<T, L>& exponents)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exponents[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Pow(const Vec<T, L>& bases, T exp)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Pow(bases[i], exp));
  return result;
}

template<typename T, size_t L>
constexpr Vec<T, L> Sqrt(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Sqrt(v[i]));
  return result;
}

#ifdef ISA_SSE3
template<size_t L>
constexpr Vec<float, L> Sqrt(const Vec<float, L>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<float, L> result;
    VEC_UNROLLED_LOOP(L, result[i] = Sqrt(v[i]));
    return result;
  }
  else
  {
    alignas(32) Vec<float, L> result;
    _mm_store_ps(result.data, _mm_sqrt_ps(_mm_load_ps(v.data)));
    return result;
  }
}
#endif

#ifdef ISA_SSE3
template<size_t L>
constexpr Vec<double, L> Sqrt(const Vec<double, L>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<float, L> result;
    VEC_UNROLLED_LOOP(L, result[i] = Sqrt(v[i]));
    return result;
  }
  else
  {
#ifdef ISA_AVX
    if constexpr (L > 2)
    {
      alignas(64) Vec<double, L> result;
      _mm256_store_ps(result.data, _mm256_sqrt_pd(_mm256_load_ps(v.data)));
      return result;
    }
    else
#endif
    {
      alignas(32) Vec<double, L> result;
      _mm_store_pd(result.data, _mm_sqrt_pd(_mm_load_pd(v.data)));
      return result;
    }
  }
}
#endif

template<typename T, size_t L>
constexpr Vec<T, L> RSqrt(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = RSqrt(v[i]));
  return result;
}

#ifdef ISA_SSE3
template<size_t L>
constexpr Vec<float, L> RSqrt(const Vec<float, 4>& v)
{
  if (std::is_constant_evaluated())
  {
    Vec<float, L> result;
    VEC_UNROLLED_LOOP(L, result[i] = RSqrt(v[i]));
    return result;
  }
  else
  {
    alignas(32) Vec<float, L> result;
    _mm_store_ps(result.data, _mm_rsqrt_ps(_mm_load_ps(v.data)));
    return result;
  }
}
#endif

template<typename T, size_t L>
inline Vec<T, L> Sin(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Sin(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Cos(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Cos(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> Tan(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = Tan(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> ASin(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ASin(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> ACos(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ACos(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> ATan(const Vec<T, L>& v)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ATan(v[i]));
  return result;
}

template<typename T, size_t L>
inline Vec<T, L> ATan(const Vec<T, L>& v1, const Vec<T, L>& v2)
{
  Vec<T, L> result;
  VEC_UNROLLED_LOOP(L, result[i] = ATan(v1[i], v2[i]));
  return result;
}

#undef VEC_UNROLLED_LOOP
#undef VEC_UNROLLED_LOOP_ITERATION

} // namespace genebits::engine

#endif
