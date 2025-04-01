#define VMA_DEBUG_LOG(format, ...) printf("[VMA] " format "\n", __VA_ARGS__)
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>