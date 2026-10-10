#ifndef NDEBUG

static constexpr auto LOG_VMA{ false };

#define VMA_DEBUG_LOG(format, ...) do					\
	{													\
		if constexpr (LOG_VMA)							\
		{												\
			printf("[VMA] " format "\n", __VA_ARGS__);	\
		}												\
	} while (false)

#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1 

#endif

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>