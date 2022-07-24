#include "genebits/engine/math/funcs_common.h"

#include <gtest/gtest.h>

#include <array>

namespace plex::tests
{
namespace
{
  template<size_t Amount, typename T, typename UnaryFunction>
  constexpr std::array<T, Amount> Generate(T start, T step, UnaryFunction function)
  {
    std::array<T, Amount> result;

    for (size_t i = 0; i < Amount; ++i)
    {
      result[i] = function(start);
      start += step;
    }

    return result;
  }

  template<typename T, typename Iterator>
  void ExpectEQRange(Iterator begin, Iterator otherBegin, Iterator end)
  {
    for (Iterator it = begin; it != end; ++it, ++otherBegin)
    {
      if constexpr (std::is_same_v<T, float>) EXPECT_FLOAT_EQ(*it, *otherBegin);
      else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long double>)
      {
        EXPECT_DOUBLE_EQ(*it, *otherBegin);
      }
      else
      {
        EXPECT_EQ(*it, *otherBegin);
      }
    }
  }

  template<std::floating_point T, typename Iterator>
  void ExpectNearRange(Iterator begin, Iterator otherBegin, Iterator end, T tolerance = 0.00001)
  {
    for (Iterator it = begin; it != end; ++it, ++otherBegin)
    {
      if (std::isnan(*it))
      {
        EXPECT_LT(*otherBegin, 0.0001);
      }
      else if (std::isinf(*it))
      {
        EXPECT_GT(*otherBegin, 9007199254740992 * 0.9999);
      }
      else
      {
        T error = Abs(1 - *it / *otherBegin);
        EXPECT_LE(error, tolerance); // Allow 0.07% error
      }
    }
  }
} // namespace

TEST(Func_Common_Tests, Abs_Int_Positive)
{
  constexpr int ct_value = Abs(10);
  int rt_value = Abs(10);

  EXPECT_EQ(10, rt_value);
  EXPECT_EQ(10, ct_value);
}

TEST(Func_Common_Tests, Abs_Int_Negative)
{
  constexpr int ct_value = Abs(-99);
  int rt_value = Abs(-99);

  EXPECT_EQ(99, rt_value);
  EXPECT_EQ(99, ct_value);
}

TEST(Func_Common_Tests, Abs_Float_Positive)
{
  constexpr float ct_value = Abs(3.0f);
  float rt_value = Abs(3.0f);

  EXPECT_EQ(3.0, rt_value);
  EXPECT_EQ(3.0, ct_value);
}

TEST(Func_Common_Tests, Abs_Float_Negative)
{
  constexpr float ct_value = Abs(-3.0f);
  float rt_value = Abs(-3.0f);

  EXPECT_EQ(3.0f, rt_value);
  EXPECT_EQ(3.0f, ct_value);
}

TEST(Func_Common_Tests, Abs_Float_NegativeZero)
{
  constexpr float ct_value = Abs(-0.0f);
  float rt_value = Abs(-0.0f);

  EXPECT_EQ(0.0f, rt_value);
  EXPECT_EQ(0.0f, ct_value);
}

TEST(Func_Common_Tests, Abs_Float_NegativeInf)
{
  constexpr float ct_value = Abs(-INFINITY);
  float rt_value = Abs(-INFINITY);

  EXPECT_EQ(INFINITY, rt_value);
  EXPECT_EQ(INFINITY, ct_value);
}

TEST(Func_Common_Tests, Max_Int)
{
  int value1 = Max(10, 20);
  int value2 = Max(20, 10);

  EXPECT_EQ(20, value1);
  EXPECT_EQ(20, value2);
}

TEST(Func_Common_Tests, Min_Int)
{
  int value1 = Min(10, 20);
  int value2 = Min(20, 10);

  EXPECT_EQ(10, value1);
  EXPECT_EQ(10, value2);
}

TEST(Func_Common_Tests, Ceil_Float_Positive)
{
  constexpr float ct_value = Ceil(2.4f);
  float rt_value = Ceil(2.4f);

  EXPECT_EQ(3.0f, rt_value);
  EXPECT_EQ(3.0f, ct_value);
}

TEST(Func_Common_Tests, Ceil_Float_Negative)
{
  constexpr float ct_value = Ceil(-2.4f);
  float rt_value = Ceil(-2.4f);

  EXPECT_EQ(-2.0f, rt_value);
  EXPECT_EQ(-2.0f, ct_value);
}

TEST(Func_Common_Tests, Ceil_Float_NegativeZero)
{
  constexpr float ct_value = Ceil(-0.0f);
  float rt_value = Ceil(-0.0f);

  EXPECT_EQ(-0.0f, rt_value);
  EXPECT_EQ(-0.0f, ct_value);
}

TEST(Func_Common_Tests, Floor_Float_Positive)
{
  constexpr float ct_value = Floor(2.7f);
  float rt_value = Floor(2.7f);

  EXPECT_EQ(2.0f, rt_value);
  EXPECT_EQ(2.0f, ct_value);
}

TEST(Func_Common_Tests, Floor_Float_Negative)
{
  constexpr float ct_value = Floor(-2.7f);
  float rt_value = Floor(-2.7f);

  EXPECT_EQ(-3.0f, rt_value);
  EXPECT_EQ(-3.0f, ct_value);
}

