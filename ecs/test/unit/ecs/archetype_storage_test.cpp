#include "plex/ecs/archetype_storage.h"

#include <algorithm>

#include <gtest/gtest.h>

namespace plex::tests
{
TEST(ArchetypeStorage_Tests, Empty_AfterInitialization_True)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Insert_Single_SizeIncrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);
}

TEST(ArchetypeStorage_Tests, Contains_WithoutValue_False)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  EXPECT_FALSE(storage.Contains(0));
}

TEST(ArchetypeStorage_Tests, Contains_WithValue_False)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);

  EXPECT_TRUE(storage.Contains(0));
  EXPECT_FALSE(storage.Contains(1));
}

TEST(ArchetypeStorage_Tests, Contains_LargeNonExistant_False)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  EXPECT_FALSE(storage.Contains(1000000));
}

TEST(ArchetypeStorage_Tests, Erase_Single_SizeDecrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);
  storage.Erase(0);

  EXPECT_FALSE(storage.Contains(0));
}

TEST(ArchetypeStorage_Tests, Clear_Empty_Empty)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Clear();

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Clear_NotEmpty_Empty)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);
  storage.Clear();

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Insert_Double_IncreaseSize)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);
  storage.Insert(1);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 2);
}

TEST(ArchetypeStorage_Tests, Erase_Double_DecreaseSize)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);
  storage.Insert(1);

  storage.Erase(1);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);

  storage.Erase(0);

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Insert_ReinsertionAfterClear_RestoredState)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  storage.Insert(0);
  storage.Clear();
  storage.Insert(0);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);
}

TEST(ArchetypeStorage_Tests, Insert_TriggerSparseGrowth_CorrectState)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  const size_t big_value = 10000;

  storage.Insert(big_value);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);
  EXPECT_TRUE(storage.Contains(big_value));
  EXPECT_FALSE(storage.Contains(0));
}

TEST(ArchetypeStorage_Tests, Insert_TriggerDenseGrowth_CorrectState)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<>();

  const size_t amount = 10000;

  for (size_t i = 0; i < amount; i++)
  {
    storage.Insert(i);
  }

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), amount);

  for (size_t i = 0; i < amount; i++)
  {
    EXPECT_TRUE(storage.Contains(i));
  }

  EXPECT_FALSE(storage.Contains(amount));
}

TEST(ArchetypeStorage_Tests, Insert_WithComponent_SizeIncrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int>();

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);

  storage.Insert(0, 10);

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);
}

TEST(ArchetypeStorage_Tests, Insert_WithComponentNonTrivial_SizeIncrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<std::string>();

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);

  storage.Insert(0, std::string { "10" });

  EXPECT_FALSE(storage.Empty());
  EXPECT_EQ(storage.Size(), 1);
}

TEST(ArchetypeStorage_Tests, Insert_WithComponent_CorrectValue)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int>();

  storage.Insert(0, 10);

  EXPECT_EQ(storage.Unpack<int>(0), 10);
}

TEST(ArchetypeStorage_Tests, Insert_WithComponentRef_CorrectValue)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int>();

  int data = 10;

  storage.Insert(0, data);

  EXPECT_EQ(storage.Unpack<int>(0), 10);
}

TEST(ArchetypeStorage_Tests, Insert_WithComponentNonTrivial_CorrectValue)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<std::string>();

  storage.Insert(0, std::string { "10" });

  EXPECT_EQ(storage.Unpack<std::string>(0), "10");
}

TEST(ArchetypeStorage_Tests, Insert_WithMultipleComponents_CorrectValue)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int, std::string>();

  storage.Insert(0, 10, std::string { "11" });

  EXPECT_EQ(storage.Unpack<int>(0), 10);
  EXPECT_EQ(storage.Unpack<std::string>(0), "11");
}

TEST(ArchetypeStorage_Tests, Erase_WithComponent_SizeDecrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int>();

  storage.Insert(0, 10);
  storage.Erase(0);

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Erase_WithComponentNonTrivial_SizeDecrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<std::string>();

  storage.Insert(0, std::string { "10" });
  storage.Erase(0);

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Insert_WithMultipleComponents_SizeDecrease)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int, std::string>();

  storage.Insert(0, 10, std::string { "11" });
  storage.Erase(0);

  EXPECT_TRUE(storage.Empty());
  EXPECT_EQ(storage.Size(), 0);
}

TEST(ArchetypeStorage_Tests, Erase_MultipleEraseAfterMultipleInsert_CorrectState)
{
  ArchetypeStorageSparseArray<size_t> sparse;
  ArchetypeStorage<size_t> storage(&sparse);
  storage.Initialize<int>();

  storage.Insert<int>(0, 0);
  storage.Insert<int>(1, 10);
  storage.Insert<int>(2, 20);

  EXPECT_EQ(storage.Size(), 3);
  EXPECT_FALSE(storage.Contains(1000));

  storage.Erase(0);
  storage.Erase(1);

  EXPECT_EQ(storage.Size(), 1);

  EXPECT_TRUE(storage.Contains(2));
  EXPECT_EQ(storage.Unpack<int>(2), 20);

  EXPECT_FALSE(storage.Contains(0));
  EXPECT_FALSE(storage.Contains(1));
}

} // namespace plex::tests