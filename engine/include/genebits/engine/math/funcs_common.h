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
  ///
  /// Returns an sqrt approximation of a single-precision floating-point value.
  ///
  /// Normally used as a initial guess for Newton's method.
  ///
  /// @param[in] x The value to approximate the square root of.
  ///
  /// @returns An approximation of the square root of the given value.
  ///
  constexpr float SqrtApprox(float x)
  {
    auto i = std::bit_cast<uint32_t>(x);
    i = (i >> 1) + (127u << 22);
    return std::bit_cast<float>(i);
  }

  ///
  /// Returns an sqrt approximation of a double-precision floating-point value.
  ///
  /// Normally used as a initial guess for Newton's method.
  ///
  /// @param[in] x The value to approximate the square root of.
  ///
  /// @returns An approximation of the square root of the given value.
  ///
  constexpr double SqrtApprox(double x) noexcept
  {
    auto i = std::bit_cast<uint64_t>(x);
    i = (i >> 1) + (1023ull << 51);
    return std::bit_cast<double>(i);
  }

  ///
  /// Returns an sqrt approximation of a double-precision floating-point value.
  ///
  /// Normally used as a initial guess for Newton's method.
  ///
  /// @param[in] x The value to approximate the square root of.
  ///
  /// @returns An approximation of the square root of the given value.
  ///
  constexpr long double SqrtApprox(long double x) noexcept
  {
    return SqrtApprox(static_cast<double>(x));
  }
} // namespace details

namespace ctmath
{
  // Math functions made to be run at compile-time

  ///
  /// Returns the absolute value of x.
  ///
  /// @param[in] x The value to get the absolute value of.
  ///
  /// @returns The absolute value of x.
  ///
  template<typename T>
  constexpr T Abs(T x) noexcept
  {
    return x < 0 ? -x : x;
  }

  ///
  /// Returns the largest integer less than or equal to x.
  ///
  /// @param[in] x The value to get the floor of.
  ///
  /// @returns The largest integer less than or equal to x.
  ///
  template<typename T>
  constexpr T Floor(T x) noexcept
  {
    const auto i = static_cast<long>(x);
    if (i > x) return static_cast<T>(i - 1);
    else
      return static_cast<T>(i);
  }

  ///
  /// Returns the smallest integer greater than or equal to x.
  ///
  /// @param[in] x The value to get the ceil of.
  ///
  /// @returns The smallest integer greater than or equal to x.
  ///
  template<typename T>
  constexpr auto Ceil(T x) noexcept
  {
    const auto i = static_cast<long>(x);
    if (i < x) return static_cast<T>(i + 1);
    else
      return static_cast<T>(i);
  }

  // Round

  /// Returns the nearest integer to x.
  ///
  /// @param[in] x The value to get the nearest integer of.
  ///
  /// @returns The nearest integer to x.
  ///
  template<typename T>
  constexpr T Round(T x) noexcept
  {
    if (x < 0) return static_cast<T>(static_cast<long>(x - T(0.5)));
    else
      return static_cast<T>(static_cast<long>(x + T(0.5)));
  }

  // Sqrt

  ///
  /// Returns the square root of x.
  ///
  /// Uses Newton's method to approximate the square root.
  ///
  /// @param[in] x The value to get the square root of.
  ///
  /// @returns The square root of x.
  ///
  template<typename T>
  constexpr T Sqrt(T x) noexcept
  {
    if (x < 0) return std::numeric_limits<T>::quiet_NaN();

    if (x == T(0) || x == std::numeric_limits<T>::quiet_NaN() || x == std::numeric_limits<T>::signaling_NaN()
        || x >= std::numeric_limits<T>::infinity())
    {
      return x;
    }

    const T y = x;

    x = details::SqrtApprox(x); // Initial approximation

    // Newton-Raphson iterations
    x = (x + y / x) / T(2);
    x = (x + y / x) / T(2);
    x = (x + y / x) / T(2);
    x = (x + y / x) / T(2);

    return x;
  }

