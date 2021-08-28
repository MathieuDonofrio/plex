
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
  my_namespace::func1();

  return 0;
}