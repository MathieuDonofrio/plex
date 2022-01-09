
#include "genebits/engine/parallel/threading.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Threading_Tests, GetCPUInfo_NormalExecution_HasProcessors)
{
  auto info = GetCPUInfo();

  EXPECT_GT(info.processors.size(), 0u);
}

TEST(Threading_Tests, GetCPUInfo_NormalExecution_HasCaches)
{
  auto info = GetCPUInfo();

  EXPECT_GT(info.caches.size(), 0u);
}
} // namespace genebits::engine::tests