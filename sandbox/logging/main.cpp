
#include "genebits/engine/async/threading.h"
#include "genebits/engine/debug/logging.h"

using namespace genebits::engine;

namespace my_namespace
{
void func3()
{
  LOG_TRACE("Test");
  LOG_TRACE("Test");
  LOG_TRACE("Test");
  LOG_TRACE("Test");
  LOG_TRACE("Test");
  LOG_INFO("Test");
  LOG_WARN("Test");
  LOG_INFO("Test");
  LOG_INFO("Test");
  LOG_ERROR("Test");
}

void func2()
{
  func3();
}

void func1()
{
  func2();
}
} // namespace my_namespace

int main()
{
  LOG_INFO("Physical Processors: {}", GetAmountPhysicalProcessors());
  LOG_INFO("Logical Processors: {}", GetAmountLogicalProcessors());

  CPUInfo info = GetCPUInfo();

  for ([[maybe_unused]] auto& cache : info.caches)
  {
    LOG_INFO("Cache: level={}, size={}, line_size={}", cache.level, cache.size, cache.line_size);
  }

  my_namespace::func1();

  return 0;
}