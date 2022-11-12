#include "plex/containers/deque.h"

#include <gtest/gtest.h>

namespace plex::tests
{
static_assert(IsTriviallyRelocatable<Deque<size_t>>::value, "Dequeue should be relocatable");
static_assert(std::random_access_iterator<Deque<size_t>::iterator>, "Dequeue iterator should be random access");
static_assert(std::ranges::random_access_range<Deque<size_t>>, "Deque should be a random access range");

TEST(Deque_Tests, Empty_Trivial_AfterDefaultConstruction_True)
{
  Deque<double> deque;

  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, Size_Trivial_AfterDefaultConstruction_Zero)
{
  Deque<double> deque;

  EXPECT_EQ(deque.size(), 0);
}

TEST(Deque_Tests, Capacity_Trivial_AfterDefaultConstruction_Zero)
{
  Deque<double> deque;

  EXPECT_EQ(deque.capacity(), 0);
}

TEST(Deque_Tests, ConstructorCArray_Trivial_EmptyCArray_Empty)
{
  Deque<double> deque({});

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, ConstructorCArray_NonTrivial_EmptyCArray_Empty)
{
  Deque<std::string> deque({});

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, ConstructorCArray_Trivial_OneElement_NotEmpty)
{
  Deque<double> deque({ 0.5 });

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, ConstructorCArray_Trivial_OneElement_CorrectValue)
{
  Deque<double> deque({ 0.5 });

  EXPECT_EQ(deque[0], 0.5);
}

TEST(Deque_Tests, ConstructorCArray_NonTrivial_OneElement_NotEmpty)
{
  Deque<std::string> deque({ std::string { "0.5" } });

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, ConstructorCArray_NonTrivial_OneElement_CorrectValue)
{
  Deque<std::string> deque({ std::string { "0.5" } });

  EXPECT_EQ(deque[0], std::string { "0.5" });
}

TEST(Deque_Tests, ConstructorCArray_Trivial_ManyElements_CorrectValues)
{
  Deque<double> deque({ 0.5, 0.4, 0.1 });

  EXPECT_FALSE(deque.empty());
  EXPECT_EQ(deque.size(), 3);

  EXPECT_EQ(deque[0], 0.5);
  EXPECT_EQ(deque[1], 0.4);
  EXPECT_EQ(deque[2], 0.1);
}

TEST(Deque_Tests, ConstructorIterator_Trivial_ManyElements_CorrectValues)
{
  Deque<double> values({ 0.5, 0.4, 0.1 });
  Deque<double> deque(values.begin(), values.end());

  EXPECT_FALSE(deque.empty());
  EXPECT_EQ(deque.size(), 3);

  EXPECT_EQ(deque[0], 0.5);
  EXPECT_EQ(deque[1], 0.4);
  EXPECT_EQ(deque[2], 0.1);
}

TEST(Deque_Tests, ConstructorRanges_Trivial_ManyElements_CorrectValues)
{
  Deque<double> values({ 0.5, 0.4, 0.1 });

  Deque<double> deque(values);

  EXPECT_FALSE(deque.empty());
  EXPECT_EQ(deque.size(), 3);

  EXPECT_EQ(deque[0], 0.5);
  EXPECT_EQ(deque[1], 0.4);
  EXPECT_EQ(deque[2], 0.1);
}

TEST(Deque_Tests, PushFront_Trivial_Single_SizeIncrease)
{
  Deque<double> deque;

  deque.push_front(0);

  EXPECT_EQ(deque.size(), 1);
}

TEST(Deque_Tests, PushFront_NonTrivial_Single_SizeIncrease)
{
  Deque<std::string> deque;

  deque.push_front("0");

  EXPECT_EQ(deque.size(), 1);
}

TEST(Deque_Tests, PushFront_Trivial_Double_SizeIncrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.push_front(0);

  EXPECT_EQ(deque.size(), 2);
}

TEST(Deque_Tests, PushFront_NonTrivial_Double_SizeIncrease)
{
  Deque<std::string> deque;

  deque.push_front("0");
  deque.push_front("0");

  EXPECT_EQ(deque.size(), 2);
}

TEST(Deque_Tests, PushFront_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 1000;

  Deque<double> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_front(static_cast<double>(0));
  }

  EXPECT_EQ(deque.size(), cAmount);
}

TEST(Deque_Tests, PushFront_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 1000;

  Deque<std::string> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_front(std::to_string(i));
  }

  EXPECT_EQ(deque.size(), cAmount);
}

TEST(Deque_Tests, PushBack_Trivial_Single_SizeIncrease)
{
  Deque<double> deque;

  deque.push_back(0);

  EXPECT_EQ(deque.size(), 1);
}

