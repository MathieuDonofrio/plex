
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/system.h"

#include <iostream>

using namespace genebits::engine;

namespace genebits::engine
{
Task AsyncPrint(ThreadPool& pool, std::string string)
{
  co_await pool.Schedule();

  std::cout << string << std::endl;
  std::cout << "Task Thread Id: " << std::this_thread::get_id() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds { 100 });
}

} // namespace genebits::engine

int main()
{
  using namespace genebits::engine;

  std::cout << "Main Thread Id: " << std::this_thread::get_id() << std::endl;

  ThreadPool pool;

  SyncCounter counter(2);

  // Create tasks
  auto task1 = AsyncPrint(pool, "Hello world 1!");
  auto task2 = AsyncPrint(pool, "Hello world 2!");

  // Create syncs
  auto sync1 = MakeSync<SyncCounter>(task1);
  auto sync2 = MakeSync<SyncCounter>(task2);

  // Start tasks with sync
  sync1.Start(counter);
  sync2.Start(counter);

  // Wait for tasks
  counter.Wait();

  std::cout << "End" << std::endl;

  return 0;
}