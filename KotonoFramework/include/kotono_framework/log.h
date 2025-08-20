#pragma once

#include "LogImportanceLevel.h"

#ifndef KT_LOG_COMPILE_TIME_LEVEL
#define KT_LOG_COMPILE_TIME_LEVEL KT_LOG_IMPORTANCE_LEVEL_HIGH
#endif

#ifdef _DEBUG
#include <cstdio>

// Internal helper for log level filtering
#define KT_SHOULD_LOG(level) ((level) >= KT_LOG_COMPILE_TIME_LEVEL)

#define KT_LOG_KF(level, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[KF] " format "\n", ##__VA_ARGS__);  \
    }                                                  

#else
#define KT_LOG_KF(level, format, ...)
#define KT_SHOULD_LOG(level) false
#endif