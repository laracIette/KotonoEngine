#pragma once

#include "LogImportanceLevel.h"

#ifdef _DEBUG
#include <cstdio>

#define KT_LOG(level, category, format, ...)                \
    if constexpr (KT_SHOULD_LOG(level))              \
    {                                                \
        printf("[" category "] " format "\n", ##__VA_ARGS__);  \
    }

#else
#define KT_LOG(level, category, format, ...)
#endif // _DEBUG