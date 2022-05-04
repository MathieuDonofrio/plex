#ifndef GENEBITS_ENGINE_MATH_FUNCS_COMMON_H
#define GENEBITS_ENGINE_MATH_FUNCS_COMMON_H

#include "genebits/engine/config/compiler.h"

#include <bit>
#include <cmath>
#include <type_traits>

#ifdef ISA_X86
#include <immintrin.h>
#endif

namespace genebits::engine
{
// Body generation

#define MATH_GEN(crt, constexpr_crt, ...)                                \
  noexcept                                                               \
  {                                                                      \
    if (std::is_constant_evaluated()) return constexpr_crt(__VA_ARGS__); \
    else                                                                 \
      return crt(__VA_ARGS__);                                           \
  }

namespace details
{
  constexpr int NearInt(float x) noexcept
  {
    return static_cast<int>(x);
  }

  constexpr long NearInt(double x) noexcept
  {
    return static_cast<long>(x);
  }

  constexpr long long NearInt(long double x) noexcept
  {
    return static_cast<long long>(x);
  }

  template<typename T>
  constexpr bool IsNear(T x, T y) noexcept
  {
    T diff = x - y;
    if (diff < 0) diff = -diff;

    return diff <= std::numeric_limits<T>::epsilon();
  }

  constexpr float SqrtApprox(float x)
  {
    auto i = std::bit_cast<uint32_t>(x);
    i -= 1 << 23;
    i >>= 1;
    i += 1 << 29;
    return std::bit_cast<float>(i);
  }

  constexpr double SqrtApprox(double x) noexcept
  {
    auto i = std::bit_cast<uint64_t>(x);
    i -= 1ULL << 52;
    i >>= 1;
    i += 1ULL << 61;
    return std::bit_cast<double>(i);
  }

  constexpr long double SqrtApprox(long double x) noexcept
  {
    return SqrtApprox(static_cast<double>(x));
  }

  template<typename T, size_t MaxIterations>
  constexpr T SqrtBabylonian(T x) noexcept
  {
    T current = SqrtApprox(x); // Initial guess

    current = T(0.5) * (current + x / current);
    current = T(0.5) * (current + x / current);
    current = T(0.5) * (current + x / current);
    current = T(0.5) * (current + x / current);

    return current;
  }
} // namespace details

namespace ctmath
{
  // Math functions made to be run at compile-time

  template<typename T>
  constexpr T Abs(T x) noexcept
  {
    return x < 0 ? -x : x;
  }

  template<typename T>
  constexpr T Floor(T x) noexcept
  {
    const auto i = details::NearInt(x);
    if (i > x) return static_cast<T>(i - 1);
    else
      return static_cast<T>(i);
  }

  template<typename T>
  constexpr auto Ceil(T x) noexcept
  {
    const auto i = details::NearInt(x);
    if (i < x) return static_cast<T>(i + 1);
    else
      return static_cast<T>(i);
  }

  template<typename T>
  constexpr auto Round(T x) noexcept
  {
    if (x < 0) return static_cast<T>(details::NearInt(x - T(0.5)));
    else
      return static_cast<T>(details::NearInt(x + T(0.5)));
  }

  template<typename T>
  constexpr T Sqrt(T x) noexcept
  {
    if (x == 0) return 0;
    if (x < 0 || x >= std::numeric_limits<T>::infinity()) return std::numeric_limits<T>::quiet_NaN();

    return details::SqrtBabylonian<T, 4>(x);
  }

  template<typename T>
  constexpr T RSqrt(T x) noexcept
  {
    return x == 0 ? std::numeric_limits<T>::infinity() : T(1) / Sqrt(x);
  }
} // namespace ctmath

namespace rtmath
{
  // Math functions made to be run at runtime and maybe compile-time if declared constexpr.

  template<typename T>
  constexpr T Max(T x, T y) noexcept
  {
    return x > y ? x : y;
  }

  template<typename T>
  constexpr T Min(T x, T y) noexcept
  {
    return x < y ? x : y;
  }

  template<typename T, std::unsigned_integral U>
  constexpr T Pow(T base, U exp) noexcept
  {
    T result = 1;
    while (exp)
    {
      if (exp & 1) result *= base;
      exp >>= 1;
      base *= base;
    }
    return result;
  }

  template<typename T, std::signed_integral U>
  constexpr T Pow(T base, U exp) noexcept
  {
    using UT = std::make_unsigned_t<U>;
    if (exp < 0) return T(1) / Pow(base, static_cast<UT>(-exp));
    else
      return Pow(base, static_cast<UT>(exp));
  }

  inline float Sqrt(float x) noexcept
  {
#ifdef ISA_SSSE3
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ps1(x)));
#else
    return std::sqrtf(x);
#endif
  }

  inline double Sqrt(double x) noexcept
  {
#ifdef ISA_SSSE3
    return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_set_sd(x), _mm_set_sd(0)));
#else
    return std::sqrt(x);
#endif
  }

  inline long double Sqrt(long double x) noexcept
  {
    return Sqrt(static_cast<float>(x));
  }

  inline float RSqrt(float x) noexcept
  {
#ifdef ISA_SSSE3
    return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ps1(x))); // Slightly more accurate and faster
#else
    // auto y = std::bit_cast<float>(0x5F3759DF - (std::bit_cast<std::uint32_t>(x) >> 1));
    // return y * (1.5f - (x * 0.5f * y * y));

    // This version is slightly more accurate than the above because the constants have been fine-tuned.
    auto y = std::bit_cast<float>(0x5F1FFFF9 - (std::bit_cast<std::uint32_t>(x) >> 1));
    return y * (0.703952253f * (2.38924456f - x * y * y));
