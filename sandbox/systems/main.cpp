
#include <iostream>

#include "plex/app/app.h"
#include "plex/async/sync_wait.h"
#include "plex/async/task.h"
#include "plex/debug/logging.h"
#include "plex/system/system.h"

using namespace plex;

namespace plex
{
class TestApp : public App
{
public:
  struct Stage1
  {};

  struct Stage2
  {};

  TestApp()
  {
    AddSystem<TestApp::Stage1>(system1);
    AddSystem<TestApp::Stage1>(system2);
    AddSystem<TestApp::Stage2>(system3);
  }

  void Run()
  {
    Schedule<TestApp::Stage1>();
    Schedule<TestApp::Stage2>();
    Schedule<TestApp::Stage2>();
    Schedule<TestApp::Stage1>();
    Schedule<TestApp::Stage2>();

    SyncWait(RunScheduler());
  }

  static void system1()
  {
    LOG_INFO("System1");
  }

  static void system2()
  {
    LOG_INFO("System2");
  }

  static void system3()
  {
    LOG_INFO("System3");
  }
};
} // namespace plex

int main()
{
  using namespace plex;

  TestApp app;

  app.Run();

  return 0;
}