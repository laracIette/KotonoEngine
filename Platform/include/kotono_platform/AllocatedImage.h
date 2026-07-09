#pragma once
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UAllocatedImage
{
	VkImage image;
	VmaAllocation allocation;
	VkImageView imageView;
};