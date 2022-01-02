
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/system.h"

using namespace genebits::engine;

namespace genebits::engine
{
template<typename... Components>
class MySystem : public System<Entity, Components...>
{
public:
  MySystem(size_t id) : id_(id) {}

  void OnUpdate() override
  {

    LOG_INFO("System {}", id_);
  }

private:
  size_t id_;
};
} // namespace genebits::engine

int main()
{
  using namespace genebits::engine;

  /*
  Registry<Entity> registry;

  registry.Create();
  registry.Create();


  ThreadPool pool;

  JobScheduler scheduler(pool);

   */

  SystemGroup group;

  MySystem<const int, float> system1(1);
  MySystem<const int> system2(2);
  MySystem<const float, int> system3(3);
  MySystem<const int> system4(4);

  group.AddSystem(&system1);
  group.AddSystem(&system2);
  group.AddSystem(&system3);
  group.AddSystem(&system4);

  group.Compile();

  group.Run();

  return 0;
}