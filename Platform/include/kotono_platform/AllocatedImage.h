#pragma once
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UAllocatedImage
{
	VkImage image;
	VmaAllocation allocation;
	VkImageView imageView;
};

struct UAllocatedImageCreateInfo
{
	u32 extentX;
	u32 extentY;
	u32 extentZ;
	u32 arrayLayers;
	u32 mipLevels;
	VkFormat format;
	VkImageUsageFlags usage;
	VkImageType imageType;
	VkImageViewType viewType;
	VkImageAspectFlags aspect;
	VkSampleCountFlagBits numSamples;
	VkImageTiling tiling;
	VkMemoryPropertyFlags properties;

	static UAllocatedImageCreateInfo CreateSampled2D(
		  const u32 width
		, const u32 height
		, const u32 mipLevels
		, const VkFormat format
		, const VkImageUsageFlags usage
		, const VkImageAspectFlags aspect
		, const VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D
		, const u32 arrayLayers = 1
	)
	{
		return {
			.extentX = width,
			.extentY = height,
			.extentZ = 1,
			.arrayLayers = arrayLayers,
			.mipLevels = mipLevels,
			.format = format,
			.usage = VK_IMAGE_USAGE_SAMPLED_BIT | usage,
			.imageType = VK_IMAGE_TYPE_2D,
			.viewType = viewType,
			.aspect = aspect,
			.numSamples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		};
	}
};
