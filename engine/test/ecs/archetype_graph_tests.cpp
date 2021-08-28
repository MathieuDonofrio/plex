#include "genebits/engine/ecs/archetype_graph.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(ArchetypeGraph_Tests, AssureArchetype_Single_UniqueId)
{
  ArchetypeGraph graph;

  ASSERT_NE(graph.AssureArchetype<>(), graph.AssureArchetype<int>());
  ASSERT_NE(graph.AssureArchetype<double>(), graph.AssureArchetype<int>());
}

TEST(ArchetypeGraph_Tests, AssureArchetype_SingleTwice_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ(graph.AssureArchetype<int>(), graph.AssureArchetype<int>());
  ASSERT_EQ(graph.AssureArchetype<float>(), graph.AssureArchetype<float>());
}

TEST(ArchetypeGraph_Tests, AssureArchetype_MultipleSameOrder_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ((graph.AssureArchetype<int, double>()), (graph.AssureArchetype<int, double>()));
  ASSERT_EQ((graph.AssureArchetype<int, double, float>()), (graph.AssureArchetype<int, double, float>()));
}

TEST(ArchetypeGraph_Tests, AssureArchetype_MultipleDifferentOrder_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ((graph.AssureArchetype<int, double>()), (graph.AssureArchetype<double, int>()));
  ASSERT_EQ((graph.AssureArchetype<float, double, int>()), (graph.AssureArchetype<int, float, double>()));
}

TEST(ArchetypeGraph_Tests, AssureArchetype_MultipleDifferentValues_DifferentIds)
{
  ArchetypeGraph graph;

  ASSERT_NE((graph.AssureArchetype<int, double>()), (graph.AssureArchetype<int>()));
  ASSERT_NE((graph.AssureArchetype<int, float>()), (graph.AssureArchetype<double, int>()));
  ASSERT_NE((graph.AssureArchetype<bool, double, int>()), (graph.AssureArchetype<int, float, double>()));
}

TEST(ArchetypeGraph_Tests, AssureView_Single_UniqueId)
{
  ArchetypeGraph graph;

  ASSERT_NE(graph.AssureView<>(), graph.AssureView<int>());
  ASSERT_NE(graph.AssureView<double>(), graph.AssureView<int>());
}

TEST(ArchetypeGraph_Tests, AssureView_SingleTwice_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ(graph.AssureView<int>(), graph.AssureView<int>());
  ASSERT_EQ(graph.AssureView<float>(), graph.AssureView<float>());
}

TEST(ArchetypeGraph_Tests, AssureView_MultipleSameOrder_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ((graph.AssureView<int, double>()), (graph.AssureView<int, double>()));
  ASSERT_EQ((graph.AssureView<int, double, float>()), (graph.AssureView<int, double, float>()));
}

TEST(ArchetypeGraph_Tests, AssureView_MultipleDifferentOrder_SameId)
{
  ArchetypeGraph graph;

  ASSERT_EQ((graph.AssureView<int, double>()), (graph.AssureView<double, int>()));
  ASSERT_EQ((graph.AssureView<float, double, int>()), (graph.AssureView<int, float, double>()));
}

TEST(ArchetypeGraph_Tests, AssureView_MultipleDifferentValues_DifferentIds)
{
  ArchetypeGraph graph;

  ASSERT_NE((graph.AssureView<int, double>()), (graph.AssureView<int>()));
  ASSERT_NE((graph.AssureView<int, float>()), (graph.AssureView<double, int>()));
  ASSERT_NE((graph.AssureView<bool, double, int>()), (graph.AssureView<int, float, double>()));
}

TEST(ArchetypeGraph_Tests, ViewArchetypes_Multiple_CorrectSize)
{
  ArchetypeGraph graph;

  graph.AssureArchetype<int>();
  graph.AssureArchetype<float>();
  graph.AssureArchetype<bool>();
  graph.AssureArchetype<double>();
  graph.AssureArchetype<int, float>();
  graph.AssureArchetype<double, float>();
  graph.AssureArchetype<float, double>();
  graph.AssureArchetype<float, double, int>();
  graph.AssureArchetype<double, float, int>();
  graph.AssureArchetype<int, float, double>();
  graph.AssureArchetype<bool, double, int>();

  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<>()).Size(), 8);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<int>()).Size(), 4);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<float>()).Size(), 4);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<double>()).Size(), 4);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<bool>()).Size(), 2);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<bool>()).Size(), 2);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<int, float>()).Size(), 2);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<double, int>()).Size(), 2);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<int, double>()).Size(), 2);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<float, double, int>()).Size(), 1);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<double, float, int>()).Size(), 1);
}

TEST(ArchetypeGraph_Tests, ViewArchetypes_AfterView_CorrectSize)
{
  ArchetypeGraph graph;

  graph.AssureArchetype<int>();
  graph.AssureArchetype<float>();
  graph.AssureArchetype<bool>();
  graph.AssureArchetype<double>();

  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<>()).Size(), 4);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<int>()).Size(), 1);

  graph.AssureArchetype<int, float>();
  graph.AssureArchetype<double, float>();
  graph.AssureArchetype<float, double, int>();
  graph.AssureArchetype<bool, double, int>();

  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<>()).Size(), 8);
  ASSERT_EQ(graph.ViewArchetypes(graph.AssureView<int>()).Size(), 4);
}

TEST(ArchetypeGraph_Tests, ViewArchetypes_Multiple_CorrectArchetypes)
{
  ArchetypeGraph graph;

  FastVector<ArchetypeId> archetypes;
  FastVector<ArchetypeId> view_archetypes;

  archetypes.PushBack(graph.AssureArchetype<int>());

  graph.AssureArchetype<float>();
  graph.AssureArchetype<bool>();
  graph.AssureArchetype<double>();

  archetypes.PushBack(graph.AssureArchetype<int, float>());

  graph.AssureArchetype<double, float>();

  size_t view = graph.AssureView<int>();

  archetypes.PushBack(graph.AssureArchetype<float, double, int>());
  archetypes.PushBack(graph.AssureArchetype<bool, double, int>());

  view_archetypes = graph.ViewArchetypes(view);

  ASSERT_EQ(view_archetypes.Size(), archetypes.Size());

  std::sort(archetypes.begin(), archetypes.end());
  std::sort(view_archetypes.begin(), view_archetypes.end());

  ASSERT_TRUE(std::includes(archetypes.begin(), archetypes.end(), view_archetypes.begin(), view_archetypes.end()));
}
} // namespace genebits::engine::tests