
#include "genebits/engine/logging/stacktrace.h"

#include <iostream>

using namespace genebits::engine;

void function2()
{
  StackTrace stacktrace = StackBackTrace(10);

  for (const auto& frame : stacktrace.frames)
  {
    std::cout << '\t' << frame.name << " in " << frame.file_name << ':' << frame.line << std::endl;
  }
}

void function1()
{
  function2();
}

void function0()
{
  function1();
}

int main()
{
  function0();
  return 0;
}