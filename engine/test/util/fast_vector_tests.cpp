#include "genebits/engine/util/fast_vector.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(sizeof(FastVector<size_t>) == 16, "The size of FastVector should not need to be bigger than 16 bytes");

TEST(FastVector_Tests, Empty_Trivial_AfterDefaultConstruction_True)
{
  FastVector<double> vector;

  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, Size_Trivial_AfterDefaultConstruction_Zero)
{
  FastVector<double> vector;

  EXPECT_EQ(vector.Size(), 0u);
}

TEST(FastVector_Tests, Capacity_Trivial_AfterDefaultConstruction_Zero)
{
  FastVector<double> vector;

  EXPECT_EQ(vector.Capacity(), 0u);
}

TEST(FastVector_Tests, PushBack_Trivial_Single_SizeIncrease)
{
  FastVector<double> vector;

  vector.PushBack(0);

  EXPECT_EQ(vector.Size(), 1u);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Single_SizeIncrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");

  EXPECT_EQ(vector.Size(), 1u);
}

TEST(FastVector_Tests, PushBack_Trivial_Double_SizeIncrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);

  EXPECT_EQ(vector.Size(), 2u);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Double_SizeIncrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");

  EXPECT_EQ(vector.Size(), 2u);
}

TEST(FastVector_Tests, PushBack_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(0));
  }

  EXPECT_EQ(vector.Size(), cAmount);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  FastVector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  EXPECT_EQ(vector.Size(), cAmount);
}

TEST(FastVector_Tests, PushBack_Trivial_Single_CorrectValue)
{
  FastVector<double> vector;

  const double value = 10;

  vector.PushBack(value);

  EXPECT_EQ(vector[0], value);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Single_CorrectValue)
{
  FastVector<std::string> vector;

  const std::string value = "10";

  vector.PushBack(value);

  EXPECT_EQ(vector[0], value);
}

TEST(FastVector_Tests, PushBack_Trivial_Double_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(10);
  vector.PushBack(11);

  EXPECT_EQ(vector[0], 10);
  EXPECT_EQ(vector[1], 11);
}

TEST(FastVector_Tests, PushBack_NonTrivial_Double_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("10");
  vector.PushBack("11");

  EXPECT_EQ(vector[0], std::string { "10" });
  EXPECT_EQ(vector[1], std::string { "11" });
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
    EXPECT_EQ(vector[i], static_cast<double>(i));
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
    EXPECT_EQ(vector[i], std::to_string(i));
  }
}

TEST(FastVector_Tests, PopBack_Trivial_Single_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, Clear_Trivial_Empty)
{
  FastVector<double> vector;

  vector.PushBack(10);
  vector.PushBack(11);

  vector.Clear();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, Clear_NonTrivial_Empty)
{
  FastVector<std::string> vector;

  vector.PushBack("10");
  vector.PushBack("11");

  vector.Clear();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, Reserve_Trivial_Empty_CorrectCapacity)
{
  FastVector<double> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, Reserve_NonTrivial_Empty_CorrectCapacity)
{
  FastVector<std::string> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, Reserve_Trivial_Increase_CorrectCapacity)
{
  FastVector<double> vector;

  vector.Reserve(5);

  EXPECT_EQ(vector.Capacity(), 5u);

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, Reserve_NonTrivial_Increase_CorrectCapacity)
{
  FastVector<std::string> vector;

  vector.Reserve(5);

  EXPECT_EQ(vector.Capacity(), 5u);

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, Reserve_Trivial_Decrease_DoNothing)
{
  FastVector<double> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);

  vector.Reserve(5);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, Reserve_NonTrivial_Decrease_DoNothing)
{
  FastVector<std::string> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.Capacity(), 10u);

  vector.Reserve(5);

  EXPECT_EQ(vector.Capacity(), 10u);
}

TEST(FastVector_Tests, PopBack_NonTrivial_Single_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_Double_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_NonTrivial_Double_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_DoublePushSinglePop_SizeDecrease)
{
  FastVector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_NonTrivial_DoublePushSinglePop_SizeDecrease)
{
  FastVector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, PopBack_Trivial_DoublePushSinglePop_CorrectValue)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();

  EXPECT_EQ(vector[0], 1);
}

TEST(FastVector_Tests, PopBack_NonTrivial_DoublePushSinglePop_CorrectValue)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();

  EXPECT_EQ(vector[0], std::string { "1" });
}

TEST(FastVector_Tests, PopBack_Trivial_PushPopPush_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PopBack();
  vector.PushBack(2);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_EQ(vector[0], 2);
}

TEST(FastVector_Tests, PopBack_NonTrivial_PushPopPush_CorrectSize)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PopBack();
  vector.PushBack("2");

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(FastVector_Tests, PopBack_Trivial_PushPushPopPush_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();
  vector.PushBack(3);

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[1], 3);
}

TEST(FastVector_Tests, PopBack_NonTrivial_PushPushPopPush_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();
  vector.PushBack("3");

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[1], std::string { "3" });
}

TEST(FastVector_Tests, EraseAt_Trivial_Single_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, EraseAt_NonTrivial_Single_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, EraseAt_Trivial_Double_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.EraseAt(1);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], 1);

  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, EraseAt_NonTrivial_Double_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.EraseAt(1);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], "1");

  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_TRUE(vector.Empty());
}

TEST(FastVector_Tests, EraseAt_Trivial_PushPushErase_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(FastVector_Tests, EraseAt_NonTrivial_PushPushErase_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.EraseAt(0);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], "2");
}

