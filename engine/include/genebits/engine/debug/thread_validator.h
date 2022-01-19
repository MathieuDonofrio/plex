#ifndef GENEBITS_ENGINE_LOCAL_THREAD_VALIDATOR_H
#define GENEBITS_ENGINE_LOCAL_THREAD_VALIDATOR_H

#ifndef NDEBUG

#include <optional>
#include <thread>
#include <type_traits>

#include "genebits/engine/debug/assertion.h"

#define LOCAL_THREAD_VALIDATOR_INIT mutable ::std::optional<::std::thread::id> __local_thread__;

///
/// Performs lazy initialization of local thread if the call is not constant evaluated.
/// If initialized, will assert to make sure that the call happened on the same thread it was
/// initialized on.
///
/// Context is the pointer where thread validator was initialized.
///
#define LOCAL_THREAD_ASSERT(context)                                                                       \
  if (!::std::is_constant_evaluated())                                                                     \
  {                                                                                                        \
    if (context)                                                                                           \
    {                                                                                                      \
      if (!context->__local_thread__) [[unlikely]]                                                         \
      {                                                                                                    \
        context->__local_thread__ = ::std::make_optional<::std::thread::id>(::std::this_thread::get_id()); \
      }                                                                                                    \
      else                                                                                                 \
      {                                                                                                    \
        ASSERT(::std::this_thread::get_id() == context->__local_thread__, "Called on different thread");   \
      }                                                                                                    \
    }                                                                                                      \
  }

#else
#define LOCAL_THREAD_VALIDATOR_INIT
#define LOCAL_THREAD_ASSERT
#endif

#endif // GENEBITS_LOCAL_THREAD_VALIDATOR_H
