
#include <iostream>

#include "genebits/engine/async/sync_wait.h"
#include "genebits/engine/async/task.h"
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/phase.h"
#include "genebits/engine/ecs/system.h"

using namespace genebits::engine;

namespace genebits::engine
{
template<typename... Components>
class PrintSystem : public System<Components...>
{
public:
  PrintSystem(ThreadPool& pool, std::string string, size_t sleep_ms) : pool_(pool), string_(string), sleep_ms_(sleep_ms)
  {}

private:
  Task<> OnUpdate()
  {
    co_await pool_.Schedule();

    std::this_thread::sleep_for(std::chrono::milliseconds { sleep_ms_ });

    LOG_INFO("{}", string_);
  }

  ThreadPool& pool_;
  std::string string_;
  size_t sleep_ms_;
};

} // namespace genebits::engine

int main()
{
  using namespace genebits::engine;

  ThreadPool pool;

  auto group = MakeRef<SystemGroup>();

  group->Add(MakeRef<PrintSystem<int>>(pool, "System 1", 1));
  group->Add(MakeRef<PrintSystem<int>>(pool, "System 2", 1));
  group->Add(MakeRef<PrintSystem<>>(pool, "System 3", 0));

  Phase phase = Phase::Compile(group);

  SyncWait(phase.Run());

  return 0;
}