#endif
  }

  inline double RSqrt(double x) noexcept
  {
#ifdef ISA_SSSE3
    return 1.0 / Sqrt(x); // Already pretty fast on x86
#else
    const double halfx = x * 0.5;

    // auto y = std::bit_cast<double>(0x5FE6EC85E7DE30D9 - (std::bit_cast<std::uint64_t>(x) >> 1));
    auto y = std::bit_cast<double>(0x5FE6EB50C7B537A9 - (std::bit_cast<std::uint64_t>(x) >> 1));

    y = y * (1.5 - (halfx * y * y));
    y = y * (1.5 - (halfx * y * y)); // Second iteration for better precision for doubles

    return y;
#endif
  }

  inline long double RSqrt(long double x) noexcept
  {
    return RSqrt(static_cast<double>(x));
  }
} // namespace rtmath

// clang-format off

//
// Basic operations
//

// Absolute value

constexpr int Abs(int x) MATH_GEN(std::abs, ctmath::Abs, x);
constexpr long Abs(long x) MATH_GEN(std::labs, ctmath::Abs, x);
constexpr long long Abs(long long x) MATH_GEN(std::llabs, ctmath::Abs, x);
constexpr float Abs(float x) MATH_GEN(std::fabsf, ctmath::Abs, x);
constexpr double Abs(double x) MATH_GEN(std::fabs, ctmath::Abs, x);
constexpr long double Abs(long double x) MATH_GEN(std::fabsl, ctmath::Abs, x);

// Maximum

template<typename T> constexpr T Max(T x, T y) noexcept { return rtmath::Max(x, y); }

// Minimum

template<typename T> constexpr T Min(T x, T y) noexcept { return rtmath::Min(x, y); }

// Ceiling

constexpr float Ceil(float x) MATH_GEN(std::ceilf, ctmath::Ceil, x);
constexpr double Ceil(double x) MATH_GEN(std::ceil, ctmath::Ceil, x);
constexpr long double Ceil(long double x) MATH_GEN(std::ceill, ctmath::Ceil, x);

// Floor

constexpr float Floor(float x) MATH_GEN(std::floorf, ctmath::Floor, x);
constexpr double Floor(double x) MATH_GEN(std::floor, ctmath::Floor, x);
constexpr long double Floor(long double x) MATH_GEN(std::floorl, ctmath::Floor, x);

// Round

constexpr float Round(float x) MATH_GEN(std::roundf, ctmath::Round, x);
constexpr double Round(double x) MATH_GEN(std::round, ctmath::Round, x);
constexpr long double Round(long double x) MATH_GEN(std::roundl, ctmath::Round, x);

//
// Exponential functions
//

// Exp

inline float Exp(float x) noexcept { return std::expf(x); };
inline double Exp(double x) noexcept { return std::exp(x); };
inline long double Exp(long double x) noexcept { return std::expl(x); };

// Exp2

inline float Exp2(float x) noexcept { return std::exp2f(x); }
inline double Exp2(double x) noexcept { return std::exp2(x); }
inline long double Exp2(long double x) noexcept { return std::exp2l(x); }

// Log

inline float Log(float x) noexcept { return std::logf(x); }
inline double Log(double x) noexcept { return std::log(x); }
inline long double Log(long double x) noexcept { return std::logl(x); }

// Log2

inline float Log2(float x) noexcept { return std::log2f(x); }
inline double Log2(double x) noexcept { return std::log2(x); }
inline long double Log2(long double x) noexcept { return std::log2l(x); }

//
// Power functions
//

// Power

template<typename T, std::integral U> constexpr T Pow(T base, U exp) noexcept { return rtmath::Pow(base, exp); }

inline float Pow(float base, float exp) noexcept { return std::powf(base, exp); }
inline double Pow(double base, double exp) noexcept { return std::pow(base, exp); }
inline long double Pow(long double base, long double exp) noexcept { return std::powl(base, exp); }

// Square Root

constexpr float Sqrt(float x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);
constexpr double Sqrt(double x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);
constexpr long double Sqrt(long double x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);

// Approximate Reciprocal Square Root

constexpr float RSqrt(float x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);
constexpr double RSqrt(double x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);
constexpr long double RSqrt(long double x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);

//
// Trigonometric functions
//

// Sin

inline float Sin(float x) noexcept { return std::sinf(x); }
inline double Sin(double x) noexcept { return std::sin(x); }
inline long double Sin(long double x) noexcept { return std::sinl(x); }

// Cos

inline float Cos(float x) noexcept { return std::cosf(x); }
inline double Cos(double x) noexcept { return std::cos(x); }
inline long double Cos(long double x) noexcept { return std::cosl(x); }

// Tan

inline float Tan(float x) noexcept { return std::tanf(x); }
inline double Tan(double x) noexcept { return std::tan(x); }
inline long double Tan(long double x) noexcept { return std::tanl(x); }

// Arc Sin

inline float ASin(float x) noexcept { return std::asinf(x); }
inline double ASin(double x) noexcept { return std::asin(x); }
inline long double ASin(long double x) noexcept { return std::asinl(x); }

// Arc Cos

inline float ACos(float x) noexcept { return std::acosf(x); }
inline double ACos(double x) noexcept { return std::acos(x); }
inline long double ACos(long double x) noexcept { return std::acosl(x); }

// Arc Tan

inline float ATan(float x) noexcept { return std::atanf(x); }
inline double ATan(double x) noexcept { return std::atan(x); }
inline long double ATan(long double x) noexcept { return std::atanl(x); }

inline float ATan(float y, float x) noexcept { return std::atan2f(y, x); }
inline double ATan(double y, double x) noexcept { return std::atan2(y, x); }
inline long double ATan(long double y, long double x) noexcept { return std::atan2l(y, x); }

// clang-format on

#undef MATH_GEN
} // namespace genebits::engine

#endif
