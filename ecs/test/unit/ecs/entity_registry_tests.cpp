#include "plex/ecs/entity_registry.h"

#include <gtest/gtest.h>

namespace plex::tests
{
TEST(EntityRegistry_Tests, EntityCount_AfterInitialization_Zero)
{
  EntityRegistry registry;

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
}

TEST(EntityRegistry_Tests, Create_AfterInitialization_IncreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(10);

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ(entity, 0u);
}

TEST(EntityRegistry_Tests, Create_Multiple_IncreaseEntityCount)
{
  EntityRegistry registry;

  for (size_t i = 0; i < 10; i++)
  {
    EXPECT_EQ(registry.Create<size_t>(size_t(i)), i);
  }

  EXPECT_EQ(registry.EntityCount(), 10);
  EXPECT_EQ(registry.EntityCount<size_t>(), 10);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
}

TEST(EntityRegistry_Tests, Create_MultipleEntitiesMultipleComponents_IncreaseEntityCount)
{
  EntityRegistry registry;

  auto entity1 = registry.Create<int, double, float>(10, 0.5, 0.2f);
  auto entity2 = registry.Create<int, float>(11, 0.8f);

  EXPECT_EQ(registry.EntityCount(), 2);
  EXPECT_EQ(registry.EntityCount<int>(), 2);
  EXPECT_EQ(registry.EntityCount<double>(), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 2);
  EXPECT_EQ((registry.EntityCount<int, float>()), 2);
  EXPECT_EQ((registry.EntityCount<int, double>()), 1);
  EXPECT_EQ((registry.EntityCount<int, double, float>()), 1);
  EXPECT_EQ((registry.EntityCount<bool>()), 0);
  EXPECT_EQ(entity1, 0u);
  EXPECT_EQ(entity2, 1u);
}

TEST(EntityRegistry_Tests, Destroy_Single_DecreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
}

TEST(EntityRegistry_Tests, Destroy_WithView_DecreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy<int>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
}

TEST(EntityRegistry_Tests, Destroy_WithEmptyView_DecreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(EntityRegistry_Tests, Destroy_WithPartialView_DecreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<double>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(EntityRegistry_Tests, Destroy_WithExactView_DecreaseEntityCount)
{
  EntityRegistry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(EntityRegistry_Tests, Create_AfterDestroy_Restore)
{
  EntityRegistry registry;

  auto entity1 = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity1);

  auto entity2 = registry.Create<int, double>(10, 0.5);

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<double>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 0);

  EXPECT_EQ(entity1, entity2);
}

TEST(EntityRegistry_Tests, DestroyAll_MultipleEntities_DecreaseEntityCount)
{
  EntityRegistry registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  EXPECT_EQ(registry.EntityCount(), 4);
  EXPECT_EQ(registry.EntityCount<int>(), 2);
  EXPECT_EQ((registry.EntityCount<double, int>()), 1);
  EXPECT_EQ((registry.EntityCount<double>()), 2);
  EXPECT_EQ((registry.EntityCount<float>()), 1);

  registry.DestroyAll();

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);
  EXPECT_EQ((registry.EntityCount<float>()), 0);
}

TEST(EntityRegistry_Tests, DestroyAll_WithView_DecreaseEntityCountCorrectly)
{
  EntityRegistry registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  registry.DestroyAll<double>();

  EXPECT_EQ(registry.EntityCount(), 2);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);

  registry.DestroyAll<int>();

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<float>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);

  registry.DestroyAll<float>();

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<float>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);
}

TEST(EntityRegistry_Tests, Unpack_Single_Correct)
{
  EntityRegistry registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  EXPECT_EQ(10, registry.Unpack<int>(created_entity));
  EXPECT_EQ(0.5, registry.Unpack<double>(created_entity));
}

TEST(EntityRegistry_Tests, Unpack_Modify_ModifiedValue)
{
  EntityRegistry registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  registry.Unpack<int>(created_entity) = 11;

  EXPECT_EQ(11, registry.Unpack<int>(created_entity));
  EXPECT_EQ(0.5, registry.Unpack<double>(created_entity));

  registry.Unpack<double>(created_entity) = 1.5;

  EXPECT_EQ(11, registry.Unpack<int>(created_entity));
  EXPECT_EQ(1.5, registry.Unpack<double>(created_entity));
}

TEST(EntityRegistry_Tests, HasComponents_Zero_False)
{
  EntityRegistry registry;

  auto created_entity = registry.Create<int>(10);

  EXPECT_FALSE(registry.HasComponents<double>(created_entity));
}

TEST(EntityRegistry_Tests, HasComponents_One_False)
{
  EntityRegistry registry;

  auto created_entity = registry.Create<int>(10);

  EXPECT_TRUE(registry.HasComponents<int>(created_entity));
}

