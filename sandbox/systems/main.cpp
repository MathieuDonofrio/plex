
#include "plex/app/app.h"
#include "plex/async/sync_wait.h"
#include "plex/async/task.h"
#include "plex/debug/logging.h"
#include "plex/ecs/ecs.h"
#include "plex/events/events.h"

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

  struct Stage3
  {};

  TestApp()
  {
    AddPackage(ECSPackage {});
    AddPackage(EventsPackage {});

    GetGlobal<EntityRegistry>().Create<int>(10);
    GetGlobal<EntityRegistry>().Create<int>(99);

    AddSystem<TestApp::Stage1>(system1);
    AddSystem<TestApp::Stage1>(system2);
    AddSystem<TestApp::Stage2>(system3);
    AddSystem<TestApp::Stage3>(system4);
  }

  void Run()
  {
    Schedule<TestApp::Stage1>();
    Schedule<TestApp::Stage2>();
    Schedule<TestApp::Stage3>();
    Schedule<TestApp::Stage3>();

    SyncWait(RunScheduler());
  }

  static void system1(EntityRegistry& registry)
  {
    LOG_INFO("System1");

    EntityForEach(registry.ViewFor<int>(), [](Entity entity, int i) { LOG_INFO("Entity {}: {}", entity, i); });

    registry.Create<int>(100);
  }

  static Task<void> system2(ThreadPool& pool)
  {
    co_await pool.Schedule();

    LOG_INFO("System2");
  }

  static void system3(Event<int> int_event)
  {
    LOG_INFO("System3");

    int_event.Send(100);
    int_event.Send(200);
    int_event.Send(300);
  }

  static void system4(Event<const int> int_event)
  {
    LOG_INFO("System4");

    while (int_event.HasNext())
    {
      LOG_INFO("Received {}", int_event.Next());
    }
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