TEST(FastVector_Tests, Erase_Trivial_PushPushErase_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector.Erase(std::find(vector.begin(), vector.end(), 1));

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(FastVector_Tests, Erase_NonTrivial_PushPushErase_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  vector.Erase(std::find(vector.begin(), vector.end(), std::string { "1" }));

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_FALSE(vector.Empty());
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(FastVector_Tests, Resize_Trivial_Empty_SizeIncrease)
{
  constexpr size_t cAmount = 10;

  FastVector<double> vector;

  vector.Resize(cAmount);

  EXPECT_EQ(vector.Size(), cAmount);
  EXPECT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, Resize_NonTrivial_Empty_SizeIncrease)
{
  constexpr size_t cAmount = 10;

  FastVector<std::string> vector;

  vector.Resize(cAmount);

  EXPECT_EQ(vector.Size(), cAmount);
  EXPECT_FALSE(vector.Empty());
}

TEST(FastVector_Tests, Resize_Trivial_Empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  FastVector<double> vector;

  vector.Resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], double {});
  }
}

TEST(FastVector_Tests, Resize_NonTrivial_Empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  FastVector<std::string> vector;

  vector.Resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::string {});
  }
}

TEST(FastVector_Tests, Resize_Trivial_Increase_CorrectValues)
{
  constexpr size_t cAmount = 10;

  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector.Resize(cAmount);

  EXPECT_EQ(vector.Size(), cAmount);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  for (size_t i = 2; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], double {});
  }
}

TEST(FastVector_Tests, Resize_NonTrivial_Increase_CorrectValues)
{
  constexpr size_t cAmount = 10;

  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  vector.Resize(cAmount);

  EXPECT_EQ(vector.Size(), cAmount);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });

  for (size_t i = 2; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::string {});
  }
}

TEST(FastVector_Tests, Resize_Trivial_Decrease_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PushBack(3);

  vector.Resize(2);

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  vector.PushBack(4);
  vector.Resize(1);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_EQ(vector[0], 1);
}

TEST(FastVector_Tests, Resize_NonTrivial_Decrease_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PushBack("3");

  vector.Resize(2);

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });

  vector.PushBack("4");
  vector.Resize(1);

  EXPECT_EQ(vector.Size(), 1u);
  EXPECT_EQ(vector[0], std::string { "1" });
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
    EXPECT_EQ(vector[counter], value);

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
    EXPECT_EQ(vector[counter], value);

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
    EXPECT_EQ(vector[counter], *it);

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
    EXPECT_EQ(vector[counter], *it);

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
    EXPECT_EQ(vector[cAmount - counter - 1], *it);

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

  EXPECT_EQ(vector.front(), 0);
}

TEST(FastVector_Tests, Back_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  FastVector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  EXPECT_EQ(vector.back(), static_cast<double>(cAmount - 1UL));
}

TEST(FastVector_Tests, MoveConstructor_Trivial_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  FastVector<double> copy { std::move(vector) };

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_EQ(vector.Capacity(), 0u);

  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(FastVector_Tests, MoveConstructor_NonTrivial_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  FastVector<std::string> copy { std::move(vector) };

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_EQ(vector.Capacity(), 0u);

  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(FastVector_Tests, MoveAssignment_Trivial_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  FastVector<double> moved = std::move(vector);

  EXPECT_EQ(moved.Size(), 2u);
  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_EQ(vector.Capacity(), 0u);

  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
}

TEST(FastVector_Tests, MoveAssignment_NonTrivial_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  FastVector<std::string> moved = std::move(vector);

  EXPECT_EQ(moved.Size(), 2u);
  EXPECT_EQ(vector.Size(), 0u);
  EXPECT_EQ(vector.Capacity(), 0u);

  EXPECT_EQ(moved[0], std::string { "1" });
  EXPECT_EQ(moved[1], std::string { "2" });
}

TEST(FastVector_Tests, MoveAssignment_SelfMove_DoNothing)
{
  FastVector<int> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector = std::move(vector);

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(FastVector_Tests, CopyConstructor_Trivial_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  FastVector<double> copy { vector };

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(copy.Capacity(), 2u);
  EXPECT_EQ(vector.Size(), 2u);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(FastVector_Tests, CopyConstructor_NonTrivial_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  FastVector<std::string> copy { vector };

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(copy.Capacity(), 2u);
  EXPECT_EQ(vector.Size(), 2u);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(FastVector_Tests, CopyAssignment_Trivial_CorrectValues)
{
  FastVector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  FastVector<double> copy = vector;

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(copy.Capacity(), 2u);
  EXPECT_EQ(vector.Size(), 2u);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(FastVector_Tests, CopyAssignment_NonTrivial_CorrectValues)
{
  FastVector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  FastVector<std::string> copy = vector;

  EXPECT_EQ(copy.Size(), 2u);
  EXPECT_EQ(copy.Capacity(), 2u);
  EXPECT_EQ(vector.Size(), 2u);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(FastVector_Tests, CopyAssignment_SelfAssignement_DoNothing)
{
  FastVector<int> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector = vector;

  EXPECT_EQ(vector.Size(), 2u);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(FastVector_Tests, Equality_Empty_Equal)
{
  FastVector<int> vector1;

  FastVector<int> vector2;

  EXPECT_EQ(vector1, vector2);
}

TEST(FastVector_Tests, Equality_SelfEquality_Equal)
{
  FastVector<int> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  EXPECT_EQ(vector, vector);
}

TEST(FastVector_Tests, Equality_SameValues_Equal)
{
  FastVector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(2);
  vector1.PushBack(3);

  FastVector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_EQ(vector1, vector2);
}

TEST(FastVector_Tests, Inequality_DifferentSize_NotEqual)
{
  FastVector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(2);

  FastVector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_NE(vector1, vector2);
}

TEST(FastVector_Tests, Inequality_DifferentValues_NotEqual)
{
  FastVector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(4);
  vector1.PushBack(3);

  FastVector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_NE(vector1, vector2);
}

} // namespace genebits::engine::tests