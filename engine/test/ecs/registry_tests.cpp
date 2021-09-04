#include "genebits/engine/ecs/registry.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Registry_Tests, Size_AfterInitialization_Zero)
{
  Registry<size_t> registry;

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<double>(), 0);
}

TEST(Registry_Tests, Create_AfterInitialization_IncreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int>(10);

  ASSERT_EQ(registry.Size(), 1);
  ASSERT_EQ(registry.Size<int>(), 1);
  ASSERT_EQ(registry.Size<double>(), 0);
  ASSERT_EQ(entity, 0);
}

TEST(Registry_Tests, Create_Multiple_IncreaseSize)
{
  Registry<size_t> registry;

  for (size_t i = 0; i < 10; i++)
  {
    ASSERT_EQ(registry.Create<size_t>(size_t(i)), i);
  }

  ASSERT_EQ(registry.Size(), 10);
  ASSERT_EQ(registry.Size<size_t>(), 10);
  ASSERT_EQ(registry.Size<double>(), 0);
}

TEST(Registry_Tests, Create_MultipleEntitiesMultipleComponents_IncreaseSize)
{
  Registry<size_t> registry;

  auto entity1 = registry.Create<int, double, float>(10, 0.5, 0.2f);
  auto entity2 = registry.Create<int, float>(11, 0.8f);

  ASSERT_EQ(registry.Size(), 2);
  ASSERT_EQ(registry.Size<int>(), 2);
  ASSERT_EQ(registry.Size<double>(), 1);
  ASSERT_EQ(registry.Size<float>(), 2);
  ASSERT_EQ((registry.Size<int, float>()), 2);
  ASSERT_EQ((registry.Size<int, double>()), 1);
  ASSERT_EQ((registry.Size<int, double, float>()), 1);
  ASSERT_EQ((registry.Size<bool>()), 0);
  ASSERT_EQ(entity1, 0);
  ASSERT_EQ(entity2, 1);
}

TEST(Registry_Tests, Destroy_Single_DecreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy(entity);

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
}

TEST(Registry_Tests, Destroy_WithView_DecreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy<int>(entity);

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
}

TEST(Registry_Tests, Destroy_WithEmptyView_DecreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy(entity);

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<double>(), 0);
  ASSERT_EQ((registry.Size<double, int>()), 0);
}

TEST(Registry_Tests, Destroy_WithPartialView_DecreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<double>(entity);

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<double>(), 0);
  ASSERT_EQ((registry.Size<double, int>()), 0);
}

TEST(Registry_Tests, Destroy_WithExactView_DecreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity);

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<double>(), 0);
  ASSERT_EQ((registry.Size<double, int>()), 0);
}

TEST(Registry_Tests, Create_AfterDestroy_Restore)
{
  Registry<size_t> registry;

  auto entity1 = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity1);

  auto entity2 = registry.Create<int, double>(10, 0.5);

  ASSERT_EQ(registry.Size(), 1);
  ASSERT_EQ(registry.Size<int>(), 1);
  ASSERT_EQ(registry.Size<double>(), 1);
  ASSERT_EQ((registry.Size<double, int>()), 1);
  ASSERT_EQ(registry.Size<float>(), 0);

  ASSERT_EQ(entity1, entity2);
}

TEST(Registry_Tests, DestroyAll_MultipleEntities_DecreaseSize)
{
  Registry<size_t> registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  ASSERT_EQ(registry.Size(), 4);
  ASSERT_EQ(registry.Size<int>(), 2);
  ASSERT_EQ((registry.Size<double, int>()), 1);
  ASSERT_EQ((registry.Size<double>()), 2);
  ASSERT_EQ((registry.Size<float>()), 1);

  registry.DestroyAll();

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ((registry.Size<double, int>()), 0);
  ASSERT_EQ((registry.Size<double>()), 0);
  ASSERT_EQ((registry.Size<float>()), 0);
}

TEST(Registry_Tests, DestroyAll_WithView_DecreaseSizeCorrectly)
{
  Registry<size_t> registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  registry.DestroyAll<double>();

  ASSERT_EQ(registry.Size(), 2);
  ASSERT_EQ(registry.Size<int>(), 1);
  ASSERT_EQ(registry.Size<float>(), 1);
  ASSERT_EQ((registry.Size<double, int>()), 0);
  ASSERT_EQ((registry.Size<double>()), 0);

  registry.DestroyAll<int>();

  ASSERT_EQ(registry.Size(), 1);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<float>(), 1);
  ASSERT_EQ((registry.Size<double, int>()), 0);
  ASSERT_EQ((registry.Size<double>()), 0);

  registry.DestroyAll<float>();

  ASSERT_EQ(registry.Size(), 0);
  ASSERT_EQ(registry.Size<int>(), 0);
  ASSERT_EQ(registry.Size<float>(), 0);
  ASSERT_EQ((registry.Size<double, int>()), 0);
  ASSERT_EQ((registry.Size<double>()), 0);
}