  ///
  /// Returns the reciprocal square root of x.
  ///
  /// @param[in] x The value to get the reciprocal square root of.
  ///
  /// @returns The reciprocal square root of x.
  ///
  template<typename T>
  constexpr T RSqrt(T x) noexcept
  {
    if (x == 0) return std::numeric_limits<T>::infinity();

    return T(1) / Sqrt(x);
  }
} // namespace ctmath

namespace rtmath
{
  // Math functions made to be run at runtime and maybe compile-time if declared constexpr.

  ///
  /// Returns the base raised to the power of the exp.
  ///
  /// @tparam T The type of base and exp.
  ///
  /// @param[in] base The base.
  /// @param[in] exp The exponent.
  ///
  /// @return x raised to the power of y.
  ///
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

  ///
  /// Returns the base raised to the power of the exp.
  ///
  /// @tparam T The type of base and exp.
  ///
  /// @param[in] base The base.
  /// @param[in] exp The exponent.
  ///
  /// @return x raised to the power of y.
  ///
  template<typename T, std::signed_integral U>
  constexpr T Pow(T base, U exp) noexcept
  {
    using UT = std::make_unsigned_t<U>;
    if (exp < 0) return T(1) / Pow(base, static_cast<UT>(-exp));
    else
      return Pow(base, static_cast<UT>(exp));
  }

  ///
  /// Returns the single-precision square root of x.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the square root of.
  ///
  /// @return The square root of x.
  ///
  inline float Sqrt(float x) noexcept
  {
#ifdef ISA_SSSE3
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ps1(x)));
#else
    return std::sqrtf(x);
#endif
  }

  ///
  /// Returns the double-precision square root of x.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the square root of.
  ///
  /// @return The square root of x.
  ///
  inline double Sqrt(double x) noexcept
  {
#ifdef ISA_SSSE3
    return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_set_sd(0), _mm_set_sd(x)));
#else
    return std::sqrt(x);
#endif
  }

  ///
  /// Returns the double-precision square root of x.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the square root of.
  ///
  /// @return The square root of x.
  ///
  inline long double Sqrt(long double x) noexcept
  {
    return Sqrt(static_cast<float>(x));
  }

  ///
  /// Returns the single-precision approximate reciprocal square root of x. Approximation of 1/sqrt(x).
  ///
  /// The accuracy of the approximation is guaranteed to have an error less than 0.07%. For some architectures,
  /// the accuracy may be better.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the reciprocal square root of.
  ///
  /// @return The approximate reciprocal square root of x.
  ///
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

  ///
  /// Returns the double-precision approximate reciprocal square root of x. Approximation of 1/sqrt(x).
  ///
  /// The accuracy of the approximation is guaranteed to have an error less than 0.001%. For some architectures,
  /// the accuracy may be better.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the reciprocal square root of.
  ///
  /// @return The approximate reciprocal square root of x.
  ///
  inline double RSqrt(double x) noexcept
  {
#ifdef ISA_SSSE3
    return 1.0 / Sqrt(x); // Already pretty fast on x86 and accurate
#else
    const double halfx = x * 0.5;

    // auto y = std::bit_cast<double>(0x5FE6EC85E7DE30D9 - (std::bit_cast<std::uint64_t>(x) >> 1));
    auto y = std::bit_cast<double>(0x5FE6EB50C7B537A9 - (std::bit_cast<std::uint64_t>(x) >> 1));

    y = y * (1.5 - (halfx * y * y));
    y = y * (1.5 - (halfx * y * y)); // Second iteration for better precision for doubles

    return y;
#endif
  }

  ///
  /// Returns the double-precision approximate reciprocal square root of x. Approximation of 1/sqrt(x).
  ///
  /// The accuracy of the approximation is guaranteed to have an error less than 0.001%. For some architectures,
  /// the accuracy may be better.
  ///
  /// @tparam T The type of x.
  ///
  /// @param[in] x The value to take the reciprocal square root of.
  ///
  /// @return The approximate reciprocal square root of x.
  ///
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

