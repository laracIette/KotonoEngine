#pragma once

#ifdef NDEBUG

#define KT_LOG(...)

#else

#include "macro_utils.h"
#include <format>
#include <iostream>
#include <print>

inline constexpr auto LOG_COLOR_DEFAULT{	"\033[0m" };
inline constexpr auto LOG_COLOR_ACCENT{		"\033[38;2;72;187;204m" };
inline constexpr auto LOG_COLOR_WARNING{	"\033[38;2;229;192;123m" };
inline constexpr auto LOG_COLOR_ERROR{		"\033[38;2;224;108;117m" };

template<typename... Args>
inline void logInternal(char const* logColor, char const* file, int line, std::format_string<Args...> format, Args&&... args)
{
	std::println("{0}[{1}:{2}] {3}{4}", logColor, file, line, std::format(format, std::forward<Args>(args)...), LOG_COLOR_DEFAULT);    
}

#define LOG_SEVERITY_COLOR_Normal		LOG_COLOR_DEFAULT
#define LOG_SEVERITY_COLOR_Important	LOG_COLOR_ACCENT
#define LOG_SEVERITY_COLOR_Warning		LOG_COLOR_WARNING
#define LOG_SEVERITY_COLOR_Error		LOG_COLOR_ERROR

#define GET_LOG_SEVERITY_COLOR(...) MACRO_CONCAT(LOG_SEVERITY_COLOR_, __VA_ARGS__)

/// Log a message to the console
#define Log(Severity, Format, ...) logInternal(GET_LOG_SEVERITY_COLOR(Severity), __FILE__, __LINE__, Format, __VA_ARGS__)

#endif
