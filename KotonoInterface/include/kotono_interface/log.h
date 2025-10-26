#pragma once
#include <kotono_framework/log.h>

#ifdef EDITOR
#include <cstdio>

#define KT_LOG_KI(level, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[KI] " format "\n", ##__VA_ARGS__);  \
    }                                                  

#else
#define KT_LOG_KI(level, format, ...)
#endif