
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

  struct EventsUpdateStage
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

    AddSystem<TestApp::EventsUpdateStage>(EventsUpdateSystem);
  }

  void Run()
  {
    Schedule<TestApp::Stage1>();
    Schedule<TestApp::Stage2>();
    Schedule<TestApp::Stage3>();
    Schedule<TestApp::Stage3>();

    Schedule<TestApp::EventsUpdateStage>();

    SyncWait(RunScheduler());
  }

  static Task<void> EventsUpdateSystem(ThreadPool& pool, EventRegistry& registry)
  {
    co_await pool.Schedule();

    registry.Update();
  }

  static Task<void> system1(ThreadPool& pool, Entities<int> entities, EntityRegistry& registry)
  {
    co_await pool.Schedule();

    LOG_INFO("System1");

    entities.ForEach(
      []([[maybe_unused]] Entity entity, [[maybe_unused]] int i) { LOG_INFO("Entity {}: {}", entity, i); });

    registry.Create<int>(100);
  }

  static Task<void> system2(ThreadPool& pool)
  {
    co_await pool.Schedule();

    LOG_INFO("System2");
  }

  static Task<void> system3(ThreadPool& pool, Event<int> int_event)
  {
    co_await pool.Schedule();

    LOG_INFO("System3");

    int_event.Send(100);
    int_event.Send(200);
    int_event.Send(300);

    LOG_INFO("Sent events");
  }

  static Task<void> system4(ThreadPool& pool, Event<const int> int_event)
  {
    co_await pool.Schedule();

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