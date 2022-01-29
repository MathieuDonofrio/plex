
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/ecs/system.h"

#include <iostream>

#include "genebits/engine/parallel/sync_wait.h"
#include "genebits/engine/parallel/task.h"

using namespace genebits::engine;

namespace genebits::engine
{
Task<> AsyncPrint(ThreadPool& pool, std::string string)
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

  SyncWait(AsyncPrint(pool, "Hello world 1!"));
  SyncWait(AsyncPrint(pool, "Hello world 2!"));

  std::cout << "End" << std::endl;

  return 0;
}