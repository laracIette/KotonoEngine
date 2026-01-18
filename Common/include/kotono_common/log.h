#pragma once

#include "LogImportanceLevel.h"

#ifdef _DEBUG
#include <format>
#include <iostream>

template<typename... Args>
void log_internal(const char* category, const char* funcName, std::format_string<Args...> format, Args&&... args)
{
    std::cout 
        << std::format("[{}.{}()]", category, funcName) << ' '
        << std::format(format, std::forward<Args>(args)...)
        << std::endl;
}

#define KT_LOG(level, category, format, ...)                     \
    if constexpr (KT_SHOULD_LOG(level))                         \
    {                                                           \
        log_internal(category, __FUNCTION__, format, __VA_ARGS__);  \
    }

#else
#define KT_LOG(level, format, ...)
#endif // _DEBUG