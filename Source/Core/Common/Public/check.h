#pragma once

#ifdef NDEBUG

#define Check(...)

#else

#include "macro_utils.h"
#include "Logging/log.h"
#include <cstdlib>
#include <stdexcept>

#if defined(_MSC_VER)
#define CHECK_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define CHECK_DEBUG_BREAK() __builtin_trap()
#endif

#define CHECK_HANDLE_RETURN() return
#define CHECK_HANDLE_THROW(Message) throw std::runtime_error{ Message }
#define CHECK_HANDLE_ABORT() CHECK_DEBUG_BREAK(); std::abort()

#define CHECK_LOG_Warning(Message, ...)	Log(Warning,	"Warning",	Message,	__VA_ARGS__)
#define CHECK_LOG_Error(Message, ...)	Log(Error,		"Error",	Message,	__VA_ARGS__)
#define CHECK_LOG_Throw(Message, ...)	Log(Error,		"Throw",	Message,	__VA_ARGS__); CHECK_HANDLE_THROW(Message)
#define CHECK_LOG_Abort(Message, ...)	Log(Error,		"Abort",	Message,	__VA_ARGS__); CHECK_HANDLE_ABORT()

#define GET_CHECK_LOG(...) MACRO_CONCAT(CHECK_LOG_, __VA_ARGS__)

#define Check(Severity, Condition, Message, ...) do				\
    {															\
        if (!(Condition))										\
        {														\
            GET_CHECK_LOG(Severity)(Message, __VA_ARGS__);		\
        }														\
    } while (false)


#endif
