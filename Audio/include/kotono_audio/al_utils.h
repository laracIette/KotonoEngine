#pragma once

#ifdef NDEBUG
#define AL_CHECK_THROW(x, msg) x
#else
#define AL_CHECK_THROW(x, msg) x; ALCheckAndThrow(alGetError(), msg)
#endif

void ALCheckAndThrow(int result, char const* message);
