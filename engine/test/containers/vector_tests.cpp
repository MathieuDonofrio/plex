#include "genebits/engine/containers/vector.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(sizeof(Vector<size_t>) == 16, "The size of Vector should not need to be bigger than 16 bytes");
static_assert(std::contiguous_iterator<Vector<size_t>::iterator>, "Vector should have contiguous iterator");

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

TEST(Vector_Tests, PushBack_Trivial_Single_SizeIncrease)
{
  Vector<double> vector;

  vector.PushBack(0);

  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, PushBack_NonTrivial_Single_SizeIncrease)
{
  Vector<std::string> vector;

  vector.PushBack("0");

  EXPECT_EQ(vector.size(), 1);
}

TEST(Vector_Tests, PushBack_Trivial_Double_SizeIncrease)
{
  Vector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);

  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, PushBack_NonTrivial_Double_SizeIncrease)
{
  Vector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");

  EXPECT_EQ(vector.size(), 2);
}

TEST(Vector_Tests, PushBack_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(0));
  }

  EXPECT_EQ(vector.size(), cAmount);
}

TEST(Vector_Tests, PushBack_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(std::to_string(i));
  }

  EXPECT_EQ(vector.size(), cAmount);
}

TEST(Vector_Tests, PushBack_Trivial_Single_CorrectValue)
{
  Vector<double> vector;

  const double value = 10;

  vector.PushBack(value);

  EXPECT_EQ(vector[0], value);
}

TEST(Vector_Tests, PushBack_NonTrivial_Single_CorrectValue)
{
  Vector<std::string> vector;

  const std::string value = "10";

  vector.PushBack(value);

  EXPECT_EQ(vector[0], value);
}

TEST(Vector_Tests, PushBack_Trivial_Double_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(10);
  vector.PushBack(11);

  EXPECT_EQ(vector[0], 10);
  EXPECT_EQ(vector[1], 11);
}

TEST(Vector_Tests, PushBack_NonTrivial_Double_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("10");
  vector.PushBack("11");

  EXPECT_EQ(vector[0], std::string { "10" });
  EXPECT_EQ(vector[1], std::string { "11" });
}

TEST(Vector_Tests, PushBack_Trivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
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
    vector.PushBack(std::to_string(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::to_string(i));
  }
}

TEST(Vector_Tests, PopBack_Trivial_Single_SizeDecrease)
{
  Vector<double> vector;

  vector.PushBack(0);
  vector.PopBack();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Clear_Trivial_Empty)
{
  Vector<double> vector;

  vector.PushBack(10);
  vector.PushBack(11);

  vector.Clear();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Clear_NonTrivial_empty)
{
  Vector<std::string> vector;

  vector.PushBack("10");
  vector.PushBack("11");

  vector.Clear();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, Reserve_Trivial_empty_CorrectCapacity)
{
  Vector<double> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_NonTrivial_empty_CorrectCapacity)
{
  Vector<std::string> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_Trivial_Increase_CorrectCapacity)
{
  Vector<double> vector;

  vector.Reserve(5);

  EXPECT_EQ(vector.capacity(), 5);

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_NonTrivial_Increase_CorrectCapacity)
{
  Vector<std::string> vector;

  vector.Reserve(5);

  EXPECT_EQ(vector.capacity(), 5);

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_Trivial_Decrease_DoNothing)
{
  Vector<double> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);

  vector.Reserve(5);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, Reserve_NonTrivial_Decrease_DoNothing)
{
  Vector<std::string> vector;

  vector.Reserve(10);

  EXPECT_EQ(vector.capacity(), 10);

  vector.Reserve(5);

  EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector_Tests, PopBack_NonTrivial_Single_sizeDecrease)
{
  Vector<std::string> vector;

  vector.PushBack("0");
  vector.PopBack();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_Double_sizeDecrease)
{
  Vector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();
  vector.PopBack();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_NonTrivial_Double_sizeDecrease)
{
  Vector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();
  vector.PopBack();

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_DoublePushSinglePop_sizeDecrease)
{
  Vector<double> vector;

  vector.PushBack(0);
  vector.PushBack(0);
  vector.PopBack();

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, PopBack_NonTrivial_DoublePushSinglePop_sizeDecrease)
{
  Vector<std::string> vector;

  vector.PushBack("0");
  vector.PushBack("0");
  vector.PopBack();

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, PopBack_Trivial_DoublePushSinglePop_CorrectValue)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();

  EXPECT_EQ(vector[0], 1);
}

TEST(Vector_Tests, PopBack_NonTrivial_DoublePushSinglePop_CorrectValue)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();

  EXPECT_EQ(vector[0], std::string { "1" });
}

TEST(Vector_Tests, PopBack_Trivial_PushPopPush_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PopBack();
  vector.PushBack(2);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, PopBack_NonTrivial_PushPopPush_Correctsize)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PopBack();
  vector.PushBack("2");

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(Vector_Tests, PopBack_Trivial_PushPushPopPush_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PopBack();
  vector.PushBack(3);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[1], 3);
}

TEST(Vector_Tests, PopBack_NonTrivial_PushPushPopPush_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PopBack();
  vector.PushBack("3");

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[1], std::string { "3" });
}

TEST(Vector_Tests, EraseAt_Trivial_Single_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, EraseAt_NonTrivial_Single_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, EraseAt_Trivial_Double_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.EraseAt(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 1);

  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, EraseAt_NonTrivial_Double_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.EraseAt(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], "1");

  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 0);
  EXPECT_TRUE(vector.empty());
}

