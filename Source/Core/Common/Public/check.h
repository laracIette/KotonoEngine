#pragma once

#ifdef NDEBUG

#define Check(...)

#else

#include "macro_utils.h"
#include "Logging/log.h"
#include <stdexcept>

#define _CHECK_Warning(Message, ...)	KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Warning, "Warning", Message, __VA_ARGS__)
#define _CHECK_Error(Message, ...)		KT_LOG_SEVERITY(KT_LOG_COMPILE_TIME_LEVEL, ELogSeverity::Error, "Error", Message, __VA_ARGS__)
#define _CHECK_Throw(Message, ...)		throw std::runtime_error{ Message }

#define _GET_CHECK(...) MACRO_CONCAT(_CHECK_, __VA_ARGS__)

#define Check(Severity, Condition, Message, ...) if (!(Condition)) _GET_CHECK(Severity)(Message, __VA_ARGS__)

#endif
