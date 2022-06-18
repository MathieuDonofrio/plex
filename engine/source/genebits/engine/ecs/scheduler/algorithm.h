#ifndef GENEBITS_ENGINE_ECS_SCHEDULER_ALGORITHM_H
#define GENEBITS_ENGINE_ECS_SCHEDULER_ALGORITHM_H

#include "genebits/engine/ecs/scheduler.h"

namespace genebits::engine
{
Vector<Scheduler::Step> ComputeSchedulerData(const Vector<Stage*>& stages);
} // namespace genebits::engine

#endif
