
#include "plex/os/cpu_info.h"

#include <gtest/gtest.h>

namespace plex::tests
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
} // namespace plex::tests