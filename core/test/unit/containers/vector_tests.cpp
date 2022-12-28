#include "plex/containers/vector.h"

#include <gtest/gtest.h>

namespace plex::tests
{
static_assert(
  sizeof(Vector<size_t>) == sizeof(void*) * 2, "The size of Vector should not need to be bigger than 2 pointers");
static_assert(std::contiguous_iterator<Vector<size_t>::iterator>, "Vector should have contiguous iterator");
static_assert(std::ranges::contiguous_range<Vector<size_t>>, "Vector should be a contiguous iterator");

static_assert(IsTriviallyRelocatable<Vector<size_t>>::value, "Vector should be relocatable");

TEST(Vector_Tests, Empty_Trivial_AfterDefaultConstruction_True)
{
  Vector<double> vector;

  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Size_Trivial_AfterDefaultConstruction_Zero)
{
  Vector<double> vector;

  EXPECT_EQ(vector.size(), 0);
}

TEST(Vector_Tests, Capacity_Trivial_AfterDefaultConstruction_Zero)
{
  Vector<double> vector;

  EXPECT_EQ(vector.capacity(), 0);
}

TEST(Vector_Tests, ConstructorCArray_Trivial_EmptyCArray_Empty)
{
  Vector<double> vector({});

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, ConstructorCArray_NonTrivial_EmptyCArray_Empty)
{
  Vector<std::string> vector({});

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, ConstructorCArray_Trivial_OneElement_NotEmpty)
{
  Vector<double> vector({ 0.5 });

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, ConstructorCArray_Trivial_OneElement_CorrectValue)
{
  Vector<double> vector({ 0.5 });

  EXPECT_EQ(vector[0], 0.5);
}

TEST(Vector_Tests, ConstructorCArray_NonTrivial_OneElement_NotEmpty)
{
  Vector<std::string> vector({ std::string { "0.5" } });

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, ConstructorCArray_NonTrivial_OneElement_CorrectValue)
{
  Vector<std::string> vector({ std::string { "0.5" } });

  EXPECT_EQ(vector[0], std::string { "0.5" });
}

TEST(Vector_Tests, ConstructorCArray_Trivial_ManyElements_CorrectValues)
{
  Vector<double> vector({ 0.5, 0.4, 0.1 });

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 3);

  EXPECT_EQ(vector[0], 0.5);
  EXPECT_EQ(vector[1], 0.4);
  EXPECT_EQ(vector[2], 0.1);
}

TEST(Vector_Tests, ConstructorIterator_Trivial_ManyElements_CorrectValues)
{
  Vector<double> values({ 0.5, 0.4, 0.1 });

  Vector<double> vector(values.begin(), values.end());

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 3);

  EXPECT_EQ(vector[0], 0.5);
  EXPECT_EQ(vector[1], 0.4);
  EXPECT_EQ(vector[2], 0.1);
}

TEST(Vector_Tests, ConstructorRanges_Trivial_ManyElements_CorrectValues)
{
  Vector<double> values({ 0.5, 0.4, 0.1 });

  Vector<double> vector(values);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 3);

  EXPECT_EQ(vector[0], 0.5);
  EXPECT_EQ(vector[1], 0.4);
  EXPECT_EQ(vector[2], 0.1);
}

TEST(Vector_Tests, PushBack_Trivial_Single_SizeIncrease)
{
  Vector<double> vector;

  vector.push_back(0);

  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, PushBack_NonTrivial_Single_SizeIncrease)
{
  Vector<std::string> vector;

  vector.push_back("0");

  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, PushBack_Trivial_Double_SizeIncrease)
{
  Vector<double> vector;

  vector.push_back(0);
  vector.push_back(0);

  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, PushBack_NonTrivial_Double_SizeIncrease)
{
  Vector<std::string> vector;

  vector.push_back("0");
  vector.push_back("0");

  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, PushBack_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(0));
  }

  EXPECT_EQ(vector.size(), cAmount);
}

