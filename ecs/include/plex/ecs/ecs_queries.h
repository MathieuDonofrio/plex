#ifndef PLEX_ECS_ECS_QUERIES_H
#define PLEX_ECS_ECS_QUERIES_H

#include "entity_registry.h"

namespace plex
{
template<typename... Components>
class Entities
{
public:
  static Entities Fetch(void*, Context& global_context, Context&)
  {
    return { global_context.Get<EntityRegistry>().ViewFor<Components...>() };
  }

  static consteval std::array<QueryDataAccess, sizeof...(Components)> GetDataAccess() noexcept
  {
    return { QueryDataAccess {
      TypeName<Components>(),
      TypeName<EntityRegistry>(), // Accessing a subset of the entity registry
      std::is_const_v<Components>, // Check const qualifier to see if the access is read-only.
      IsThreadSafe<Components>::value // Check ThreadSafe trait to see if the access is thread-safe.
    }... };
  }

public:
  template<typename Function>
  ALWAYS_INLINE void ForEach(const Function& function)
  {
    EntityForEach(view_, function);
  }

private:
  Entities(View<Components...> view) : view_(view) {}

private:
  View<Components...> view_;
};

} // namespace plex

#endif
