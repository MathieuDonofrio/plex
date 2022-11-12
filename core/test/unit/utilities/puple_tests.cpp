#include "plex/utilities/puple.h"

#include <gtest/gtest.h>

namespace plex::tests
{
static_assert(std::is_trivially_copyable_v<Puple<int, float, double, bool>>, "Puple must be trivially copyable");

static_assert(
  std::is_same_v<decltype(Puple<int> {}.get_pointer<int>()), int*&>, "Puple pointer must return references");
static_assert(
  std::is_same_v<decltype(Puple<int, float> {}.get_pointer<int>()), int*&>, "Puple pointer must return references");

TEST(Puple_Tests, DefaultConstructor_DefaultState)
{
  Puple<int, float> puple {};

  EXPECT_EQ(puple.get_pointer<int>(), nullptr);
  EXPECT_EQ(puple.get_pointer<float>(), nullptr);
}

TEST(Puple_Tests, ParametricConstructor_CorrectState)
{
  int i = 2;
  float f = 0.5;

  Puple<int, float> puple { &i, &f };

  EXPECT_NE(puple.get_pointer<int>(), nullptr);
  EXPECT_NE(puple.get_pointer<float>(), nullptr);

  EXPECT_EQ(puple.get<int>(), 2);
  EXPECT_EQ(puple.get<float>(), 0.5);
}

TEST(Puple_Tests, StructuredBidings_SingleNonReference_CorrectState)
{
  int i = 2;

  Puple<int> puple { &i };

  auto [a] = puple;

  EXPECT_EQ(a, i);
}

TEST(Puple_Tests, StructuredBidings_MultipleNonReference_CorrectState)
{
  int i = 2;
  float f = 0.5;

  Puple<int, float> puple { &i, &f };

  auto [a, b] = puple;

  EXPECT_EQ(a, i);
  EXPECT_EQ(b, f);
}

TEST(Puple_Tests, StructuredBidings_SingleReference_CorrectState)
{
  int i = 2;

  Puple<int> puple { &i };

  auto& [a] = puple;

  EXPECT_EQ(a, i);
}

TEST(Puple_Tests, StructuredBidings_MultipleReference_CorrectState)
{
  int i = 2;
  float f = 0.5;

  Puple<int, float> puple { &i, &f };

  auto& [a, b] = puple;

  EXPECT_EQ(a, i);
  EXPECT_EQ(b, f);
}

} // namespace plex::tests