TEST(Vector_Tests, PushBack_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  EXPECT_EQ(vector.size(), cAmount);
}

TEST(Vector_Tests, PushBack_Trivial_Single_CorrectValue)
{
  Vector<double> vector;

  const double value = 10;

  vector.push_back(value);

  EXPECT_EQ(vector[0], value);
}

TEST(Vector_Tests, PushBack_NonTrivial_Single_CorrectValue)
{
  Vector<std::string> vector;

  const std::string value = "10";

  vector.push_back(value);

  EXPECT_EQ(vector[0], value);
}

TEST(Vector_Tests, PushBack_Trivial_Double_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(10);
  vector.push_back(11);

  EXPECT_EQ(vector[0], 10);
  EXPECT_EQ(vector[1], 11);
}

TEST(Vector_Tests, PushBack_NonTrivial_Double_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.push_back("11");

  EXPECT_EQ(vector[0], std::string { "10" });
  EXPECT_EQ(vector[1], std::string { "11" });
}

TEST(Vector_Tests, PushBack_Trivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], static_cast<double>(i));
  }
}

TEST(Vector_Tests, PushBack_NonTrivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::to_string(i));
  }
}

TEST(Vector_Tests, EmplaceBack_NonTrivial_Single_SizeIncrease)
{
  Vector<std::pair<int, int>> vector;

  vector.emplace_back(1, 2);

  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, EmplaceBack_NonTrivial_Single_CorrectValue)
{
  Vector<std::pair<int, int>> vector;

  vector.emplace_back(99, 10);

  EXPECT_EQ(vector[0].first, 99);
  EXPECT_EQ(vector[0].second, 10);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtEndWhenEmpty_SizeIncrease)
{
  Vector<int> vector;

  vector.insert(vector.end(), 99);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtEndWhenEmpty_CorrectValue)
{
  Vector<int> vector;

  vector.insert(vector.end(), 99);

  EXPECT_EQ(vector.back(), 99);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtEnd_SizeIncrease)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.insert(vector.end(), 99);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtEnd_CorrectValue)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.insert(vector.end(), 99);

  EXPECT_EQ(vector.front(), 10);
  EXPECT_EQ(vector.back(), 99);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtBegining_SizeIncrease)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.insert(vector.begin(), 99);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtBegining_CorrectValue)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.insert(vector.begin(), 99);

  EXPECT_EQ(vector.front(), 99);
  EXPECT_EQ(vector.back(), 10);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtMiddle_SizeIncrease)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.push_back(11);
  vector.insert(vector.begin() + 1, 99);

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 3);
}

TEST(Vector_Tests, Insert_Trivial_SingleAtMiddle_CorrectValue)
{
  Vector<int> vector;

  vector.push_back(10);
  vector.push_back(11);
  vector.insert(vector.begin() + 1, 99);

  EXPECT_EQ(vector.front(), 10);
  EXPECT_EQ(*(vector.begin() + 1), 99);
  EXPECT_EQ(vector.back(), 11);
}

TEST(Vector_Tests, Insert_Trivial_DoubleAtBegining_CorrectValue)
{
  Vector<int> vector;

  vector.insert(vector.begin(), 10);
  vector.insert(vector.begin(), 11);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 11);
  EXPECT_EQ(vector[1], 10);
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtEndWhenEmpty_SizeIncrease)
{
  Vector<std::string> vector;

  vector.insert(vector.end(), "99");

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtEndWhenEmpty_CorrectValue)
{
  Vector<std::string> vector;

  vector.insert(vector.end(), "99");

  EXPECT_EQ(vector.back(), std::string { "99" });
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtEnd_SizeIncrease)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.insert(vector.end(), "99");

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtEnd_CorrectValue)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.insert(vector.end(), "99");

  EXPECT_EQ(vector.front(), std::string { "10" });
  EXPECT_EQ(vector.back(), std::string { "99" });
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtBegining_SizeIncrease)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.insert(vector.begin(), "99");

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtBegining_CorrectValue)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.insert(vector.begin(), "99");

  EXPECT_EQ(vector.front(), std::string { "99" });
  EXPECT_EQ(vector.back(), std::string { "10" });
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtMiddle_SizeIncrease)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.push_back("11");
  vector.insert(vector.begin() + 1, "99");

  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector.size(), 3);
}

