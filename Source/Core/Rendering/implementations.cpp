#include <kotono_common/LogImportanceLevel.h>

#ifndef NDEBUG

#ifndef VMA_LOG_IMPORTANCE_LEVEL
#define VMA_LOG_IMPORTANCE_LEVEL ELogImportanceLevel::Low
#endif

#define VMA_DEBUG_LOG(format, ...)                            \
	if constexpr (KT_SHOULD_LOG(VMA_LOG_IMPORTANCE_LEVEL))    \
    {                                                         \
		printf("[VMA] " format "\n", __VA_ARGS__);            \
	}

#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1 

#endif

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>