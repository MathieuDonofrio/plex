#ifndef GENEBITS_ENGINE_DEBUG_ASSERTION_H
#define GENEBITS_ENGINE_DEBUG_ASSERTION_H

#ifdef NDEBUG
#define ASSERT(expression, message) ((void)0)
#define ERROR(message)
#else
#include <cassert>
#define ASSERT(expression, message) assert(((void)(message), (expression)))
#define ERROR(message) ASSERT(true, message)
#endif
#endif