#include "engine/util/fast_vector.h"

#include <vector>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(FastVector_Tests, Empty_Trivial_AfterDefaultConstruction_True)
{
  FastVector<double> vector;

  ASSERT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, Size_Trivial_AfterDefaultConstruction_Zero)
{
  FastVector<double> vector;

  ASSERT_EQ(vector.Size(), 0);
}

TEST(FastVector_Tests, Capacity_Trivial_AfterDefaultConstruction_Zero)
{
  FastVector<double> vector;

  ASSERT_EQ(vector.Capacity(), 0);
}

TEST(FastVector_Tests, PushBack_Trivial_Single_SizeIncrease)
{
  FastVector<double> vector;

  vector.PushBack(0);

  ASSERT_EQ(vector.Size(), 1);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Single_SizeIncrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");

  ASSERT_EQ(vector.Size(), 1);
}

TEST(FastVector_Tests, PushBack_Trivial_Double_SizeIncrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);

  ASSERT_EQ(vector.Size(), 2);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Double_SizeIncrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");

  ASSERT_EQ(vector.Size(), 2);
}

TEST(FastVector_Tests, PushBack_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(0));
  }

  ASSERT_EQ(vector.Size(), cAmount);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  FastVector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  ASSERT_EQ(vector.Size(), cAmount);
}

TEST(FastVector_Tests, PushBack_Trivial_Single_CorrectValue)
{
  FastVector<double> vector;

  const double value = 10;

  vector.PushBack(value);

  ASSERT_EQ(vector[0], value);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Single_CorrectValue)
{
  FastVector<std::string> vector;

  const std::string value = "10";

  vector.PushBack(value);

  ASSERT_EQ(vector[0], value);
}

TEST(FastVector_Tests, PushBack_Trivial_Double_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(10);
  vector.PushBack(11);

  ASSERT_EQ(vector[0], 10);
  ASSERT_EQ(vector[1], 11);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Double_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("10");
  vector.PushBack("11");

  ASSERT_EQ(vector[0], std::string { "10" });
  ASSERT_EQ(vector[1], std::string { "11" });
}

TEST(FastVector_Tests, PushBack_Trivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    ASSERT_EQ(vector[i], static_cast<double>(i));
  }
}

TEST(FastVector_Tests, PushBack_NonTrivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    ASSERT_EQ(vector[i], std::to_string(i));
  }
}

TEST(FastVector_Tests, PopBack_Trivial_Single_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 0);
  ASSERT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_NonTrivial_Single_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 0);
  ASSERT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_Double_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 0);
  ASSERT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_NonTrivial_Double_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 0);
  ASSERT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_DoublePushSinglePop_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 1);
  ASSERT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_NonTrivial_DoublePushSinglePop_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();

  ASSERT_EQ(vector.Size(), 1);
  ASSERT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_DoublePushSinglePop_CorrectValue)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();

  ASSERT_EQ(vector[0], 1);
}

TEST(FastVector_Tests, PopBack_NonTrivial_DoublePushSinglePop_CorrectValue)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();

  ASSERT_EQ(vector[0], std::string { "1" });
}

TEST(FastVector_Tests, PopBack_Trivial_PushPopPush_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PopBack();
  vector.PushBack(2);

  ASSERT_EQ(vector.Size(), 1);
  ASSERT_EQ(vector[0], 2);
}

TEST(FastVector_Tests, PopBack_NonTrivial_PushPopPush_CorrectSize)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PopBack();
  vector.PushBack("2");

  ASSERT_EQ(vector.Size(), 1);
  ASSERT_EQ(vector[0], std::string { "2" });
}

TEST(FastVector_Tests, PopBack_Trivial_PushPushPopPush_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();
  vector.PushBack(3);

  ASSERT_EQ(vector.Size(), 2);
  ASSERT_EQ(vector[1], 3);
}

TEST(FastVector_Tests, PopBack_NonTrivial_PushPushPopPush_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();
  vector.PushBack("3");

  ASSERT_EQ(vector.Size(), 2);
  ASSERT_EQ(vector[1], std::string { "3" });
}

TEST(FastVector_Tests, BuiltInForEach_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto value : vector)
  {
    ASSERT_EQ(vector[counter], value);

    counter++;
  }
}

TEST(FastVector_Tests, BuiltInForEach_NonTrivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  size_t counter = 0;

  for (auto value : vector)
  {
    ASSERT_EQ(vector[counter], value);

    counter++;
  }
}

TEST(FastVector_Tests, Begin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.begin(); it != vector.end(); it++)
  {
    ASSERT_EQ(vector[counter], *it);

    counter++;
  }
}

TEST(FastVector_Tests, CBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.cbegin(); it != vector.cend(); it++)
  {
    ASSERT_EQ(vector[counter], *it);

    counter++;
  }
}

TEST(FastVector_Tests, RBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.rbegin(); it != vector.rend(); it++)
  {
    ASSERT_EQ(vector[cAmount - counter - 1], *it);

    counter++;
  }
}

TEST(FastVector_Tests, Front_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  ASSERT_EQ(vector.front(), 0);
}

TEST(FastVector_Tests, Back_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  ASSERT_EQ(vector.back(), cAmount - 1);
}

} // namespace genebits::engine::tests