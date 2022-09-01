#ifndef PLEX_APP_APP_H
#define PLEX_APP_APP_H

#include "plex/async/thread_pool.h"
#include "plex/scheduler/scheduler.h"

namespace plex
{
class Package;

///
/// The app is the main entry point to a plex-based application.
///
/// It is simply an empty shell until packages are added.
///
class App
{
public:
  ///
  /// Default constructor.
  ///
  App();

  virtual ~App() = default;

  ///
  /// Returns a task containing the execution of all currently scheduled stages.
  ///
  /// Resets the stages to be scheduled.
  ///
  /// @return Task for the execution of all the scheduled stages.
  ///
  Task<void> RunScheduler();

  ///
  /// Adds a package to the application.
  ///
  /// @param[in] package Package to add.
  ///
  void AddPackage(const Package& package);

  ///
  /// Queues the stage to have all its systems executed in the next scheduler run.
  ///
  /// The stage will be run in the order that it was scheduled.
  ///
  /// Systems inside a stage are guaranteed to run in the order that they have been scheduled if there are dependencies
  /// between systems in other stages.
  ///
  /// @tparam StageType The stage to schedule
  ///
  template<typename StageType>
  void Schedule()
  {
    scheduler_.template Schedule<StageType>();
  }

  ///
  /// Adds a system to the scheduler for the given stage.
  ///
  /// @tparam StageType The stage to add system to.
  /// @tparam SystemType Type of the system to add.
  ///
  /// @param[in] system The system to add.
  ///
  /// @return Builder-pattern style interface for ordering the added system.
  ///
  template<typename StageType, typename SystemType>
  typename Stage::SystemOrder AddSystem(SystemType* system)
  {
    return scheduler_.template AddSystem<StageType>(system);
  }

  ///
  /// Constructs the object directly into the global context.
  ///
  /// Objects in the global context can be accessed by any query.
  ///
  /// @tparam Type Type of the global object to create.
  /// @tparam Args Argument types of the global object's constructor.
  ///
  /// @param[in] args Arguments to create the global object with.
  ///
  template<typename Type, typename... Args>
  void EmplaceGlobal(Args&&... args)
  {
    global_context_.template Emplace<Type>(std::forward<Args>(args)...);
  }

  ///
  /// Adds the object to the global context.
  ///
  /// Objects in the global context can be accessed by any query.
  ///
  /// @tparam Type The type of the object to add.
  ///
  /// @param[in] value The object to add.
  ///
  template<typename Type>
  void AddGlobal(const Type& value)
  {
    EmplaceGlobal(value);
  }

  ///
  /// Adds the object to the global context.
  ///
  /// Objects in the global context can be accessed by any query.
  ///
  /// @tparam Type The type of the object to add.
  ///
  /// @param[in] value The object to add.
  ///
  template<typename Type>
  void AddGlobal(Type&& value)
  {
    EmplaceGlobal(std::move(value));
  }

  ///
  /// Returns a reference to the object of the given type inside the global context.
  ///
  /// @warning If the type does not exist in the context, the behaviour of this method is undefined.
  ///
  /// @tparam Type The type to obtain reference to instance.
  ///
  /// @return Reference to instance of type from global context.
  ///
  template<typename Type>
  const Type& GetGlobal() const noexcept
  {
    global_context_.template Get<Type>();
  }

  ///
  /// Returns a reference to the object of the given type inside the global context.
  ///
  /// @warning If the type does not exist in the context, the behaviour of this method is undefined.
  ///
  /// @tparam Type The type to obtain reference to instance.
  ///
  /// @return Reference to instance of type in global context.
  ///
  template<typename Type>
  Type& GetGlobal() noexcept
  {
    global_context_.template Get<Type>();
  }

private:
  Context global_context_;
  Scheduler scheduler_;
  ThreadPool work_pool_;
};
} // namespace plex

#endif