TEST(EntityRegistry_Tests, HasComponents_Multiple_False)
{
  EntityRegistry registry;

  auto created_entity = registry.Create<int, double, float>(10, 0.5, 0.2f);

  EXPECT_TRUE(registry.HasComponents<int>(created_entity));
  EXPECT_TRUE(registry.HasComponents<double>(created_entity));
  EXPECT_TRUE(registry.HasComponents<float>(created_entity));
  EXPECT_FALSE(registry.HasComponents<bool>(created_entity));

  EXPECT_TRUE((registry.HasComponents<int, double>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<double, int>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<float, int>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<double, float>(created_entity)));
  EXPECT_FALSE((registry.HasComponents<double, bool>(created_entity)));
  EXPECT_FALSE((registry.HasComponents<int, bool>(created_entity)));

  EXPECT_TRUE((registry.HasComponents<int, double, float>(created_entity)));
}

TEST(ViewIterator_Tests, PreIncrement_Empty_NoIterations)
{
  EntityRegistry registry;

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 0);
}

TEST(ViewIterator_Tests, PreIncrement_Single_OneIteration)
{
  EntityRegistry registry;

  registry.Create<int>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PreIncrement_Double_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<double>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(ViewIterator_Tests, PreIncrement_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, PreIncrement_WithMultipleEntities_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<double>(0);
  registry.Create<double>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(ViewIterator_Tests, PostIncrement_Single_OneIteration)
{
  EntityRegistry registry;

  registry.Create<int>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); it++)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PostIncrement_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); it++)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, PreDecrement_Single_OneIteration)
{
  EntityRegistry registry;

  registry.Create<int>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = --view.end(); it != view.begin() - 1; --it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PostDecrement_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = --view.end(); it != view.begin() - 1; it--)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, AddAssign_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin();
  it += 3;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, SubtractAssign_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin();
  it += 3;
  it -= 2;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 3);
}

TEST(ViewIterator_Tests, Add_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin() + 3;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, Subtract_Multiple_CorrectIterations)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = (view.begin() + 3) - 2;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 3);
}

TEST(SubView_Tests, Contains_SingleExact_True)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(0);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_TRUE(mono_view.Contains(entity));
}

TEST(SubView_Tests, Contains_SingleDestroyed_False)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(0);
  registry.Destroy(entity);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_FALSE(mono_view.Contains(entity));
}

TEST(SubView_Tests, EntityCount_Multiple_CorrectEntityCount)
{
  EntityRegistry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_EQ(mono_view.Size(), 3);
}

TEST(SubView_Tests, Unpack_Single_CorrectValue)
{
  EntityRegistry registry;

  auto entity = registry.Create<int>(10);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_EQ(mono_view.Unpack<int>(entity), 10);
}

TEST(SubViewIterator_Tests, Dereference_Single_CorrectEntity)
{
  EntityRegistry registry;

  registry.Create<int>(99);

  SubView<int> mono_view = *registry.ViewFor<int>().begin();

  auto it = mono_view.begin();

  int value = (*mono_view.begin()).get<int>();

  EXPECT_EQ(value, 99);
}

TEST(SubViewIterator_Tests, PreIncrement_Double_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  int value1 = (*mono_view.begin()).get<int>();
  int value2 = (*(++mono_view.begin())).get<int>();

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PreDecrement_Double_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto it = ++sub_view.begin();

  int value1 = (*it).get<int>();
  int value2 = (*(--it)).get<int>();

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PostIncrement_Double_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto it = sub_view.begin();

  int value1 = (*it).get<int>();

  it++;

  int value2 = (*it).get<int>();

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PostDecrement_Double_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto value = ++sub_view.begin();

  int value1 = (*value).get<int>();

  value--;

  int value2 = (*(value)).get<int>();

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, AddAssign_Multiple_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto value = sub_view.begin();

  int value1 = (*value).get<int>();

  value += 2;

  int value2 = (*value).get<int>();

  EXPECT_EQ(value1 + value2, 4);
}

TEST(SubViewIterator_Tests, SubstractAssign_Multiple_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto value = ++(++sub_view.begin());

  int value1 = (*value).get<int>();

  value -= 2;

  int value2 = (*value).get<int>();

  EXPECT_EQ(value1 + value2, 4);
}

TEST(SubViewIterator_Tests, Add_Multiple_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView sub_view = *registry.ViewFor<int>().begin();

  int value1 = (*(sub_view.begin() + 1)).get<int>();
  int value2 = (*(sub_view.begin() + 2)).get<int>();

  EXPECT_EQ(value1 + value2, 5);
}

