#pragma once
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UAllocatedBuffer final
{
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo allocationInfo;
	VkDeviceAddress bda;
};

