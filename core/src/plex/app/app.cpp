#include "plex/app/app.h"

#include "plex/app/package.h"

namespace plex
{
App::App()
{
  global_context_.Insert(&work_pool_, [](void*) {});
}

void App::AddPackage(const Package& package)
{
  package.DoAdd(*this);
}

Task<void> App::RunScheduler()
{
  return scheduler_.RunAll(global_context_);
}
} // namespace plex
