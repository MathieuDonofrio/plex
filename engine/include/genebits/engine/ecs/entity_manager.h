#ifndef GENEBITS_ENGINE_ECS_ENTITY_MANAGER_H
#define GENEBITS_ENGINE_ECS_ENTITY_MANAGER_H

#include <concepts>

#include "genebits/engine/containers/vector.h"

namespace genebits::engine
{
///
/// Responsible for providing and recycling entity identifiers.
///
/// @tparam Entity Entity of integral type to generate.
///
template<std::unsigned_integral Entity>
class EntityManager final
{
public:
  ///
  /// Constructor.
  ///
  constexpr EntityManager() noexcept : current_(0) {}

  ///
  /// Destructor
  ///
  ~EntityManager() = default;

  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager& operator=(EntityManager&&) = delete;

  ///
  /// Obtain a unique entity identifier.
  ///
  /// If there are entity identifiers that have been recycled, those will be returned first,
  /// otherwise a new identifier will be generated.
  ///
  /// @return Unique entity identifier.
  ///
  [[nodiscard]] Entity Obtain()
  {
    if (recycled_.empty()) return Generate();
    else
    {
      auto entity = recycled_.back();
      recycled_.pop_back();

      return entity;
    }
  }

  ///
  /// Generates a new entity identifier.
  ///
  /// Fetches current sequence value and increments it after returning the value;
  ///
  /// @note
  ///    Sequence starts at 0.
  ///
  /// @return New unique entity identifier.
  ///
  [[nodiscard]] constexpr Entity Generate() noexcept
  {
    return current_++;
  }

  ///
  /// Releases the entity identifier allowing it to be reused.
  ///
  /// @param[in] entity Entity identifier to release.
  ///
  void Release(const Entity entity) noexcept
  {
    ASSERT(entity <= current_, "Entity not from this manager");

    recycled_.push_back(entity);
  }

  ///
  /// Releases all the entity identifiers and resets the generator sequence to 0.
  ///
  void ReleaseAll() noexcept
  {
    recycled_.clear();

    current_ = 0;
  }

  ///
  /// Returns the amount of entity identifier currently circulating.
  ///
  /// @return Amount of entity identifiers currently circulating.
  ///
  [[nodiscard]] constexpr size_t CirculatingCount() const noexcept
  {
    return current_ - RecycledCount();
  }

  ///
  /// Returns the amount of entity identifiers ready to be reused.
  ///
  /// @return Amount of entity identifiers currently recycled.
  ///
  [[nodiscard]] constexpr size_t RecycledCount() const noexcept
  {
    return recycled_.size();
  }

private:
  Entity current_;
  Vector<Entity> recycled_;
};

} // namespace genebits::engine

#endif
