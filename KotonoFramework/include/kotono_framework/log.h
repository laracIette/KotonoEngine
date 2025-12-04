#pragma once

#include "LogImportanceLevel.h"

#ifdef _DEBUG
#include <cstdio>

#define KT_LOG_KF(level, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[KF] " format "\n", ##__VA_ARGS__);  \
    }                                                  

#else
#define KT_LOG_KF(level, format, ...)
#endif