TEST(SubViewIterator_Tests, Subtract_Multiple_CorrectEntities)
{
  EntityRegistry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView sub_view = *registry.ViewFor<int>().begin();

  auto value = sub_view.begin() + 2;

  int value1 = (*(value - 1)).get<int>();
  int value2 = (*(value - 2)).get<int>();

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, CopyAssign_Single_CorrectEntity)
{
  EntityRegistry registry;

  registry.Create<int>(99);

  SubView sub_view = *registry.ViewFor<int>().begin();

  SubView<int>::iterator it = sub_view.begin();

  ++it;

  it = (it - 1);

  int value = (*it).get<int>();

  EXPECT_EQ(value, 99);
}

TEST(SubViewEntityIterator_Tests, Increment_Double_CorrectEntities)
{
  EntityRegistry registry;

  auto entity1 = registry.Create<int>(1);
  auto entity2 = registry.Create<int>(2);

  SubView sub_view = *registry.ViewFor<int>().begin();

  int iterations = 0;

  size_t last = 9999999;

  for (auto it = sub_view.ebegin(); it != sub_view.eend(); ++it)
  {
    auto entity = *it;

    EXPECT_NE(entity, last);

    EXPECT_TRUE(entity == entity1 || entity == entity2);

    last = entity;

    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(EntityForEach_Tests, SubView_Single_CorrectEntity)
{
  EntityRegistry registry;

  registry.Create<int>(99);

  SubView sub_view = *registry.ViewFor<int>().begin();

  size_t call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, 99);
      ++call_count;
    });

  EXPECT_EQ(call_count, 1);
}

TEST(EntityForEach_Tests, SubView_3_CorrectEntities)
{
  constexpr int amount = 3;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, SubView_4_CorrectEntities)
{
  constexpr int amount = 4;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, SubView_7_CorrectEntities)
{
  constexpr int amount = 7;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, SubView_8_CorrectEntities)
{
  constexpr int amount = 8;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, SubView_9_CorrectEntities)
{
  constexpr int amount = 9;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, SubView_65_CorrectEntities)
{
  constexpr int amount = 65;

  EntityRegistry registry;

  for (int i = 0; i < amount; i++)
  {
    registry.Create(i);
  }

  SubView sub_view = *registry.ViewFor<int>().begin();

  int call_count = 0;

  EntityForEach(sub_view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, amount);
}

TEST(EntityForEach_Tests, View_SingleArchetype_CorrectEntities)
{
  constexpr int arch1_amount = 2;

  constexpr int total_amount = arch1_amount;

  EntityRegistry registry;

  for (int i = 0; i < arch1_amount; i++)
  {
    registry.Create(i);
  }

  auto view = registry.ViewFor<int>();

  int call_count = 0;

  EntityForEach(view,
    [&](Entity, int value)
    {
      EXPECT_EQ(value, call_count);
      ++call_count;
    });

  EXPECT_EQ(call_count, total_amount);
}

TEST(EntityForEach_Tests, View_TwoArchetypes_CorrectEntities)
{
  constexpr int arch1_amount = 2;
  constexpr int arch2_amount = 9;

  constexpr int total_amount = arch1_amount + arch2_amount;

  EntityRegistry registry;

  for (int i = 0; i < arch1_amount; i++)
  {
    registry.Create(i);
  }

  for (int i = arch1_amount; i < total_amount; i++)
  {
    registry.Create(i, 0.5);
  }

  auto view = registry.ViewFor<int>();

  int call_count = 0;

  std::set<int> seen_entities;

  EntityForEach(view,
    [&](Entity, int value)
    {
      seen_entities.emplace(value);
      ++call_count;
    });

  for (int i = 0; i < total_amount; i++)
  {
    EXPECT_TRUE(seen_entities.contains(i));
  }

  EXPECT_EQ(call_count, total_amount);
  EXPECT_EQ(seen_entities.size(), total_amount);
}

TEST(EntityForEach_Tests, View_TwoArchetypesStructuredBindings_CorrectEntities)
{
  constexpr int arch1_amount = 2;
  constexpr int arch2_amount = 9;

  constexpr int total_amount = arch1_amount + arch2_amount;

  EntityRegistry registry;

  for (int i = 0; i < arch1_amount; i++)
  {
    registry.Create(i);
  }

  for (int i = arch1_amount; i < total_amount; i++)
  {
    registry.Create(i, 0.5);
  }

  auto view = registry.ViewFor<int>();

  int call_count = 0;

  std::set<int> seen_entities;

  for (auto sub_view : view)
  {
    for (auto [entity, value] : sub_view)
    {
      (void)value;
      seen_entities.emplace(entity);
      ++call_count;
    }
  }

  for (int i = 0; i < total_amount; i++)
  {
    EXPECT_TRUE(seen_entities.contains(i));
  }

  EXPECT_EQ(call_count, total_amount);
  EXPECT_EQ(seen_entities.size(), total_amount);
}
} // namespace plex::tests