#ifndef GENEBITS_ENGINE_ECS_ENTITY_MANAGER_H
#define GENEBITS_ENGINE_ECS_ENTITY_MANAGER_H

#include <concepts>

#include "genebits/engine/util/fast_vector.h"

namespace genebits::engine
{
template<std::unsigned_integral Entity, Allocator AllocatorImpl = Mallocator>
class EntityManager
{
public:
  constexpr EntityManager() noexcept : current_(0) {}

  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager& operator=(EntityManager&&) = delete;

  [[nodiscard]] Entity Generate()
  {
    if (recycled_.Empty()) return current_++;
    else
    {
      auto entity = recycled_.back();
      recycled_.PopBack();

      return entity;
    }
  }

  void Release(const Entity entity) noexcept
  {
    recycled_.PushBack(entity);
  }

  void ReleaseAll() noexcept
  {
    recycled_.Clear();

    current_ = 0;
  }

  [[nodiscard]] size_t RecycledCount() const noexcept
  {
    return recycled_.Size();
  }

private:
  Entity current_;
  FastVector<Entity, AllocatorImpl> recycled_;
};

} // namespace genebits::engine

#endif
