#pragma once
#include <vma/vk_mem_alloc.h>
struct UAllocatedBuffer final
{
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo allocationInfo;
};

