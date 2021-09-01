#include "genebits/engine/ecs/registry.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Registry_Tests, Create_AfterInitialization_IncreaseSize)
{
  Registry<size_t> registry;

  auto entity = registry.Create<int>(10);

  ASSERT_EQ(registry.Size(), 1);
  ASSERT_EQ(registry.Size<int>(), 1);
  ASSERT_EQ(entity, 0);
}
} // namespace genebits::engine::tests