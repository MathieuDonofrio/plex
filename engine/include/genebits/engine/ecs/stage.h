#ifndef GENEBITS_ENGINE_ECS_STAGE_H
#define GENEBITS_ENGINE_ECS_STAGE_H

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/ecs/system.h"
#include <span>

namespace genebits::engine
{
class Stage
{
private:
  struct SystemInfo
  {
    Vector<void*> run_after;
    // Vector<void*> run_before;
  };

public:
  class SystemOrder
  {
  public:
    template<typename SystemType>
    SystemOrder After(SystemType system)
    {
      stage_.system_infos_[index_].run_after.PushBack(reinterpret_cast<void*>(system));
      return *this;
    }

  private:
    friend Stage;

    constexpr SystemOrder(Stage& stage, size_t index) noexcept : stage_(stage), index_(index) {}

    SystemOrder(const SystemOrder& other) = default;

    Stage& stage_;
    size_t index_;
  };

public:
  template<typename SystemType>
  SystemOrder AddSystem(SystemType system)
  {
    registered_systems_.EmplaceBack(system);
    system_infos_.EmplaceBack();

    return SystemOrder(*this, registered_systems_.size() - 1);
  }

  [[nodiscard]] bool IsAfter(const SystemObject& before, const SystemObject& after) const;

  [[nodiscard]] std::span<Ref<SystemObject>> GetSystems()
  {
    return { registered_systems_.begin(), registered_systems_.size() };
  }

private:
  Vector<Ref<SystemObject>> registered_systems_;
  Vector<SystemInfo> system_infos_;
};
} // namespace genebits::engine

#endif
