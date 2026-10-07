#pragma once

#ifdef NDEBUG

#define Check(...)

#else

#include "macro_utils.h"
#include "Logging/log.h"
#include <cstdlib>
#include <stdexcept>

#if defined(_MSC_VER)
#define _BREAK __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define _BREAK __builtin_trap()
#endif

#define _HANDLE_THROW(Message) throw std::runtime_error{ Message }
#define _HANDLE_ABORT() _BREAK; std::abort()

#define _LOG_Warning(Message, ...)	KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Warning, "Warning", Message, __VA_ARGS__)
#define _LOG_Error(Message, ...)	KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Error, "Error", Message, __VA_ARGS__)
#define _LOG_Throw(Message, ...)	KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Error, "Throw", Message, __VA_ARGS__); _HANDLE_THROW(Message)
#define _LOG_Abort(Message, ...)	KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Error, "Abort", Message, __VA_ARGS__); _HANDLE_ABORT()

#define _GET_LOG(...) MACRO_CONCAT(_LOG_, __VA_ARGS__)

#define Check(Severity, Condition, Message, ...) do     \
    {                                                   \
        if (!(Condition))                               \
        {                                               \
            _GET_LOG(Severity)(Message, __VA_ARGS__);   \
        }                                               \
    } while (false)


#endif
