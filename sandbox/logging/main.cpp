
#include "genebits/engine/debug/logging.h"

#include <iostream>

using namespace genebits::engine;

void listen(const LogEvent& event)
{
  std::cout << "File=" << event.log.metadata.file_name;
  std::cout << " Line=" << event.log.metadata.line;
  std::cout << " Message=" << event.log.message;
  std::cout << std::endl;
}

int main()
{
  EventHandler<LogEvent> handler;
  handler.Bind<&listen>();

  GetEnvironment().GetEventBus().Subscribe(handler);

  INFO("Test");

  GetEnvironment().GetEventBus().Unsubscribe(handler);

  return 0;
}