TEST(Vector_Tests, EraseAt_Trivial_PushPushErase_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);
  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, EraseAt_NonTrivial_PushPushErase_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.EraseAt(0);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], "2");
}

TEST(Vector_Tests, Erase_Trivial_PushPushErase_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector.Erase(std::find(vector.begin(), vector.end(), 1));

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], 2);
}

TEST(Vector_Tests, Erase_NonTrivial_PushPushErase_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  vector.Erase(std::find(vector.begin(), vector.end(), std::string { "1" }));

  EXPECT_EQ(vector.size(), 1);
  EXPECT_FALSE(vector.empty());
  EXPECT_EQ(vector[0], std::string { "2" });
}

TEST(Vector_Tests, Resize_Trivial_empty_sizeIncrease)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.Resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, Resize_NonTrivial_empty_sizeIncrease)
{
  constexpr size_t cAmount = 10;

  Vector<std::string> vector;

  vector.Resize(cAmount);

  EXPECT_EQ(vector.size(), cAmount);
  EXPECT_FALSE(vector.empty());
}

TEST(Vector_Tests, Resize_Trivial_empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.Resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], double {});
  }
}

TEST(Vector_Tests, Resize_NonTrivial_empty_DefaultValues)
{
  constexpr size_t cAmount = 10;

  Vector<std::string> vector;

  vector.Resize(cAmount);

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(vector[i], std::string {});
  }
}

TEST(Vector_Tests, Resize_Trivial_Increase_CorrectValues)
{
  constexpr size_t cAmount = 10;

  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  vector.Resize(cAmount);

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

  vector.PushBack("1");
  vector.PushBack("2");

  vector.Resize(cAmount);

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

  vector.PushBack(1);
  vector.PushBack(2);
  vector.PushBack(3);

  vector.Resize(2);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);

  vector.PushBack(4);
  vector.Resize(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], 1);
}

TEST(Vector_Tests, Resize_NonTrivial_Decrease_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");
  vector.PushBack("3");

  vector.Resize(2);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });

  vector.PushBack("4");
  vector.Resize(1);

  EXPECT_EQ(vector.size(), 1);
  EXPECT_EQ(vector[0], std::string { "1" });
}

TEST(Vector_Tests, BuiltInForEach_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

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

TEST(Vector_Tests, BuiltInForEach_NonTrivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<std::string> vector;

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

TEST(Vector_Tests, Begin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

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

TEST(Vector_Tests, CBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

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

TEST(Vector_Tests, RBegin_Trivial_IterateMany_CorrectValues)
{
  constexpr size_t cAmount = 100;

  Vector<double> vector;

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

TEST(Vector_Tests, Front_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  EXPECT_EQ(vector.front(), 0);
}

TEST(Vector_Tests, Back_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Vector<double> vector;

  for (size_t i = 0; i < cAmount; i++)
  {
    vector.PushBack(static_cast<double>(i));
  }

  EXPECT_EQ(vector.back(), cAmount - 1);
}

TEST(Vector_Tests, MoveConstructor_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

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

  vector.PushBack("1");
  vector.PushBack("2");

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

  vector.PushBack(1);
  vector.PushBack(2);

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

  vector.PushBack("1");
  vector.PushBack("2");

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

  vector.PushBack(1);
  vector.PushBack(2);

  vector = std::move(vector);

  EXPECT_EQ(vector.size(), 2);
  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
}

TEST(Vector_Tests, CopyConstructor_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  Vector<double> copy { vector };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.capacity(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Vector_Tests, CopyConstructor_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  Vector<std::string> copy { vector };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.capacity(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Vector_Tests, CopyAssignment_Trivial_CorrectValues)
{
  Vector<double> vector;

  vector.PushBack(1);
  vector.PushBack(2);

  Vector<double> copy = vector;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.capacity(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], 1);
  EXPECT_EQ(vector[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Vector_Tests, CopyAssignment_NonTrivial_CorrectValues)
{
  Vector<std::string> vector;

  vector.PushBack("1");
  vector.PushBack("2");

  Vector<std::string> copy = vector;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(copy.capacity(), 2);
  EXPECT_EQ(vector.size(), 2);

  EXPECT_EQ(vector[0], std::string { "1" });
  EXPECT_EQ(vector[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Vector_Tests, CopyAssignment_SelfAssignement_DoNothing)
{
  Vector<int> vector;

  vector.PushBack(1);
  vector.PushBack(2);

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

  vector.PushBack(1);
  vector.PushBack(2);

  EXPECT_EQ(vector, vector);
}

TEST(Vector_Tests, Equality_SameValues_Equal)
{
  Vector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(2);
  vector1.PushBack(3);

  Vector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_EQ(vector1, vector2);
}

TEST(Vector_Tests, Inequality_Differentsize_NotEqual)
{
  Vector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(2);

  Vector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_NE(vector1, vector2);
}

TEST(Vector_Tests, Inequality_DifferentValues_NotEqual)
{
  Vector<int> vector1;

  vector1.PushBack(1);
  vector1.PushBack(4);
  vector1.PushBack(3);

  Vector<int> vector2;

  vector2.PushBack(1);
  vector2.PushBack(2);
  vector2.PushBack(3);

  EXPECT_NE(vector1, vector2);
}

} // namespace genebits::engine::tests