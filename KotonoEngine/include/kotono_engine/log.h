#pragma once
#include <kotono_framework/log.h>

#ifdef EDITOR
#include <cstdio>

#define KT_LOG_KE(level, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[KE] " format "\n", ##__VA_ARGS__);  \
    }                                                  

#else
#define KT_LOG_KE(level, format, ...)
#endif