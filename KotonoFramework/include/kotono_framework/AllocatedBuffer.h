#pragma once
#include <vma/vk_mem_alloc.h>
struct KtAllocatedBuffer final
{
	VkBuffer Buffer;
	VmaAllocation Allocation;
	VmaAllocationInfo AllocationInfo;
};

