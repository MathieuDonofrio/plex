#ifndef GENEBITS_ENGINE_LOCAL_THREAD_VALIDATOR_H
#define GENEBITS_ENGINE_LOCAL_THREAD_VALIDATOR_H

#ifndef NDEBUG

#include <type_traits>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/parallel/threading.h"

// TODO maybe use std::optional and std::thead::id?

#define LOCAL_THREAD_VALIDATOR_INIT mutable std::thread::native_handle_type __local_thread__ = nullptr

///
/// Performs lazy initialization of local thread if the call is not constant evaluated.
/// If initialized, will assert to make sure that the call happened on the same thread it was
/// initialized on.
///
#define LOCAL_THREAD_ASSERT                                                                  \
  if (!std::is_constant_evaluated())                                                         \
  {                                                                                          \
    if (__local_thread__ == nullptr) __local_thread__ = this_thread::NativeHandle();         \
    else                                                                                     \
    {                                                                                        \
      ASSERT(this_thread::NativeHandle() == __local_thread__, "Called on different thread"); \
    }                                                                                        \
  }

#else
#define LOCAL_THREAD_VALIDATOR_INIT
#define LOCAL_THREAD_ASSERT
#endif

#endif // GENEBITS_LOCAL_THREAD_VALIDATOR_H
