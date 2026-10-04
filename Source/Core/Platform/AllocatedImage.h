#pragma once
#include "vk_utils.h"
#include <kotono_common/types.h>
#include <stdexcept>
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

	void GenerateMipmaps(VkCommandBuffer commandBuffer, VkFormat format, i32 width, i32 height, u32 mipLevels) const
	{
		VkImageMemoryBarrier barrier{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = image,
			.subresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		i32 mipWidth = width;
		i32 mipHeight = height;

		for (u32 i{ 1 }; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			const VkImageBlit blit{
				.srcSubresource{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel = i - 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
				.srcOffsets = {
					{ 0, 0, 0 },
					{ mipWidth, mipHeight, 1 }
				},
				.dstSubresource{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel = i,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
				.dstOffsets = {
					{ 0, 0, 0 },
					{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }
				},
			};

			vkCmdBlitImage(commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR
			);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

	void TransitionLayout(VkCommandBuffer commandBuffer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels) const
	{
		VkImageMemoryBarrier2 barrier{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.oldLayout = oldLayout,
			.newLayout = newLayout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = image,
			.subresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = mipLevels,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_2_NONE;
			barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;

			barrier.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
			barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;

			barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
			barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_2_NONE;
			barrier.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			barrier.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
			barrier.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::invalid_argument{ "unsupported layout transition!" };
		}

		VkDependencyInfo const depInfo{
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.dependencyFlags = 0,
			.memoryBarrierCount = 0,
			.pMemoryBarriers = VK_NULL_HANDLE,
			.bufferMemoryBarrierCount = 0,
			.pBufferMemoryBarriers = VK_NULL_HANDLE,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &barrier,
		};

		vkCmdPipelineBarrier2(commandBuffer, &depInfo);
	}
};
