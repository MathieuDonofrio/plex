#ifndef GENEBITS_ENGINE_SYSTEM_CONTEXT_H
#define GENEBITS_ENGINE_SYSTEM_CONTEXT_H

#include <bit>

#include "genebits/engine/containers/type_map.h"
#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/erased_ptr.h"

namespace plex
{
///
/// Container for single instances of different types.
///
class Context
{
public:
  ///
  /// Inserts a new instance into the context.
  ///
  /// If there is already a instance of the same type, the old instance will be replaced.
  ///
  /// @tparam Type The type of the instance to insert.
  /// @tparam Args Argument types to pass to the constructor.
  ///
  /// @param[in] args Arguments to pass to the constructor.
  ///
  template<typename Type, typename... Args>
  void Emplace(Args&&... args)
  {
    InsertOrReplace<Type>(MakeErased<Type>(std::forward<Args>(args)...));
  }

  ///
  /// Inserts a new instance into the context.
  ///
  /// If there is already a instance of the same type, the old instance will be replaced.
  ///
  /// @tparam Type The type of the instance to insert.
  ///
  /// @param[in] instance Pointer to the instance to insert.
  /// @param[in] deleter Custom deleter to use when destroying the resource.
  ///
  template<typename Type>
  void Insert(Type* instance, void (*deleter)(void*))
  {
    InsertOrReplace<Type>(ErasedPtr<Type>(instance, deleter));
  }

  ///
  /// Inserts a new instance into the context.
  ///
  /// If there is already a instance of the same type, the old instance will be replaced.
  ///
  /// @tparam Type The type of resource to insert.
  ///
  /// @param[in] instance Pointer to the resource to insert.
  ///
  template<typename Type>
  void Insert(Type* instance)
  {
    InsertOrReplace<Type>(ErasedPtr<Type>(instance));
  }

  ///
  /// Removes the instance of a given type from the context.
  ///
  /// @warning The instance must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam Type The type of instance to remove.
  ///
  template<typename Type>
  void Remove()
  {
    ASSERT(Contains<Type>(), "Instance of given type not found");

    map_.Get<Type>() = nullptr;
    instances_.SwapAndPop(FindByName(TypeName<Type>()));
  }

  ///
  /// Returns a reference to the instance of the given type.
  ///
  /// @warning The instance must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam Type The type of instance to return.
  ///
  /// @return Reference to the instance.
  ///
  template<typename Type>
  const Type& Get() const noexcept
  {
    ASSERT(Contains<Type>(), "Instance of given type not found");

    return *std::bit_cast<Type*>(map_.Get<Type>());
  }

  ///
  /// Returns a reference to the instance of the given type.
  ///
  /// @warning The instance must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam Type The type of instance to return.
  ///
  /// @return Reference to the instance.
  ///
  template<typename Type>
  Type& Get() noexcept
  {
    return const_cast<Type&>(static_cast<const Context&>(*this).Get<Type>());
  }

  ///
  /// Returns whether or not the context contains an instance of the given type.
  ///
  /// @tparam Type The type of instance to check for.
  ///
  /// @return True if the manager contains the instance, false otherwise.
  ///
  template<typename Type>
  [[nodiscard]] bool Contains() const noexcept
  {
    return map_.template ContainsNonDefault<Type>();
  }

  ///
  /// Returns the number of instances in the container.
  ///
  /// @return The number of instances.
  ///
  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return instances_.size();
  }

private:
  ///
  /// Holds information about a resource.
  ///
  struct InstanceInfo
  {
    using IsTriviallyRelocatable = std::true_type;

    ///
    /// Parametric constructor.
    ///
    /// @param[in] name The name of the resource.
    /// @param[in] instance The instance.
    ///
    constexpr InstanceInfo(std::string_view name, ErasedPtr<void>&& instance) noexcept
      : name(name), instance(std::move(instance))
    {}

    std::string_view name;
    ErasedPtr<void> instance;
  };

  ///
  /// Inserts a type-erased instance pointer into the context.
  ///
  /// If a resource for the type already exists, the old resource will be replaced.
  ///
  /// @tparam Type The type of the instance to insert.
  ///
  /// @param[in] resource Pointer to the resource to insert.
  ///
  template<typename Type>
  COLD_SECTION void InsertOrReplace(ErasedPtr<void>&& instance)
  {
    const constexpr auto name = TypeName<Type>();

    const bool already_exists = Contains<Type>();

    map_.Assure<Type>() = instance.get();

    if (already_exists) FindByName(name)->instance.swap(instance);
    else
    {
      instances_.emplace_back(name, std::move(instance));
    }
  }

  ///
  /// Returns a iterator to the instance with the given name, or end() if not found.
  ///
  /// @param[in] name Name of the instance type to find.
  ///
  /// @return Iterator to the instance, or end() if not found.
  ///
  [[nodiscard]] COLD_SECTION InstanceInfo* FindByName(std::string_view name) noexcept
  {
    for (auto it = instances_.begin(); it != instances_.end(); ++it)
    {
      if (it->name == name) return it;
    }

    return instances_.end();
  };

private:
  TypeMap<void*> map_;
  Vector<InstanceInfo> instances_;
};

} // namespace plex

#endif
