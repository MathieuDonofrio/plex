#include "genebits/engine/ecs/storage.h"

#include <algorithm>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Storage_Tests, Empty_AfterInitialization_True)
{
  Storage<size_t> storage;

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Insert_Single_SizeIncrease)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);
}

TEST(Storage_Tests, Contains_WithoutValue_False)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  ASSERT_FALSE(storage.Contains(0));
}

TEST(Storage_Tests, Contains_WithValue_False)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);

  ASSERT_TRUE(storage.Contains(0));
  ASSERT_FALSE(storage.Contains(1));
}

TEST(Storage_Tests, Contains_LargeNonExistant_False)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  ASSERT_FALSE(storage.Contains(1000000));
}

TEST(Storage_Tests, Erase_Single_SizeDecrease)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);
  storage.Erase(0);

  ASSERT_FALSE(storage.Contains(0));
}

TEST(Storage_Tests, Clear_Empty_Empty)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Clear();

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Clear_NotEmpty_Empty)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);
  storage.Clear();

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Insert_Double_IncreaseSize)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);
  storage.Insert(1);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 2);
}

TEST(Storage_Tests, Erase_Double_DecreaseSize)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);
  storage.Insert(1);

  storage.Erase(1);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);

  storage.Erase(0);

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Insert_ReinsertionAfterClear_RestoredState)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  storage.Insert(0);
  storage.Clear();
  storage.Insert(0);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);
}

TEST(Storage_Tests, Insert_TriggerSparseGrowth_CorrectState)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  const size_t big_value = 10000;

  storage.Insert(big_value);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);
  ASSERT_TRUE(storage.Contains(big_value));
  ASSERT_FALSE(storage.Contains(0));
}

TEST(Storage_Tests, Insert_TriggerDenseGrowth_CorrectState)
{
  Storage<size_t> storage;
  storage.Initialize<>();

  const size_t amount = 10000;

  for (size_t i = 0; i < amount; i++)
  {
    storage.Insert(i);
  }

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), amount);

  for (size_t i = 0; i < amount; i++)
  {
    ASSERT_TRUE(storage.Contains(i));
  }

  ASSERT_FALSE(storage.Contains(amount));
}

TEST(Storage_Tests, Insert_WithComponent_SizeIncrease)
{
  Storage<size_t> storage;
  storage.Initialize<int>();

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);

  storage.Insert(0, 10);

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);
}

TEST(Storage_Tests, Insert_WithComponentNonTrivial_SizeIncrease)
{
  Storage<size_t> storage;
  storage.Initialize<std::string>();

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);

  storage.Insert(0, std::string { "10" });

  ASSERT_FALSE(storage.Empty());
  ASSERT_EQ(storage.Size(), 1);
}

TEST(Storage_Tests, Insert_WithComponent_CorrectValue)
{
  Storage<size_t> storage;
  storage.Initialize<int>();

  storage.Insert(0, 10);

  ASSERT_EQ(storage.Unpack<int>(0), 10);
}

TEST(Storage_Tests, Insert_WithComponentNonTrivial_CorrectValue)
{
  Storage<size_t> storage;
  storage.Initialize<std::string>();

  storage.Insert(0, std::string { "10" });

  ASSERT_EQ(storage.Unpack<std::string>(0), "10");
}

TEST(Storage_Tests, Insert_WithMultipleComponents_CorrectValue)
{
  Storage<size_t> storage;
  storage.Initialize<int, std::string>();

  storage.Insert(0, 10, std::string { "11" });

  ASSERT_EQ(storage.Unpack<int>(0), 10);
  ASSERT_EQ(storage.Unpack<std::string>(0), "11");
}

TEST(Storage_Tests, Erase_WithComponent_SizeDecrease)
{
  Storage<size_t> storage;
  storage.Initialize<int>();

  storage.Insert(0, 10);
  storage.Erase(0);

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Erase_WithComponentNonTrivial_SizeDecrease)
{
  Storage<size_t> storage;
  storage.Initialize<std::string>();

  storage.Insert(0, std::string { "10" });
  storage.Erase(0);

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Insert_WithMultipleComponents_SizeDecrease)
{
  Storage<size_t> storage;
  storage.Initialize<int, std::string>();

  storage.Insert(0, 10, std::string { "11" });
  storage.Erase(0);

  ASSERT_TRUE(storage.Empty());
  ASSERT_EQ(storage.Size(), 0);
}

TEST(Storage_Tests, Erase_MultipleEraseAfterMultipleInsert_CorrectState)
{
  Storage<size_t> storage;
  storage.Initialize<int>();

  storage.Insert<int>(0, 0);
  storage.Insert<int>(1, 10);
  storage.Insert<int>(2, 20);

  ASSERT_EQ(storage.Size(), 3);
  ASSERT_FALSE(storage.Contains(1000));

  storage.Erase(0);
  storage.Erase(1);

  ASSERT_EQ(storage.Size(), 1);

  EXPECT_TRUE(storage.Contains(2));
  EXPECT_EQ(storage.Unpack<int>(2), 20);

  EXPECT_FALSE(storage.Contains(0));
  EXPECT_FALSE(storage.Contains(1));
}

} // namespace genebits::engine::tests