///
/// Returns the absolute value of x.
///
/// @param[in] x The value to take the absolute value of.
///
/// @return The absolute value of x.
///
constexpr int Abs(int x) MATH_GEN(std::abs, ctmath::Abs, x);
constexpr long Abs(long x) MATH_GEN(std::labs, ctmath::Abs, x);
constexpr long long Abs(long long x) MATH_GEN(std::llabs, ctmath::Abs, x);
constexpr float Abs(float x) MATH_GEN(std::fabsf, ctmath::Abs, x);
constexpr double Abs(double x) MATH_GEN(std::fabs, ctmath::Abs, x);
constexpr long double Abs(long double x) MATH_GEN(std::fabsl, ctmath::Abs, x);

// Maximum

///
/// Returns the maximum of x and y.
///
/// @tparam T The type of x and y.
///
/// @param[in] x The first value to compare.
/// @param[in] y The second value to compare.
///
/// @return The maximum of x and y.
///
template<typename T> constexpr T Max(T x, T y) noexcept { return x > y ? x : y; }

// Minimum

///
/// Returns the minimum of x and y.
///
/// @tparam T The type of x and y.
///
/// @param[in] x The first value to compare.
/// @param[in] y The second value to compare.
///
/// @return The minimum of x and y.
///
template<typename T> constexpr T Min(T x, T y) noexcept { return x < y ? x : y; }

// Ceiling

///
/// Returns the smallest integer greater than or equal to x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the ceiling of.
///
/// @return The ceiling of x.
///
constexpr float Ceil(float x) MATH_GEN(std::ceilf, ctmath::Ceil, x);
constexpr double Ceil(double x) MATH_GEN(std::ceil, ctmath::Ceil, x);
constexpr long double Ceil(long double x) MATH_GEN(std::ceill, ctmath::Ceil, x);

// Floor

///
/// Returns the largest integer less than or equal to x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the floor of.
///
/// @return The floor of x.
///
constexpr float Floor(float x) MATH_GEN(std::floorf, ctmath::Floor, x);
constexpr double Floor(double x) MATH_GEN(std::floor, ctmath::Floor, x);
constexpr long double Floor(long double x) MATH_GEN(std::floorl, ctmath::Floor, x);

// Round

///
/// Returns the nearest integer to x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to round.
///
/// @return The rounded value.
///
constexpr float Round(float x) MATH_GEN(std::roundf, ctmath::Round, x);
constexpr double Round(double x) MATH_GEN(std::round, ctmath::Round, x);
constexpr long double Round(long double x) MATH_GEN(std::roundl, ctmath::Round, x);

//
// Exponential functions
//

// Exp

///
/// Returns e raised to the power of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to raise e to.
///
/// @return e raised to the power of x.
///
inline float Exp(float x) noexcept { return std::expf(x); };
inline double Exp(double x) noexcept { return std::exp(x); };
inline long double Exp(long double x) noexcept { return std::expl(x); };

// Exp2

///
/// Returns 2 raised to the power of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to raise 2 to.
///
/// @return 2 raised to the power of x.
///
inline float Exp2(float x) noexcept { return std::exp2f(x); }
inline double Exp2(double x) noexcept { return std::exp2(x); }
inline long double Exp2(long double x) noexcept { return std::exp2l(x); }

// Log

///
/// Returns the natural logarithm of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the logarithm of.
///
/// @return The natural logarithm of x.
///
inline float Log(float x) noexcept { return std::logf(x); }
inline double Log(double x) noexcept { return std::log(x); }
inline long double Log(long double x) noexcept { return std::logl(x); }

// Log2

///
/// Returns the base-2 logarithm of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the logarithm of.
///
/// @return The base-2 logarithm of x.
///
inline float Log2(float x) noexcept { return std::log2f(x); }
inline double Log2(double x) noexcept { return std::log2(x); }
inline long double Log2(long double x) noexcept { return std::log2l(x); }

// Log10

///
/// Returns the base-10 logarithm of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the logarithm of.
///
/// @return The base-10 logarithm of x.
///
inline float Log10(float x) noexcept { return std::log10f(x); }
inline double Log10(double x) noexcept { return std::log10(x); }
inline long double Log10(long double x) noexcept { return std::log10l(x); }

//
// Power functions
//

// Power

