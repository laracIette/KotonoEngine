#pragma once
#ifdef _DEBUG 
#define KT_DEBUG_LOG(format, ...) printf("[DEBUG] " format "\n", ##__VA_ARGS__)
#else
#define KT_DEBUG_LOG(format, ...)
#endif