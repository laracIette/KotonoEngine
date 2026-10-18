#pragma once

#ifdef NDEBUG

#define Check(...)

#else

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

enum class ECheckSeverity
{
	Warning,
	Error,
	Throw,
	Abort,
};

consteval auto GetCheckSeverityLogSeverity(ECheckSeverity severity) noexcept
{
	switch (severity)
	{
	case ECheckSeverity::Warning:	return ELogSeverity::Warning;
	case ECheckSeverity::Error:		return ELogSeverity::Error;
	case ECheckSeverity::Throw:		return ELogSeverity::Error;
	case ECheckSeverity::Abort:		return ELogSeverity::Error;
	}
	return ELogSeverity::Warning;
}

#define Check(Severity, Condition, Message, ...) do																			\
    {																														\
        if (!(Condition))																									\
        {																													\
            LOG_INTERNAL(GetLogSeverityColor(GetCheckSeverityLogSeverity(ECheckSeverity::Severity)), Message, __VA_ARGS__);	\
			if constexpr (ECheckSeverity::Severity == ECheckSeverity::Throw)												\
			{																												\
				CHECK_HANDLE_THROW(Message);																				\
			}																												\
			else if constexpr (ECheckSeverity::Severity == ECheckSeverity::Abort)											\
			{																												\
				CHECK_HANDLE_ABORT();																						\
			}																												\
        }																													\
    } while (false)


#endif