TEST(Deque_Tests, PushBack_NonTrivial_Single_SizeIncrease)
{
  Deque<std::string> deque;

  deque.push_back("0");

  EXPECT_EQ(deque.size(), 1);
}

TEST(Deque_Tests, PushBack_Trivial_Double_SizeIncrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.push_back(0);

  EXPECT_EQ(deque.size(), 2);
}

TEST(Deque_Tests, PushBack_NonTrivial_Double_SizeIncrease)
{
  Deque<std::string> deque;

  deque.push_back("0");
  deque.push_back("0");

  EXPECT_EQ(deque.size(), 2);
}

TEST(Deque_Tests, PushBack_Trivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 1000;

  Deque<double> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(static_cast<double>(0));
  }

  EXPECT_EQ(deque.size(), cAmount);
}

TEST(Deque_Tests, PushBack_NonTrivial_Many_SizeIncrease)
{
  constexpr size_t cAmount = 1000;

  Deque<std::string> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(std::to_string(i));
  }

  EXPECT_EQ(deque.size(), cAmount);
}

TEST(Deque_Tests, PushBack_Trivial_Single_CorrectValue)
{
  Deque<double> deque;

  const double value = 10;

  deque.push_back(value);

  EXPECT_EQ(deque[0], value);
}

TEST(Deque_Tests, PushBack_NonTrivial_Single_CorrectValue)
{
  Deque<std::string> deque;

  const std::string value = "10";

  deque.push_back(value);

  EXPECT_EQ(deque[0], value);
}

TEST(Deque_Tests, PushBack_Trivial_Double_CorrectValues)
{
  Deque<double> deque;

  deque.push_back(10);
  deque.push_back(11);

  EXPECT_EQ(deque[0], 10);
  EXPECT_EQ(deque[1], 11);
}

TEST(Deque_Tests, PushBack_NonTrivial_Double_CorrectValues)
{
  Deque<std::string> deque;

  deque.push_back("10");
  deque.push_back("11");

  EXPECT_EQ(deque[0], std::string { "10" });
  EXPECT_EQ(deque[1], std::string { "11" });
}

TEST(Deque_Tests, PushBack_Trivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 1000;

  Deque<double> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(static_cast<double>(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(deque[i], static_cast<double>(i));
  }
}

TEST(Deque_Tests, PushBack_NonTrivial_Many_CorrectValues)
{
  constexpr size_t cAmount = 1000;

  Deque<std::string> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(std::to_string(i));
  }

  for (size_t i = 0; i < cAmount; i++)
  {
    EXPECT_EQ(deque[i], std::to_string(i));
  }
}

TEST(Deque_Tests, EmplaceBack_NonTrivial_Single_SizeIncrease)
{
  Deque<std::pair<int, int>> deque;

  deque.emplace_back(1, 2);

  EXPECT_EQ(deque.size(), 1);
}

TEST(Deque_Tests, EmplaceBack_NonTrivial_Single_CorrectValue)
{
  Deque<std::pair<int, int>> deque;

  deque.emplace_back(99, 10);

  EXPECT_EQ(deque[0].first, 99);
  EXPECT_EQ(deque[0].second, 10);
}

TEST(Deque_Tests, PopFront_AfterPushFront_Single_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.pop_front();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopFront_AfterPushBack_Single_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.pop_front();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopBack_AfterPushBack_Single_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.pop_back();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopBack_AfterPushFront_Single_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.pop_back();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, Clear_Trivial_Empty)
{
  Deque<double> deque;

  deque.push_back(10);
  deque.push_back(11);

  deque.clear();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, Clear_NonTrivial_empty)
{
  Deque<std::string> deque;

  deque.push_back("10");
  deque.push_back("11");

  deque.clear();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, Reserve_Trivial_empty_CorrectCapacity)
{
  Deque<double> deque;

  deque.reserve(10);

  EXPECT_GE(deque.capacity(), 10);
}

TEST(Deque_Tests, Reserve_NonTrivial_empty_CorrectCapacity)
{
  Deque<std::string> deque;

  deque.reserve(10);

  EXPECT_GE(deque.capacity(), 10);
}

TEST(Deque_Tests, Reserve_Trivial_Increase_CorrectCapacity)
{
  Deque<double> deque;

  deque.reserve(5);

  EXPECT_GE(deque.capacity(), 5);

  deque.reserve(10);

  EXPECT_GE(deque.capacity(), 10);
}

TEST(Deque_Tests, Reserve_NonTrivial_Increase_CorrectCapacity)
{
  Deque<std::string> deque;

  deque.reserve(5);

  EXPECT_GE(deque.capacity(), 5);

  deque.reserve(10);

  EXPECT_GE(deque.capacity(), 10);
}