///
/// Returns the base raised to the power of the exp.
///
/// @tparam T The type of base and exp.
///
/// @param[in] base The base.
/// @param[in] exp The exponent.
///
/// @return x raised to the power of y.
///
template<typename T, std::integral U> constexpr T Pow(T base, U exp) noexcept { return rtmath::Pow(base, exp); }

///
/// Returns the base raised to the power of the exp.
///
/// @tparam T The type of base and exp.
///
/// @param[in] base The base.
/// @param[in] exp The exponent.
///
/// @return base raised to the power of exp.
///
inline float Pow(float base, float exp) noexcept { return std::powf(base, exp); }
inline double Pow(double base, double exp) noexcept { return std::pow(base, exp); }
inline long double Pow(long double base, long double exp) noexcept { return std::powl(base, exp); }

// Square Root

///
/// Returns the square root of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the square root of.
///
/// @return The square root of x.
///
constexpr float Sqrt(float x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);
constexpr double Sqrt(double x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);
constexpr long double Sqrt(long double x) MATH_GEN(rtmath::Sqrt, ctmath::Sqrt, x);

// Approximate Reciprocal Square Root

///
/// Returns the approximate reciprocal square root of x. Approximation of 1/sqrt(x).
///
/// The accuracy of the approximation is guaranteed to have an error less than 0.07%. For some architectures,
/// the accuracy may be better.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the reciprocal square root of.
///
/// @return The approximate reciprocal square root of x.
///
constexpr float RSqrt(float x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);
constexpr double RSqrt(double x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);
constexpr long double RSqrt(long double x) MATH_GEN(rtmath::RSqrt, ctmath::RSqrt, x);

//
// Trigonometric functions
//

// Sin

///
/// Returns the sine of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the sine of.
///
/// @return The sine of x.
///
inline float Sin(float x) noexcept { return std::sinf(x); }
inline double Sin(double x) noexcept { return std::sin(x); }
inline long double Sin(long double x) noexcept { return std::sinl(x); }

// Cos

///
/// Returns the cosine of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the cosine of.
///
/// @return The cosine of x.
///
inline float Cos(float x) noexcept { return std::cosf(x); }
inline double Cos(double x) noexcept { return std::cos(x); }
inline long double Cos(long double x) noexcept { return std::cosl(x); }

// Tan

///
/// Returns the tangent of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the tangent of.
///
/// @return The tangent of x.
///
inline float Tan(float x) noexcept { return std::tanf(x); }
inline double Tan(double x) noexcept { return std::tan(x); }
inline long double Tan(long double x) noexcept { return std::tanl(x); }

// Arc Sin

///
/// Returns the arc sine of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the arc sine of.
///
/// @return The arc sine of x.
///
inline float ASin(float x) noexcept { return std::asinf(x); }
inline double ASin(double x) noexcept { return std::asin(x); }
inline long double ASin(long double x) noexcept { return std::asinl(x); }

// Arc Cos

///
/// Returns the arc cosine of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the arc cosine of.
///
/// @return The arc cosine of x.
///
inline float ACos(float x) noexcept { return std::acosf(x); }
inline double ACos(double x) noexcept { return std::acos(x); }
inline long double ACos(long double x) noexcept { return std::acosl(x); }

// Arc Tan

///
/// Returns the arc tangent of x.
///
/// @tparam T The type of x.
///
/// @param[in] x The value to take the arc tangent of.
///
/// @return The arc tangent of x.
///
inline float ATan(float x) noexcept { return std::atanf(x); }
inline double ATan(double x) noexcept { return std::atan(x); }
inline long double ATan(long double x) noexcept { return std::atanl(x); }

///
/// Returns the arc tangent of y/x.
///
/// @tparam T The type of x.
///
/// @param[in] y The y value.
/// @param[in] x The x value.
///
/// @return The arc tangent of y/x.
///
inline float ATan(float y, float x) noexcept { return std::atan2f(y, x); }
inline double ATan(double y, double x) noexcept { return std::atan2(y, x); }
inline long double ATan(long double y, long double x) noexcept { return std::atan2l(y, x); }

// clang-format on

#undef MATH_GEN
} // namespace genebits::engine

#endif
