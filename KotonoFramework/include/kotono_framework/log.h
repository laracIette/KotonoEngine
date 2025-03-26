#pragma once
#ifndef KT_DEBUG_LOG_DISABLED
#ifdef _DEBUG
#include <cstdio>
#define KT_DEBUG_LOG(format, ...) printf("[DEBUG] " format "\n", ##__VA_ARGS__)
#else
#define KT_DEBUG_LOG(format, ...)
#endif
#else
#define KT_DEBUG_LOG(format, ...)
#endif