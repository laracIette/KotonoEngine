#pragma once
#include <span>
#include <vulkan/vulkan_core.h>

namespace Barriers 
{
	void CmdTransitionImages(
		  VkCommandBuffer const commandBuffer
		, std::span<VkImage const> const images
		, VkPipelineStageFlags2 const srcStage
		, VkPipelineStageFlags2 const dstStage
		, VkAccessFlags2 const srcAccess
		, VkAccessFlags2 const dstAccess
		, VkImageLayout const oldLayout
		, VkImageLayout const newLayout
		, VkImageSubresourceRange const subresourceRange
	);

	void CmdTransitionCompute(
		  VkCommandBuffer const commandBuffer
		, VkPipelineStageFlags2 const srcStage
		, VkPipelineStageFlags2 const dstStage
		, VkAccessFlags2 const srcAccess
		, VkAccessFlags2 const dstAccess
	);
}
