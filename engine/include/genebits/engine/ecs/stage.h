#ifndef GENEBITS_ENGINE_ECS_STAGE_H
#define GENEBITS_ENGINE_ECS_STAGE_H

#include "genebits/engine/containers/vector.h"
#include "genebits/engine/ecs/system.h"
#include "genebits/engine/utilities/ref.h"

namespace genebits::engine
{
///
/// The stage object is a container for systems.
///
class Stage
{
private:
  ///
  /// Holds information about a registered system.
  ///
  struct SystemInfo
  {
    Vector<SystemHandle> run_after;
    Vector<SystemHandle> run_before;
  };

public:
  using IsTriviallyRelocatable = std::true_type;

  ///
  /// Builder-pattern style interface for explicitly ordering an added system.
  ///
  /// An instance of this class is returned when a system is added.
  ///
  class SystemOrder
  {
  public:
    ///
    /// Specifies that a system should run after some other system.
    ///
    /// @tparam SystemType Type of the system to run after.
    ///
    /// @param system System to run after.
    ///
    /// @return SystemOrder builder instance.
    ///
    template<typename SystemType>
    SystemOrder After(SystemType system)
    {
      stage_.system_infos_[index_].run_after.PushBack(std::bit_cast<SystemHandle>(system));
      return *this;
    }

    ///
    /// Specifies that a system should run before some other system.
    ///
    /// @tparam SystemType Type of the system to run before.
    ///
    /// @param system System to run before.
    ///
    /// @return SystemOrder builder instance.
    ///
    template<typename SystemType>
    SystemOrder Before(SystemType system)
    {
      stage_.system_infos_[index_].run_before.PushBack(std::bit_cast<SystemHandle>(system));
      return *this;
    }

  private:
    friend Stage;

    ///
    /// Constructs a SystemOrder builder.
    ///
    /// @param[in] stage Reference to the stage to add the system to.
    /// @param[in] index Index of the targeted system for the ordering.
    ///
    constexpr SystemOrder(Stage& stage, size_t index) noexcept : stage_(stage), index_(index) {}

    SystemOrder(const SystemOrder& other) = default;

    Stage& stage_;
    size_t index_;
  };

public:
  ///
  /// Adds a system to the stage.
  ///
  /// @tparam SystemType Type of the system to add.
  ///
  /// @param[in] system The system to add.
  ///
  /// @return Builder-pattern style interface for ordering the added system.
  ///
  template<typename SystemType>
  SystemOrder AddSystem(SystemType system)
  {
    ASSERT(ContainsSystem(system) == false, "System already exists in stage");

    registered_systems_.PushBack(std::make_unique<SystemObject>(system));
    system_infos_.EmplaceBack();

    return SystemOrder(*this, registered_systems_.size() - 1);
  }

  ///
  /// Returns whether or not the two system object are explicitly ordered in the stage. If any of the systems dont exist
  /// in the stage, false is returned.
  ///
  /// @warning This does not check if the systems are going to be run in a specific order. This only checks if there was
  /// an explicit ordering set between the two systems. For example, if system A is after system B, and system B is
  /// after system C, then there is an explicit ordering between system A and system B, and between system B and system
  /// C, but not between system A and system C. Basically, this only checks if After() or Before() was called between
  /// the two systems.
  ///
  /// @param[in] before The system to run before.
  /// @param[in] after The system to run after.
  ///
  /// @return Whether or not the two systems are explicitly ordered.
  ///
  [[nodiscard]] bool HasExplicitOrder(const SystemObject& before, const SystemObject& after) const;

  ///
  /// Returns the managed system object for the system handle provided or nullptr if the system is not registered.
  ///
  /// @param[in] handle Handle of the system to retrieve.
  ///
  /// @return The managed system object for the system handle provided or nullptr if the system is not registered.
  ///
  [[nodiscard]] const SystemObject* GetSystemObject(SystemHandle handle) const;

  ///
  /// Returns the managed system object for the system provided or nullptr if the system is not registered.
  ///
  /// @tparam SystemType Type of the system to retrieve.
  ///
  /// @param[in] handle Handle of the system to retrieve.
  ///
  /// @return The managed system object for the system provided or nullptr if the system is not registered.
  ///
  template<typename SystemType>
  [[nodiscard]] const SystemObject* GetSystemObject(SystemType system) const
  {
    return GetSystemObject(std::bit_cast<SystemHandle>(system));
  }

  ///
  /// Returns whether or not the system is registered.
  ///
  /// @tparam SystemType Type of the system to check.
  ///
  /// @param[in] system The system to check.
  ///
  /// @return Whether or not the system is registered.
  ///
  template<typename SystemType>
  [[nodiscard]] bool ContainsSystem(SystemType system) const
  {
    return GetSystemObject(system) != nullptr;
  }

  ///
  /// Returns all the system objects currently owned by the stage.
  ///
  /// @return All stage system objects.
  ///
  const Vector<std::unique_ptr<SystemObject>>& GetSystemObjects() const
  {
    return registered_systems_;
  }

  ///
  /// Returns the amount of systems currently registered.
  ///
  /// @return The amount of systems.
  ///
  [[nodiscard]] size_t GetSystemCount() const
  {
    return registered_systems_.size();
  }

private:
  Vector<std::unique_ptr<SystemObject>> registered_systems_;
  Vector<SystemInfo> system_infos_;
};
} // namespace genebits::engine

#endif