TEST(Deque_Tests, Reserve_Trivial_Decrease_DoNothing)
{
  Deque<double> deque;

  deque.reserve(10);

  size_t last_capacity = deque.capacity();

  EXPECT_GE(last_capacity, 10);

  deque.reserve(5);

  EXPECT_EQ(last_capacity, deque.capacity());
}

TEST(Deque_Tests, Reserve_NonTrivial_Decrease_DoNothing)
{
  Deque<std::string> deque;

  deque.reserve(10);

  size_t last_capacity = deque.capacity();

  EXPECT_GE(last_capacity, 10);

  deque.reserve(5);

  EXPECT_EQ(last_capacity, deque.capacity());
}

TEST(Deque_Tests, PopFront_NonTrivial_Single_sizeDecrease)
{
  Deque<std::string> deque;

  deque.push_front("0");
  deque.pop_front();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopBack_NonTrivial_Single_sizeDecrease)
{
  Deque<std::string> deque;

  deque.push_back("0");
  deque.pop_back();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopFront_DoublePushFront_DoublePopFront_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.push_front(0);
  deque.pop_front();
  deque.pop_front();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopBack_DoublePushBack_DoublePopBack_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.push_back(0);
  deque.pop_back();
  deque.pop_back();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopFront_DoublePushFront_PopFrontThenBack_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.push_front(0);
  deque.pop_front();
  deque.pop_back();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopBack_DoublePushBack_PopBackThenFrom_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.push_back(0);
  deque.pop_back();
  deque.pop_front();

  EXPECT_EQ(deque.size(), 0);
  EXPECT_TRUE(deque.empty());
}

TEST(Deque_Tests, PopFront_DoublePushFront_SinglePopFront_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.push_front(0);
  deque.pop_front();

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, PopBack_DoublePushBack_SinglePopBack_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.push_back(0);
  deque.pop_back();

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, PopFront_PushFrontThenBack_PopFront_SizeDecrease)
{
  Deque<double> deque;

  deque.push_front(0);
  deque.push_back(0);
  deque.pop_front();

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, PopBack_PushBackThenFrom_PopBack_SizeDecrease)
{
  Deque<double> deque;

  deque.push_back(0);
  deque.push_front(0);
  deque.pop_back();

  EXPECT_EQ(deque.size(), 1);
  EXPECT_FALSE(deque.empty());
}

TEST(Deque_Tests, PushFront_AfterPop_CorrectState)
{
  Deque<int> deque;

  deque.push_front(1);
  deque.push_front(2);
  deque.push_front(3);
  deque.pop_front();
  deque.push_front(4);

  EXPECT_EQ(deque.size(), 3);
  EXPECT_FALSE(deque.empty());

  EXPECT_EQ(deque[0], 4);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(deque[2], 1);
}

TEST(Deque_Tests, PushBack_AfterPop_CorrectState)
{
  Deque<int> deque;

  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  deque.pop_back();
  deque.push_back(4);

  EXPECT_EQ(deque.size(), 3);
  EXPECT_FALSE(deque.empty());

  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(deque[2], 4);
}

TEST(Deque_Tests, Front_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Deque<double> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(static_cast<double>(i));
  }

  EXPECT_EQ(deque.front(), 0);
}

TEST(Deque_Tests, Back_Trivial_CorrectValue)
{
  constexpr size_t cAmount = 20;

  Deque<double> deque;

  for (size_t i = 0; i < cAmount; i++)
  {
    deque.push_back(static_cast<double>(i));
  }

  EXPECT_EQ(deque.back(), cAmount - 1);
}

TEST(Deque_Tests, MoveConstructor_Trivial_CorrectValues)
{
  Deque<double> deque;

  deque.push_back(1);
  deque.push_back(2);

  Deque<double> copy { std::move(deque) };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 0);
  EXPECT_EQ(deque.capacity(), 0);

  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Deque_Tests, MoveConstructor_NonTrivial_CorrectValues)
{
  Deque<std::string> deque;

  deque.push_back("1");
  deque.push_back("2");

  Deque<std::string> copy { std::move(deque) };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 0);
  EXPECT_EQ(deque.capacity(), 0);

  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Deque_Tests, MoveAssignment_Trivial_CorrectValues)
{
  Deque<double> deque;

  deque.push_back(1);
  deque.push_back(2);

  Deque<double> moved = std::move(deque);

  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(deque.size(), 0);
  EXPECT_EQ(deque.capacity(), 0);

  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
}

TEST(Deque_Tests, MoveAssignment_NonTrivial_CorrectValues)
{
  Deque<std::string> deque;

  deque.push_back("1");
  deque.push_back("2");

  Deque<std::string> moved = std::move(deque);

  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(deque.size(), 0);
  EXPECT_EQ(deque.capacity(), 0);

  EXPECT_EQ(moved[0], std::string { "1" });
  EXPECT_EQ(moved[1], std::string { "2" });
}

