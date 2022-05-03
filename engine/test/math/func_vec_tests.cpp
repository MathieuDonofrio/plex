#include "genebits/engine/math/funcs_vec.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Func_Vec_Tests, RSqrt_Float_2)
{
  constexpr float2 v = { 1.0f, 2.0f };

  constexpr float2 ct_value = RSqrt(v);
  float2 rt_value = RSqrt(v);

  float2 expected = { 1.0f / Sqrt(v.x), 1.0f / Sqrt(v.y) };

  EXPECT_NEAR(ct_value.x, expected.x, 0.001f);
  EXPECT_NEAR(ct_value.y, expected.y, 0.001f);

  EXPECT_NEAR(rt_value.x, expected.x, 0.001f);
  EXPECT_NEAR(rt_value.y, expected.y, 0.001f);
}

TEST(Func_Vec_Tests, RSqrt_Float_4)
{
  constexpr float4 v = { 1.0f, 2.0f, 3.0f, 4.0f };

  constexpr float4 ct_value = RSqrt(v);
  float4 rt_value = RSqrt(v);

  float4 expected = { 1.0f / Sqrt(v.x), 1.0f / Sqrt(v.y), 1.0f / Sqrt(v.z), 1.0f / Sqrt(v.w) };

  EXPECT_NEAR(ct_value.x, expected.x, 0.001f);
  EXPECT_NEAR(ct_value.y, expected.y, 0.001f);
  EXPECT_NEAR(ct_value.z, expected.z, 0.001f);
  EXPECT_NEAR(ct_value.w, expected.w, 0.001f);

  EXPECT_NEAR(rt_value.x, expected.x, 0.001f);
  EXPECT_NEAR(rt_value.y, expected.y, 0.001f);
  EXPECT_NEAR(rt_value.z, expected.z, 0.001f);
  EXPECT_NEAR(rt_value.w, expected.w, 0.001f);
}

} // namespace genebits::engine::tests