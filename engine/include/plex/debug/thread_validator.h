#ifndef PLEX_LOCAL_THREAD_VALIDATOR_H
#define PLEX_LOCAL_THREAD_VALIDATOR_H

#ifndef NDEBUG

#include <optional>
#include <thread>
#include <type_traits>

#include "plex/debug/assertion.h"

#define LOCAL_THREAD_DECLARE mutable ::std::optional<::std::thread::id> __local_thread__;

///
/// Performs initialization of the local thread id.
///
#define LOCAL_THREAD_INIT(context)                                                                     \
  {                                                                                                    \
    context->__local_thread__ = ::std::make_optional<::std::thread::id>(::std::this_thread::get_id()); \
  }

///
/// Performs lazy initialization of local thread if the call is not constant evaluated.
/// If initialized, will assert to make sure that the call happened on the same thread it was
/// initialized on.
///
/// Context is the pointer where thread validator was initialized.
///
#define LOCAL_THREAD_ASSERT(context)                                                                 \
  if (!::std::is_constant_evaluated())                                                               \
  {                                                                                                  \
    auto ctx = context;                                                                              \
    if (ctx)                                                                                         \
    {                                                                                                \
      if (!ctx->__local_thread__) [[unlikely]]                                                       \
      {                                                                                              \
        LOCAL_THREAD_INIT(ctx);                                                                      \
      }                                                                                              \
      else                                                                                           \
      {                                                                                              \
        ASSERT(::std::this_thread::get_id() == ctx->__local_thread__, "Called on different thread"); \
      }                                                                                              \
    }                                                                                                \
  }

#else
#define LOCAL_THREAD_DECLARE
#define LOCAL_THREAD_INIT
#define LOCAL_THREAD_ASSERT
#endif

#endif // GENEBITS_LOCAL_THREAD_VALIDATOR_H
