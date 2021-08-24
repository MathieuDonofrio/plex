
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/debug/print.h"

#include <iostream>
#include <string>

using namespace genebits::engine;

void println(const std::string& s, TColor textcolor)
{
  PrintColor(textcolor);
  Print(s);
  PrintColorReset();
  PrintLine();
  PrintFlush();
}

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

  LOG_INFO("Test");

  GetEnvironment().GetEventBus().Unsubscribe(handler);

  println("Test Color", TColor::DarkRed);
  println("Test Color", TColor::Red);

  return 0;
}