TEST(Deque_Tests, MoveAssignment_SelfMove_DoNothing)
{
  Deque<int> deque;

  deque.push_back(1);
  deque.push_back(2);

  deque = std::move(deque);

  EXPECT_EQ(deque.size(), 2);
  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
}

TEST(Deque_Tests, CopyConstructor_Trivial_CorrectValues)
{
  Deque<double> deque;

  deque.push_back(1);
  deque.push_back(2);

  Deque<double> copy { deque };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 2);

  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Deque_Tests, CopyConstructor_NonTrivial_CorrectValues)
{
  Deque<std::string> deque;

  deque.push_back("1");
  deque.push_back("2");

  Deque<std::string> copy { deque };

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 2);

  EXPECT_EQ(deque[0], std::string { "1" });
  EXPECT_EQ(deque[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Deque_Tests, CopyAssignment_Trivial_CorrectValues)
{
  Deque<double> deque;

  deque.push_back(1);
  deque.push_back(2);

  Deque<double> copy = deque;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 2);

  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
}

TEST(Deque_Tests, CopyAssignment_NonTrivial_CorrectValues)
{
  Deque<std::string> deque;

  deque.push_back("1");
  deque.push_back("2");

  Deque<std::string> copy = deque;

  EXPECT_EQ(copy.size(), 2);
  EXPECT_EQ(deque.size(), 2);

  EXPECT_EQ(deque[0], std::string { "1" });
  EXPECT_EQ(deque[1], std::string { "2" });
  EXPECT_EQ(copy[0], std::string { "1" });
  EXPECT_EQ(copy[1], std::string { "2" });
}

TEST(Deque_Tests, CopyAssignment_SelfAssignement_DoNothing)
{
  Deque<int> deque;

  deque.push_back(1);
  deque.push_back(2);

  deque = deque;

  EXPECT_EQ(deque.size(), 2);
  EXPECT_EQ(deque[0], 1);
  EXPECT_EQ(deque[1], 2);
}

TEST(Deque_Tests, Equality_empty_Equal)
{
  Deque<int> deque1;

  Deque<int> deque2;

  EXPECT_EQ(deque1, deque2);
}

TEST(Deque_Tests, Equality_SelfEquality_Equal)
{
  Deque<int> deque;

  deque.push_back(1);
  deque.push_back(2);

  EXPECT_EQ(deque, deque);
}

TEST(Deque_Tests, Equality_SameValues_Equal)
{
  Deque<int> deque1;

  deque1.push_back(1);
  deque1.push_back(2);
  deque1.push_back(3);

  Deque<int> deque2;

  deque2.push_back(1);
  deque2.push_back(2);
  deque2.push_back(3);

  EXPECT_EQ(deque1, deque2);
}

TEST(Deque_Tests, Inequality_Differentsize_NotEqual)
{
  Deque<int> deque1;

  deque1.push_back(1);
  deque1.push_back(2);

  Deque<int> deque2;

  deque2.push_back(1);
  deque2.push_back(2);
  deque2.push_back(3);

  EXPECT_NE(deque1, deque2);
}

TEST(Deque_Tests, Inequality_DifferentValues_NotEqual)
{
  Deque<int> deque1;

  deque1.push_back(1);
  deque1.push_back(4);
  deque1.push_back(3);

  Deque<int> deque2;

  deque2.push_back(1);
  deque2.push_back(2);
  deque2.push_back(3);

  EXPECT_NE(deque1, deque2);
}

TEST(Deque_Tests, Iterator_Increment_Contigious_CorrectState)
{
  Deque<int> deque;

  deque.reserve(100);

  auto capacity = deque.capacity();

  for (int i = 0; i < capacity; i++)
  {
    deque.push_back(i);
  }

  auto it = deque.begin();

  for (int i = 0; i < capacity; i++, it++)
  {
    EXPECT_EQ(*it, i);
  }
}

TEST(Deque_Tests, Iterator_Increment_NotContigious_CorrectState)
{
  Deque<int> deque;

  deque.reserve(100);

  auto capacity = static_cast<int>(deque.capacity());

  for (int i = 0; i < capacity; i++)
  {
    deque.push_back(i);
  }

  for (int i = 0; i < 10; i++)
  {
    deque.pop_front();
  }

  for (int i = 0; i < 10; i++)
  {
    deque.push_back(i + capacity);
  }

  auto it = deque.begin();

  for (int i = 0; i < capacity; i++, it++)
  {
    EXPECT_EQ(*it, i + 10);
  }
}
} // namespace plex::tests