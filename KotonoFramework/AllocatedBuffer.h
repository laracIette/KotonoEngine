#pragma once
#include <vk_mem_alloc.h>
struct KtAllocatedBuffer
{
	VkBuffer Buffer;
	VmaAllocation Allocation;
	VmaAllocationInfo AllocationInfo;
};

