
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/system.h"

using namespace genebits::engine;

namespace genebits::engine
{

template<typename... Components>
class MySystem : public System<Components...>
{
public:
  MySystem(size_t id) : id_(id) {}

  void OnUpdate(JobHandle dependencies) override
  {
    auto job = new BasicLambdaJob([&]() { LOG_INFO("Running System Job: {}", id_); });

    MySystem::ScheduleDefered(job, dependencies);
  }

private:
  size_t id_;
};
} // namespace genebits::engine

int main()
{
  using namespace genebits::engine;

  Registry registry;

  registry.Create();
  registry.Create();

  ThreadPool pool;

  JobScheduler scheduler(pool);

  SystemGroup group(&scheduler, &registry);

  MySystem<int> system1(1);
  MySystem<const int, const float, double> system2(2);
  MySystem<float, const int> system3(3);
  MySystem<const int, const float, bool> system4(4);

  group.AddSystem(&system1);
  group.AddSystem(&system2);
  group.AddSystem(&system3);
  group.AddSystem(&system4);

  group.Compile();

  group.Run();

  return 0;
}