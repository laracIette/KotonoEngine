#pragma once
#include "vk_utils.h"
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
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
struct UAllocatedImage
{
	VkImage image;
	VmaAllocation allocation;
	VkImageView imageView;

	void Create(VkDevice device, VmaAllocator allocator, UAllocatedImageCreateInfo const& createInfo)
	{
		CreateImage(allocator, createInfo);
		CreateImageView(device, createInfo);
	}

	void CreateImage(VmaAllocator allocator, UAllocatedImageCreateInfo const& createInfo)
	{
		const VkImageCreateInfo imageInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = createInfo.imageType,
			.format = createInfo.format,
			.extent{
				.width = createInfo.extentX,
				.height = createInfo.extentY,
				.depth = 1,
			},
			.mipLevels = createInfo.mipLevels,
			.arrayLayers = createInfo.arrayLayers,
			.samples = createInfo.numSamples,
			.tiling = createInfo.tiling,
			.usage = createInfo.usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};

		VmaAllocationCreateInfo const allocCreateInfo{
			.usage = VMA_MEMORY_USAGE_AUTO,
			.requiredFlags = createInfo.properties,
		};

		VK_CHECK_THROW(
			vmaCreateImage(allocator, &imageInfo, &allocCreateInfo, &image, &allocation, nullptr),
			"failed to create image with memory allocation!"
		);
	}

	void CreateImageView(VkDevice device, UAllocatedImageCreateInfo const& createInfo)
	{
		const VkImageViewCreateInfo viewInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image,
			.viewType = createInfo.viewType,
			.format = createInfo.format,
			.subresourceRange{
				.aspectMask = createInfo.aspect,
				.baseMipLevel = 0,
				.levelCount = createInfo.mipLevels,
				.baseArrayLayer = 0,
				.layerCount = createInfo.arrayLayers,
			},
		};

		VK_CHECK_THROW(
			vkCreateImageView(device, &viewInfo, nullptr, &imageView),
			"failed to create texture image view!"
		);
	}

	void Cleanup(VkDevice device, VmaAllocator allocator) const
	{
		CleanupImage(allocator);
		CleanupImageView(device);
	}

	void CleanupImage(VmaAllocator allocator) const
	{
		vmaDestroyImage(allocator, image, allocation);
	}

	void CleanupImageView(VkDevice device) const
	{
		vkDestroyImageView(device, imageView, nullptr);
	}
};
