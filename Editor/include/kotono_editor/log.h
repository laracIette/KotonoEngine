#pragma once

#include <kotono_common/LogImportanceLevel.h>

#ifdef _DEBUG
#include <cstdio>

#define KT_LOG_KED(level, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[KEI] " format "\n", ##__VA_ARGS__);  \
    }                                                  

#else
#define KT_LOG_KED(level, format, ...)
#endif