TEST(Vector_Tests, Insert_NonTrivial_SingleAtMiddle_CorrectValue)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.push_back("11");
  vector.insert(vector.begin() + 1, "99");

  EXPECT_EQ(vector.front(), std::string { "10" });
  EXPECT_EQ(*(vector.begin() + 1), std::string { "99" });
  EXPECT_EQ(vector.back(), std::string { "11" });
}

TEST(Vector_Tests, Insert_NonTrivial_DoubleAtBegining_CorrectValue)
{
  Vector<std::string> vector;

  vector.insert(vector.begin(), "10");
  vector.insert(vector.begin(), "11");

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], std::string { "11" });
  EXPECT_EQ(vector[1], std::string { "10" });
}

TEST(Vector_Tests, Insert_Trivial_ManyAtBegining_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.insert(vector.begin(), static_cast<double>(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], static_cast<double>(cAmount - i - 1));
  }
}

TEST(Vector_Tests, Insert_NonTrivial_ManyAtBegining_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.insert(vector.begin(), std::to_string(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::to_string(cAmount - i - 1));
  }
}

TEST(Vector_Tests, Insert_Trivial_ManyAtBeginingPlus1_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  vector.push_back(-1);

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.insert(vector.begin() + 1, static_cast<double>(i));
  }

  EXPECT_EQ(vector[0], -1);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i + 1], static_cast<double>(cAmount - i - 1));
  }
}

TEST(Vector_Tests, Insert_NonTrivial_ManyAtBeginingPlus1_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  vector.push_back(std::string { "-1" });

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.insert(vector.begin() + 1, std::to_string(i));
  }

  EXPECT_EQ(vector[0], std::to_string(-1));

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i + 1], std::to_string(cAmount - i - 1));
  }
}

TEST(Vector_Tests, PopBack_Trivial_Single_SizeDecrease)
{
  Vector<double> vector;

  vector.push_back(0);
  vector.pop_back();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Clear_Trivial_Empty)
{
  Vector<double> vector;

  vector.push_back(10);
  vector.push_back(11);

  vector.clear();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Clear_NonTrivial_empty)
{
  Vector<std::string> vector;

  vector.push_back("10");
  vector.push_back("11");

  vector.clear();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Reserve_Trivial_empty_CorrectCapacity)
{
  Vector<double> vector;

  vector.reserve(10);

  EXPECT_GE(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_NonTrivial_empty_CorrectCapacity)
{
  Vector<std::string> vector;

  vector.reserve(10);

  EXPECT_GE(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_Trivial_Increase_CorrectCapacity)
{
  Vector<double> vector;

  vector.reserve(5);

  EXPECT_GE(vector.capacity(), 5);

  vector.reserve(10);

  EXPECT_GE(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_NonTrivial_Increase_CorrectCapacity)
{
  Vector<std::string> vector;

  vector.reserve(5);

  EXPECT_GE(vector.capacity(), 5);

  vector.reserve(10);

  EXPECT_GE(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_Trivial_Decrease_DoNothing)
{
  Vector<double> vector;

  vector.reserve(10);

  size_t last_capacity = vector.capacity();

  EXPECT_GE(last_capacity, 10);

  vector.reserve(5);

  EXPECT_EQ(last_capacity, vector.capacity());
}

TEST(Vector_Tests, Reserve_NonTrivial_Decrease_DoNothing)
{
  Vector<std::string> vector;

  vector.reserve(10);

  size_t last_capacity = vector.capacity();

  EXPECT_GE(last_capacity, 10);

  vector.reserve(5);

  EXPECT_EQ(last_capacity, vector.capacity());
}

TEST(Vector_Tests, PopBack_NonTrivial_Single_sizeDecrease)
{
  Vector<std::string> vector;

  vector.push_back("0");
  vector.pop_back();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_Double_sizeDecrease)
{
  Vector<double> vector;

  vector.push_back(0);
  vector.push_back(0);
  vector.pop_back();
  vector.pop_back();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_NonTrivial_Double_sizeDecrease)
{
  Vector<std::string> vector;

  vector.push_back("0");
  vector.push_back("0");
  vector.pop_back();
  vector.pop_back();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_DoublePushSinglePop_sizeDecrease)
{
  Vector<double> vector;

  vector.push_back(0);
  vector.push_back(0);
  vector.pop_back();

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, PopBack_NonTrivial_DoublePushSinglePop_sizeDecrease)
{
  Vector<std::string> vector;

  vector.push_back("0");
  vector.push_back("0");
  vector.pop_back();

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_DoublePushSinglePop_CorrectValue)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.pop_back();

  EXPECT_EQ(vector[0], 1);
}

TEST(Vector_Tests, PopBack_NonTrivial_DoublePushSinglePop_CorrectValue)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");
  vector.pop_back();

  EXPECT_EQ(vector[0], std::string { "1" });
}

TEST(Vector_Tests, PopBack_Trivial_PushPopPush_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.pop_back();
  vector.push_back(2);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, PopBack_NonTrivial_PushPopPush_Correctsize)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.pop_back();
  vector.push_back("2");

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(Vector_Tests, PopBack_Trivial_PushPushPopPush_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.pop_back();
  vector.push_back(3);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[1], 3);
}

TEST(Vector_Tests, PopBack_NonTrivial_PushPushPopPush_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");
  vector.pop_back();
  vector.push_back("3");

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[1], std::string { "3" });
}

