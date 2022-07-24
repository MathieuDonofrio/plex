#include "genebits/engine/ecs/archetype.h"

#include <gtest/gtest.h>

namespace plex::tests
{
TEST(ViewRelations_Tests, AssureArchetype_Single_UniqueId)
{
  ViewRelations relations;

  EXPECT_NE(relations.AssureArchetype<>(), relations.AssureArchetype<int>());
  EXPECT_NE(relations.AssureArchetype<double>(), relations.AssureArchetype<int>());
}

TEST(ViewRelations_Tests, AssureArchetype_SingleTwice_SameId)
{
  ViewRelations relations;

  EXPECT_EQ(relations.AssureArchetype<int>(), relations.AssureArchetype<int>());
  EXPECT_EQ(relations.AssureArchetype<float>(), relations.AssureArchetype<float>());
}

TEST(ViewRelations_Tests, AssureArchetype_MultipleSameOrder_SameId)
{
  ViewRelations relations;

  EXPECT_EQ((relations.AssureArchetype<int, double>()), (relations.AssureArchetype<int, double>()));
  EXPECT_EQ((relations.AssureArchetype<int, double, float>()), (relations.AssureArchetype<int, double, float>()));
}

TEST(ViewRelations_Tests, AssureArchetype_MultipleDifferentOrder_SameId)
{
  ViewRelations relations;

  EXPECT_EQ((relations.AssureArchetype<int, double>()), (relations.AssureArchetype<double, int>()));
  EXPECT_EQ((relations.AssureArchetype<float, double, int>()), (relations.AssureArchetype<int, float, double>()));
}

TEST(ViewRelations_Tests, AssureArchetype_MultipleDifferentValues_DifferentIds)
{
  ViewRelations relations;

  EXPECT_NE((relations.AssureArchetype<int, double>()), (relations.AssureArchetype<int>()));
  EXPECT_NE((relations.AssureArchetype<int, float>()), (relations.AssureArchetype<double, int>()));
  EXPECT_NE((relations.AssureArchetype<bool, double, int>()), (relations.AssureArchetype<int, float, double>()));
}

TEST(ViewRelations_Tests, AssureView_Single_UniqueId)
{
  ViewRelations relations;

  EXPECT_NE(relations.AssureView<>(), relations.AssureView<int>());
  EXPECT_NE(relations.AssureView<double>(), relations.AssureView<int>());
}

TEST(ViewRelations_Tests, AssureView_SingleTwice_SameId)
{
  ViewRelations relations;

  EXPECT_EQ(relations.AssureView<int>(), relations.AssureView<int>());
  EXPECT_EQ(relations.AssureView<float>(), relations.AssureView<float>());
}

TEST(ViewRelations_Tests, AssureView_MultipleSameOrder_SameId)
{
  ViewRelations relations;

  EXPECT_EQ((relations.AssureView<int, double>()), (relations.AssureView<int, double>()));
  EXPECT_EQ((relations.AssureView<int, double, float>()), (relations.AssureView<int, double, float>()));
}

TEST(ViewRelations_Tests, AssureView_MultipleDifferentOrder_SameId)
{
  ViewRelations relations;

  EXPECT_EQ((relations.AssureView<int, double>()), (relations.AssureView<double, int>()));
  EXPECT_EQ((relations.AssureView<float, double, int>()), (relations.AssureView<int, float, double>()));
}

TEST(ViewRelations_Tests, AssureView_MultipleDifferentValues_DifferentIds)
{
  ViewRelations relations;

  EXPECT_NE((relations.AssureView<int, double>()), (relations.AssureView<int>()));
  EXPECT_NE((relations.AssureView<int, float>()), (relations.AssureView<double, int>()));
  EXPECT_NE((relations.AssureView<bool, double, int>()), (relations.AssureView<int, float, double>()));
}

TEST(ViewRelations_Tests, ViewArchetypes_Multiple_CorrectSize)
{
  ViewRelations relations;

  relations.AssureArchetype<int>();
  relations.AssureArchetype<float>();
  relations.AssureArchetype<bool>();
  relations.AssureArchetype<double>();
  relations.AssureArchetype<int, float>();
  relations.AssureArchetype<double, float>();
  relations.AssureArchetype<float, double>();
  relations.AssureArchetype<float, double, int>();
  relations.AssureArchetype<double, float, int>();
  relations.AssureArchetype<int, float, double>();
  relations.AssureArchetype<bool, double, int>();

  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<>()).size(), 8);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<int>()).size(), 4);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<float>()).size(), 4);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<double>()).size(), 4);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<bool>()).size(), 2);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<bool>()).size(), 2);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<int, float>()).size(), 2);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<double, int>()).size(), 2);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<int, double>()).size(), 2);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<float, double, int>()).size(), 1);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<double, float, int>()).size(), 1);
}

TEST(ViewRelations_Tests, ViewArchetypes_AfterView_CorrectSize)
{
  ViewRelations relations;

  relations.AssureArchetype<int>();
  relations.AssureArchetype<float>();
  relations.AssureArchetype<bool>();
  relations.AssureArchetype<double>();

  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<>()).size(), 4);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<int>()).size(), 1);

  relations.AssureArchetype<int, float>();
  relations.AssureArchetype<double, float>();
  relations.AssureArchetype<float, double, int>();
  relations.AssureArchetype<bool, double, int>();

  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<>()).size(), 8);
  EXPECT_EQ(relations.ViewArchetypes(relations.AssureView<int>()).size(), 4);
}

TEST(ViewRelations_Tests, ViewArchetypes_Multiple_CorrectArchetypes)
{
  ViewRelations relations;

  Vector<ArchetypeId> archetypes;
  Vector<ArchetypeId> view_archetypes;

  archetypes.push_back(relations.AssureArchetype<int>());

  relations.AssureArchetype<float>();
  relations.AssureArchetype<bool>();
  relations.AssureArchetype<double>();

  archetypes.push_back(relations.AssureArchetype<int, float>());

  relations.AssureArchetype<double, float>();

  ViewId view = relations.AssureView<int>();

  archetypes.push_back(relations.AssureArchetype<float, double, int>());
  archetypes.push_back(relations.AssureArchetype<bool, double, int>());

  view_archetypes = relations.ViewArchetypes(view);

  EXPECT_EQ(view_archetypes.size(), archetypes.size());

  std::sort(archetypes.begin(), archetypes.end());
  std::sort(view_archetypes.begin(), view_archetypes.end());

  EXPECT_TRUE(std::includes(archetypes.begin(), archetypes.end(), view_archetypes.begin(), view_archetypes.end()));
}
} // namespace plex::tests