TEST(Func_Common_Tests, Floor_Float_NegativeZero)
{
  constexpr float ct_value = Floor(-0.0f);
  float rt_value = Floor(-0.0f);

  EXPECT_EQ(-0.0f, rt_value);
  EXPECT_EQ(-0.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_PositiveLow)
{
  constexpr float ct_value = Round(2.3f);
  float rt_value = Round(2.3f);

  EXPECT_EQ(2.0f, rt_value);
  EXPECT_EQ(2.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_PositiveMiddle)
{
  constexpr float ct_value = Round(2.5f);
  float rt_value = Round(2.5f);

  EXPECT_EQ(3.0f, rt_value);
  EXPECT_EQ(3.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_PositiveHigh)
{
  constexpr float ct_value = Round(2.7f);
  float rt_value = Round(2.7f);

  EXPECT_EQ(3.0f, rt_value);
  EXPECT_EQ(3.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_NegativeLow)
{
  constexpr float ct_value = Round(-2.3f);
  float rt_value = Round(-2.3f);

  EXPECT_EQ(-2.0f, rt_value);
  EXPECT_EQ(-2.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_NegativeMiddle)
{
  constexpr float ct_value = Round(-2.5f);
  float rt_value = Round(-2.5f);

  EXPECT_EQ(-3.0f, rt_value);
  EXPECT_EQ(-3.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_NegativeHigh)
{
  constexpr float ct_value = Round(-2.7f);
  float rt_value = Round(-2.7f);

  EXPECT_EQ(-3.0f, rt_value);
  EXPECT_EQ(-3.0f, ct_value);
}

TEST(Func_Common_Tests, Round_Float_NegativeZero)
{
  constexpr float ct_value = Round(-0.0f);
  float rt_value = Round(-0.0f);

  EXPECT_EQ(-0.0f, rt_value);
  EXPECT_EQ(-0.0f, ct_value);
}

TEST(Func_Common_Tests, Pow_Float_Int_Unsigned)
{
  float value = Pow(1.5f, 3);

  EXPECT_EQ(1.5f * 1.5f * 1.5f, value);
}

TEST(Func_Common_Tests, Pow_Float_Int_Signed)
{
  float value = Pow(1.5f, -3);

  EXPECT_EQ(1 / (1.5f * 1.5f * 1.5f), value);
}

TEST(Func_Common_Tests, Sqrt_Float)
{
  for (float i = 0; i < 1000; i += 0.5f)
  {
    EXPECT_EQ(Sqrt(i), std::sqrtf(i));
  }
}

TEST(Func_Common_Tests, Sqrt_Double)
{
  for (double i = 0; i < 1000; i += 0.5f)
  {
    EXPECT_EQ(Sqrt(i), std::sqrt(i));
  }
}

TEST(Func_Common_Tests, Sqrt_Float_CTEqRT)
{
  constexpr auto ct_values = Generate<1000>(0.0f, 0.5f, [](float x) { return Sqrt(x); });
  auto rt_values = Generate<1000>(0.0f, 0.5f, [](float x) { return Sqrt(x); });

  ExpectEQRange<float>(ct_values.begin(), rt_values.cbegin(), ct_values.end());
}

TEST(Func_Common_Tests, Sqrt_Double_CTEqRT)
{
  constexpr auto ct_values = Generate<1000>(0.0, 0.5, [](double x) { return Sqrt(x); });
  auto rt_values = Generate<1000>(0.0, 0.5, [](double x) { return Sqrt(x); });

  ExpectEQRange<double>(ct_values.begin(), rt_values.cbegin(), ct_values.end());
}

TEST(Func_Common_Tests, RSqrt_Float_CTEqRT)
{
  constexpr auto ct_values = Generate<1000>(0.0f, 0.5f, [](float x) { return RSqrt(x); });
  auto rt_values = Generate<1000>(0.0f, 0.5f, [](float x) { return RSqrt(x); });

  ExpectNearRange<float>(ct_values.begin(), rt_values.cbegin(), ct_values.end(), 0.007f);
}

TEST(Func_Common_Tests, RSqrt_Double_CTEqRT)
{
  constexpr auto ct_values = Generate<1000>(0.0, 0.5, [](double x) { return RSqrt(x); });
  auto rt_values = Generate<1000>(0.0, 0.5, [](double x) { return RSqrt(x); });

  ExpectNearRange<double>(ct_values.begin(), rt_values.cbegin(), ct_values.end(), 0.00001);
}

TEST(Func_Common_Tests, RSqrt_Float_SmallAccuracy)
{
  for (float x = 0.01f; x < 10.0f; x += 0.01f)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    float error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.0007); // Allow 0.07% error

    EXPECT_NEAR(value1, 1.0f / value2, 0.06f); // Extra
  }
}

TEST(Func_Common_Tests, RSqrt_Float_MediumAccuracy)
{
  for (float x = 0.1f; x < 10000.0f; x += 0.1f)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    float error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.0007); // Allow 0.07% error

    EXPECT_NEAR(value1, 1.0f / value2, 0.01f); // Extra
  }
}

TEST(Func_Common_Tests, RSqrt_Float_LargeAccuracy)
{
  for (float x = 10; x < 1000000.0f; x += 10)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    float error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.0007); // Allow 0.07% error

    EXPECT_NEAR(value1, 1.0f / value2, 0.01f); // Extra
  }
}

TEST(Func_Common_Tests, RSqrt_Double_SmallAccuracy)
{
  for (double x = 0.01; x < 10.0; x += 0.01)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    double error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.00001); // Allow 0.001% error

    EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
  }
}

TEST(Func_Common_Tests, RSqrt_Double_MediumAccuracy)
{
  for (double x = 0.1; x < 10000.0; x += 0.1)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    double error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.00001); // Allow 0.001% error

    EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
  }
}

TEST(Func_Common_Tests, RSqrt_Double_LargeAccuracy)
{
  for (double x = 10; x < 1000000.0; x += 10)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    double error = Abs(1 - value1 * value2);
    EXPECT_LE(error, 0.00001); // Allow 0.001% error

    EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
  }
}

} // namespace plex::tests