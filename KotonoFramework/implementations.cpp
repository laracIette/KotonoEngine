#define VMA_DEBUG_LOG(format, ...) printf(format "\n", __VA_ARGS__)
#define VMA_DEBUG_MARGIN 16
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>