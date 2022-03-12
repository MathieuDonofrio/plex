#ifndef GENEBITS_ENGINE_DEBUG_ASSERTION_H
#define GENEBITS_ENGINE_DEBUG_ASSERTION_H

#ifdef NDEBUG
#define ASSERT(expression, message) ((void)0)
#else
#include <cassert>
#define ASSERT(expression, message) assert(((void)(message), (expression)))
#endif
#endif