TEST(Registry_Tests, ForEach_EntireRegistrySingleEntity_CorrectEntity)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int>(10);

  size_t iterations = 0;

  registry.Each(
    [&](const size_t entity)
    {
      ASSERT_EQ(entity, created_entity);
      iterations++;
    });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, ForEach_EntireRegistryMultipleEntities_CorrectAmountEntities)
{
  Registry<size_t> registry;

  constexpr size_t amount = 100;

  for (size_t i = 0; i < amount; i++)
  {
    registry.Create<size_t>(10);
  }

  size_t iterations = 0;

  registry.Each([&](const size_t) { iterations++; });

  ASSERT_EQ(registry.Size(), iterations);
  ASSERT_EQ(registry.Size(), amount);
}

TEST(Registry_Tests, ForEach_UnpackNothing_CorrectIterations)
{
  Registry<size_t> registry;

  registry.Create<int>(10);

  size_t iterations = 0;

  registry.Each([&]() { iterations++; });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, ForEach_UnpackOneComponent_Correct)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int>(10);

  size_t iterations = 0;

  registry.Each<int>(
    [&](const size_t entity, int component)
    {
      EXPECT_EQ(entity, created_entity);
      EXPECT_EQ(component, 10);
      iterations++;
    });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, ForEach_UnpackTwoComponents_Correct)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  size_t iterations = 0;

  registry.Each<int, double>(
    [&](const size_t entity, int c1, double c2)
    {
      EXPECT_EQ(entity, created_entity);
      EXPECT_EQ(c1, 10);
      EXPECT_EQ(c2, 0.5);
      iterations++;
    });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, ForEach_UnpackTwoComponentsConst_Correct)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  size_t iterations = 0;

  registry.Each<int, const double>(
    [&](const size_t entity, int c1, const double& c2)
    {
      EXPECT_EQ(entity, created_entity);
      EXPECT_EQ(c1, 10);
      EXPECT_EQ(c2, 0.5);
      iterations++;
    });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, Unpack_Single_Correct)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  ASSERT_EQ(10, registry.Unpack<int>(created_entity));
  ASSERT_EQ(0.5, registry.Unpack<double>(created_entity));
}

TEST(Registry_Tests, ForEach_UnpackThreeComponents_Correct)
{
  Registry<size_t> registry;

  auto created_entity = registry.Create<int, double, float>(10, 0.5, 0.2f);

  size_t iterations = 0;

  registry.Each<int, double, float>(
    [&](const size_t entity, int c1, double c2, float c3)
    {
      EXPECT_EQ(entity, created_entity);
      EXPECT_EQ(c1, 10);
      EXPECT_EQ(c2, 0.5);
      EXPECT_EQ(c3, 0.2f);
      iterations++;
    });

  ASSERT_EQ(registry.Size(), iterations);
}

TEST(Registry_Tests, ForEach_MultipleArchetypes_CorrectAmountIterations)
{
  Registry<size_t> registry;

  registry.Create<int, double, float>(10, 0.5, 0.2f);
  registry.Create<int, double>(11, 0.5);
  registry.Create<float, double>(0.2f, 0.5);
  registry.Create<int>(12);

  size_t iterations = 0;

  registry.Each<int>([&](int) { iterations++; });

  ASSERT_EQ(iterations, 3);

  iterations = 0;

  registry.Each<int, double>([&](int, double) { iterations++; });

  ASSERT_EQ(iterations, 2);

  iterations = 0;

  registry.Each<int, float>([&](int, float) { iterations++; });

  ASSERT_EQ(iterations, 1);

  iterations = 0;

  registry.Each<float>([&](float) { iterations++; });

  ASSERT_EQ(iterations, 2);

  iterations = 0;

  registry.Each<float>([&](float) { iterations++; });

  ASSERT_EQ(iterations, 2);

  iterations = 0;

  registry.Each<double>([&](double) { iterations++; });

  ASSERT_EQ(iterations, 3);

  iterations = 0;

  registry.Each<double, float>([&](double, float) { iterations++; });

  ASSERT_EQ(iterations, 2);
}

TEST(Registry_Tests, ForEach_MultipleArchetypes_CorrectUnpackedValues)
{
  Registry<size_t> registry;

  std::map<size_t, double> mappings;

  registry.Create<int>(11);
  mappings[registry.Create<int, double, float>(10, 0.5, 0.25f)] = 0.5;
  mappings[registry.Create<double, float>(0.1, 0.25f)] = 0.1;
  registry.Create<int, bool>(11, false);
  mappings[registry.Create<double>(0.2)] = 0.2;
  registry.Create<float>(0.25f);

  size_t iterations = 0;

  registry.Each<double>(
    [&](const size_t entity, double c1)
    {
      EXPECT_EQ(c1, mappings[entity]);
      iterations++;
    });

  ASSERT_EQ(iterations, 3);
}

} // namespace genebits::engine::tests