TEST(Vector_Tests, SwapAndPop_Trivial_Single_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, SwapAndPop_NonTrivial_Single_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, SwapAndPop_Trivial_Double_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.SwapAndPop(vector.begin() + 1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 1);

  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, SwapAndPop_NonTrivial_Double_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");
  vector.SwapAndPop(vector.begin() + 1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], "1");

  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, SwapAndPop_Trivial_PushPushErase_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, SwapAndPop_NonTrivial_PushPushErase_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");
  vector.SwapAndPop(vector.begin());

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], "2");
}

TEST(Vector_Tests, SwapAndPop_Trivial_PushPushFindErase_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  vector.SwapAndPop(std::find(vector.begin(), vector.end(), 1));

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, SwapAndPop_NonTrivial_PushPushFindErase_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  vector.SwapAndPop(std::find(vector.begin(), vector.end(), std::string { "1" }));

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(Vector_Tests, Resize_Trivial_empty_sizeIncrease)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, Resize_NonTrivial_empty_sizeIncrease)
{
  constexpr size_t cAmount = 10;

  Vector<std::string> vector;

  vector.resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, Resize_Trivial_empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], double {});
  }
}

TEST(Vector_Tests, Resize_NonTrivial_empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  Vector<std::string> vector;

  vector.resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::string {});
  }
}

TEST(Vector_Tests, Resize_Trivial_Increase_CorrectValues)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  vector.resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  for (size_t i = 2; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], double {});
  }
}

TEST(Vector_Tests, Resize_NonTrivial_Increase_CorrectValues)
{
  constexpr size_t cAmount = 10;

  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  vector.resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });

  for (size_t i = 2; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::string {});
  }
}

TEST(Vector_Tests, Resize_Trivial_Decrease_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  vector.resize(2);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  vector.push_back(4);
  vector.resize(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], 1);
}

TEST(Vector_Tests, Resize_NonTrivial_Decrease_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");
  vector.push_back("3");

  vector.resize(2);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });

  vector.push_back("4");
  vector.resize(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], std::string { "1" });
}

TEST(Vector_Tests, BuiltInForEach_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto value : vector)
  {
    EXPECT_EQ(vector[counter], value);

    counter++;
  }
}

