#ifndef GENEBITS_ENGINE_ECS_RESOURCE_MANAGER_H
#define GENEBITS_ENGINE_ECS_RESOURCE_MANAGER_H

#include <bit>

#include "genebits/engine/containers/type_map.h"
#include "genebits/engine/containers/vector.h"
#include "genebits/engine/utilities/erased_ptr.h"

namespace genebits::engine
{
///
/// Responsible for the storage and management of resources.
///
class ResourceManager
{
public:
  ///
  /// Inserts a new resource into the manager.
  ///
  /// If there is already a resource of the same type, the old resource will be replaced.
  ///
  /// @tparam ResourceType The type of resource to insert.
  /// @tparam Args Argument types to pass to the resource constructor.
  ///
  /// @param[in] args Arguments to pass to the resource constructor.
  ///
  template<typename ResourceType, typename... Args>
  void Emplace(Args&&... args)
  {
    InsertOrReplace<ResourceType>(MakeErased<ResourceType>(std::forward<Args>(args)...));
  }

  ///
  /// Inserts a new resource into the manager.
  ///
  /// If there is already a resource of the same type, the old resource will be replaced.
  ///
  /// @tparam ResourceType The type of resource to insert.
  ///
  /// @param[in] resource Pointer to the resource to insert.
  /// @param[in] deleter Custom deleter to use when destroying the resource.
  ///
  template<typename ResourceType>
  void Insert(ResourceType* resource, void (*deleter)(void*))
  {
    InsertOrReplace<ResourceType>(ErasedPtr<ResourceType>(resource, deleter));
  }

  ///
  /// Inserts a new resource into the manager.
  ///
  /// If there is already a resource of the same type, the old resource will be replaced.
  ///
  /// @tparam ResourceType The type of resource to insert.
  ///
  /// @param[in] resource Pointer to the resource to insert.
  ///
  template<typename ResourceType>
  void Insert(ResourceType* resource)
  {
    InsertOrReplace<ResourceType>(ErasedPtr<ResourceType>(resource));
  }

  ///
  /// Removes a resource from the manager.
  ///
  /// @warning The resource must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam ResourceType The type of resource to remove.
  ///
  template<typename ResourceType>
  void Remove()
  {
    ASSERT(Contains<ResourceType>(), "Resource not found");

    map_.Get<ResourceType>() = nullptr;
    resources_.UnorderedErase(FindResourceByName(TypeInfo<ResourceType>::Name()));
  }

  ///
  /// Returns a reference to the resource of the given type.
  ///
  /// @warning The resource must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam ResourceType The type of resource to return.
  ///
  /// @return Reference to the resource.
  ///
  template<typename ResourceType>
  const ResourceType& Get() const noexcept
  {
    ASSERT(Contains<ResourceType>(), "Resource not found");

    return *std::bit_cast<ResourceType*>(map_.Get<ResourceType>());
  }

  ///
  /// Returns a reference to the resource of the given type.
  ///
  /// @warning The resource must exist, otherwise the behaviour is undefined.
  ///
  /// @tparam ResourceType The type of resource to return.
  ///
  /// @return Reference to the resource.
  ///
  template<typename ResourceType>
  ResourceType& Get() noexcept
  {
    return const_cast<ResourceType&>(static_cast<const ResourceManager&>(*this).Get<ResourceType>());
  }

  ///
  /// Returns whether or not the manager contains a resource of the given type.
  ///
  /// @tparam ResourceType The type of resource to check for.
  ///
  /// @return True if the manager contains the resource, false otherwise.
  ///
  template<typename ResourceType>
  [[nodiscard]] bool Contains() const noexcept
  {
    return map_.template ContainsNonDefault<ResourceType>();
  }

  ///
  /// Returns the number of resources managed by the manager.
  ///
  /// @return The number of resources.
  ///
  [[nodiscard]] constexpr size_t Size() const noexcept
  {
    return resources_.size();
  }

private:
  ///
  /// Holds information about a resource.
  ///
  struct ResourceInfo
  {
    using IsTriviallyRelocatable = std::true_type;

    ///
    /// Parametric constructor.
    ///
    /// @param[in] name The name of the resource.
    /// @param[in] resource The resource.
    ///
    constexpr ResourceInfo(std::string_view name, ErasedPtr<void>&& resource) noexcept
      : name(name), resource(std::move(resource))
    {}

    std::string_view name;
    ErasedPtr<void> resource;
  };

  ///
  /// Inserts a type-erased resource pointer into the manager.
  ///
  /// If a resource for the type already exists, the old resource will be replaced.
  ///
  /// @tparam ResourceType The type of resource to insert.
  ///
  /// @param[in] resource Pointer to the resource to insert.
  ///
  template<typename ResourceType>
  void InsertOrReplace(ErasedPtr<void>&& resource)
  {
    static const constexpr auto name = TypeInfo<ResourceType>::Name();

    const bool already_exists = Contains<ResourceType>();

    map_.Assure<ResourceType>() = resource.Get();

    if (already_exists) FindResourceByName(name)->resource.Swap(resource);
    else
    {
      resources_.EmplaceBack(name, std::move(resource));
    }
  }

  ///
  /// Returns a iterator to the resource with the given name, or end() if not found.
  ///
  /// @param[in] name Name of the resource to find.
  ///
  /// @return Iterator to the resource, or end() if not found.
  ///
  [[nodiscard]] auto FindResourceByName(std::string_view name) noexcept
  {
    for (auto it = resources_.begin(); it != resources_.end(); ++it)
    {
      if (it->name == name) return it;
    }

    return resources_.end();
  };

private:
  Vector<ResourceInfo> resources_;
  TypeMap<void*> map_;
};

} // namespace genebits::engine

#endif
