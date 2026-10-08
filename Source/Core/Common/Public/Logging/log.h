#pragma once

#ifdef NDEBUG

#define KT_LOG(...)

#else

#include "LogImportanceLevel.h"
#include "LogSeverity.h"
#include <format>
#include <iostream>
#include <print>

template<typename... Args>
inline void log_internal(ELogSeverity severity, char const* category, char const* funcName, std::format_string<Args...> format, Args&&... args)
{
    switch (severity)
    {
    case ELogSeverity::None:
        std::println("[{0}.{1}()] {2}", category, funcName, std::format(format, std::forward<Args>(args)...));
        break;
    case ELogSeverity::Warning:
        std::println("\033[38;2;229;192;123m[{0}.{1}()] {2}\033[0m", category, funcName, std::format(format, std::forward<Args>(args)...));
        break;
    case ELogSeverity::Error:
        std::println("\033[38;2;224;108;117m[{0}.{1}()] {2}\033[0m", category, funcName, std::format(format, std::forward<Args>(args)...));
        break;
    default:
        break;
    }
    
}

/// Log a message into the console
#define KT_LOG_SEVERITY(Level, Severity, Category, Format, ...) do					\
    {                                                                               \
        if constexpr (KT_SHOULD_LOG(Level))											\
		{																			\
			log_internal(Severity, Category, __FUNCTION__, Format, __VA_ARGS__);	\
		}																			\
    } while (false)


/// Log a message into the console with a severity of ELogSeverity::None
#define KT_LOG(Level, Category, Format, ...) KT_LOG_SEVERITY(Level, ELogSeverity::None, Category, Format, __VA_ARGS__)
  
#endif
