#pragma once

#include "LogImportanceLevel.h"

#ifdef NDEBUG

#define KT_LOG(level, format, ...)

#else

#include <format>
#include <iostream>
#include <print>

template<typename... Args>
void log_internal(const char* category, const char* funcName, std::format_string<Args...> format, Args&&... args)
{
    std::println("[{0}.{1}()] {2}", category, funcName, std::format(format, std::forward<Args>(args)...));
}

#define KT_LOG(level, category, format, ...) if constexpr (KT_SHOULD_LOG(level)) log_internal(category, __FUNCTION__, format, __VA_ARGS__)
  
#endif