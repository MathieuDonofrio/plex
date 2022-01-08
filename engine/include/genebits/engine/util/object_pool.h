#ifndef GENEBITS_ENGINE_UTIL_OBJECT_POOL_H
#define GENEBITS_ENGINE_UTIL_OBJECT_POOL_H

#include "genebits/engine/util/allocator.h"

#include <new>

namespace genebits::engine
{
///
/// Object pool.
///
/// Reuses objects to reduce the cost of allocation.
///
/// @tparam Type The type to allocate.
/// @tparam AllocatorType Allocator type to use.
///
template<typename Type, Allocator AllocatorType = Mallocator>
class ObjectPool
{
public:
  ///
  /// Acquires and constructs a new object in place.
  ///
  /// @tparam Args Argument types for constructor.
  /// @param[in] args Constructor args.
  ///
  /// @return New instance of type.
  ///
  template<typename... Args>
  Type* Acquire(Args&&... args)
  {
    Type* object = AcquireUninitialized();

    new (object) Type(std::forward<Args>(args)...);

    return object;
  }

  ///
  /// Acquires a new object but does not call its constructor.
  ///
  /// @warning Use carefully.
  ///
  /// @return New uninitialized instance of type.
  ///
  Type* AcquireUninitialized()
  {
    return reinterpret_cast<Type*>(freelist.Allocate(sizeof(Type)).ptr);
  }

  ///
  /// Releases the object back into the pool.
  ///
  /// @warning Only objects that where acquired from this pool can be release back into it.
  ///
  /// @param[in] type Instance of object to release.
  ///
  void Release(Type* instance)
  {
    freelist.Deallocate({ reinterpret_cast<char*>(instance), sizeof(Type) });
  }

private:
  // TODO Batch allocation.
  // When an appropriate batch allocator is implemented, use contiguous batch allocations. To further
  // reduce allocations and fragmentation.

  // Wrap allocator with free list to accumulated blocks of memory.
  using Allocator = Freelist<AllocatorType, sizeof(Type), sizeof(Type)>;

  Allocator freelist;
};
} // namespace genebits::engine

#endif
