#include "genebits/engine/math/funcs_common.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
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

TEST(Func_Common_Tests, RSqrt_Float)
{
  constexpr float ct_value = RSqrt(25.0f);
  float rt_value = RSqrt(25.0f);

  EXPECT_NEAR(1.0f / 5.0f, rt_value, 0.001f);
  EXPECT_NEAR(1.0f / 5.0f, ct_value, 0.001f);
}

TEST(Func_Common_Tests, RSqrt_Double)
{
  constexpr double ct_value = RSqrt(25.0f);
  double rt_value = RSqrt(25.0f);

  EXPECT_NEAR(1.0f / 5.0f, rt_value, 0.0001f);
  EXPECT_NEAR(1.0f / 5.0f, ct_value, 0.0001f);
}

TEST(Func_Common_Tests, RSqrt_Float_SmallAccuracy)
{
  for (float x = -10.0f; x < 10.0f; x += 0.01f)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0f / value2) && !std::isinf(value1) && !std::isinf(1.0f / value2))
    {
      float error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.0007); // Allow 0.07% error

      EXPECT_NEAR(value1, 1.0f / value2, 0.06f); // Extra
    }
  }
}

TEST(Func_Common_Tests, RSqrt_Float_MediumAccuracy)
{
  for (float x = -10000.0f; x < 10000.0f; x += 0.1f)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0f / value2) && !std::isinf(value1) && !std::isinf(1.0f / value2))
    {
      float error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.0007); // Allow 0.07% error

      EXPECT_NEAR(value1, 1.0f / value2, 0.01f); // Extra
    }
  }
}

TEST(Func_Common_Tests, RSqrt_Float_LargeAccuracy)
{
  for (float x = -1000000.0f; x < 1000000.0f; x += 10)
  {
    float value1 = RSqrt(x);
    float value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0f / value2) && !std::isinf(value1) && !std::isinf(1.0f / value2))
    {
      float error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.0007); // Allow 0.07% error

      EXPECT_NEAR(value1, 1.0f / value2, 0.01f); // Extra
    }
  }
}

TEST(Func_Common_Tests, RSqrt_Double_SmallAccuracy)
{
  for (double x = -10.0; x < 10.0; x += 0.01)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0 / value2) && !std::isinf(value1) && !std::isinf(1.0 / value2))
    {
      double error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.00001); // Allow 0.001% error

      EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
    }
  }
}

TEST(Func_Common_Tests, RSqrt_Double_MediumAccuracy)
{
  for (double x = -10000.0; x < 10000.0; x += 0.1)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0 / value2) && !std::isinf(value1) && !std::isinf(1.0 / value2))
    {
      double error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.00001); // Allow 0.001% error

      EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
    }
  }
}

TEST(Func_Common_Tests, RSqrt_Double_LargeAccuracy)
{
  for (double x = -1000000.0; x < 1000000.0; x += 10)
  {
    double value1 = RSqrt(x);
    double value2 = std::sqrt(x);

    if (!std::isnan(value1) && !std::isnan(1.0 / value2) && !std::isinf(value1) && !std::isinf(1.0 / value2))
    {
      double error = Abs(1 - value1 * value2);
      EXPECT_LE(error, 0.00001); // Allow 0.001% error

      EXPECT_NEAR(value1, 1.0 / value2, 0.001); // Extra
    }
  }
}

} // namespace genebits::engine::tests