#pragma once

#include "LogImportanceLevel.h"

#ifndef KT_LOG_COMPILE_TIME_LEVEL
#define KT_LOG_COMPILE_TIME_LEVEL KT_LOG_IMPORTANCE_LEVEL_HIGH
#endif

#ifndef KT_DEBUG_LOG_DISABLED
#ifdef _DEBUG
#include <cstdio>

// Internal helper for log level filtering
#define KT_SHOULD_LOG(level) ((level) >= KT_LOG_COMPILE_TIME_LEVEL)

// Main logging macro with compile-time filtering
#define KT_DEBUG_LOG(level, format, ...)               \
    if constexpr (KT_SHOULD_LOG(level))                \
    {                                                  \
        printf("[DEBUG] " format "\n", ##__VA_ARGS__); \
    }                                                  

#else
#define KT_DEBUG_LOG(level, format, ...)
#endif
#else
#define KT_DEBUG_LOG(level, format, ...)
#endif