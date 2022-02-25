
#include "genebits/engine/async/threading.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Threading_Tests, GetCPUInfo_NormalExecution_HasProcessors)
{
  auto info = GetCPUInfo();

  EXPECT_GT(info.processors.size(), 0);
}

TEST(Threading_Tests, GetCPUInfo_NormalExecution_HasCaches)
{
  auto info = GetCPUInfo();

  EXPECT_GT(info.caches.size(), 0);
}
} // namespace genebits::engine::tests