TEST(Vector_Tests, BuiltInForEach_NonTrivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(std::to_string(i));
  }

  size_t counter = 0;

  for (auto value : vector)
  {
    EXPECT_EQ(vector[counter], value);

    counter++;
  }
}

TEST(Vector_Tests, Begin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.begin(); it != vector.end(); it++)
  {
    EXPECT_EQ(vector[counter], *it);

    counter++;
  }
}

TEST(Vector_Tests, CBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.cbegin(); it != vector.cend(); it++)
  {
    EXPECT_EQ(vector[counter], *it);

    counter++;
  }
}

TEST(Vector_Tests, RBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  size_t counter = 0;

  for (auto it = vector.rbegin(); it != vector.rend(); it++)
  {
    EXPECT_EQ(vector[cAmount - counter - 1], *it);

    counter++;
  }
}

TEST(Vector_Tests, Front_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  EXPECT_EQ(vector.front(), 0);
}

TEST(Vector_Tests, Back_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.push_back(static_cast<double>(i));
  }

  EXPECT_EQ(vector.back(), cAmount - 1);
}

TEST(Vector_Tests, MoveConstructor_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  Vector<double> copy { std::move(vector) };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);

  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Vector_Tests, MoveConstructor_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  Vector<std::string> copy { std::move(vector) };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);

  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Vector_Tests, MoveAssignment_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  Vector<double> moved = std::move(vector);

  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);

  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
}

TEST(Vector_Tests, MoveAssignment_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  Vector<std::string> moved = std::move(vector);

  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(vector.size(), 0);
  EXPECT_EQ(vector.capacity(), 0);

  EXPECT_EQ(moved[0], std::string { "1" });
  EXPECT_EQ(moved[1], std::string { "2" });
}

TEST(Vector_Tests, MoveAssignment_SelfMove_DoNothing)
{
  Vector<int> vector;

  vector.push_back(1);
  vector.push_back(2);

  vector = std::move(vector);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(Vector_Tests, CopyConstructor_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  Vector<double> copy { vector };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Vector_Tests, CopyConstructor_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  Vector<std::string> copy { vector };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Vector_Tests, CopyAssignment_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.push_back(1);
  vector.push_back(2);

  Vector<double> copy = vector;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Vector_Tests, CopyAssignment_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.push_back("1");
  vector.push_back("2");

  Vector<std::string> copy = vector;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Vector_Tests, CopyAssignment_SelfAssignement_DoNothing)
{
  Vector<int> vector;

  vector.push_back(1);
  vector.push_back(2);

  vector = vector;

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(Vector_Tests, Equality_empty_Equal)
{
  Vector<int> vector1;

  Vector<int> vector2;

  EXPECT_EQ(vector1, vector2);
}

TEST(Vector_Tests, Equality_SelfEquality_Equal)
{
  Vector<int> vector;

  vector.push_back(1);
  vector.push_back(2);

  EXPECT_EQ(vector, vector);
}

TEST(Vector_Tests, Equality_SameValues_Equal)
{
  Vector<int> vector1;

  vector1.push_back(1);
  vector1.push_back(2);
  vector1.push_back(3);

  Vector<int> vector2;

  vector2.push_back(1);
  vector2.push_back(2);
  vector2.push_back(3);

  EXPECT_EQ(vector1, vector2);
}

TEST(Vector_Tests, Inequality_Differentsize_NotEqual)
{
  Vector<int> vector1;

  vector1.push_back(1);
  vector1.push_back(2);

  Vector<int> vector2;

  vector2.push_back(1);
  vector2.push_back(2);
  vector2.push_back(3);

  EXPECT_NE(vector1, vector2);
}

TEST(Vector_Tests, Inequality_DifferentValues_NotEqual)
{
  Vector<int> vector1;

  vector1.push_back(1);
  vector1.push_back(4);
  vector1.push_back(3);

  Vector<int> vector2;

  vector2.push_back(1);
  vector2.push_back(2);
  vector2.push_back(3);

  EXPECT_NE(vector1, vector2);
}

} // namespace plex::tests