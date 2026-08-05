#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>

namespace Barriers 
{
	void CmdTransitionImages(VkCommandBuffer commandBuffer
		, const VkImage* image
		, const u32 count
		, const VkPipelineStageFlags2 srcStage
		, const VkPipelineStageFlags2 dstStage
		, const VkAccessFlags2 srcAccess
		, const VkAccessFlags2 dstAccess
		, const VkImageLayout oldLayout
		, const VkImageLayout newLayout
		, const VkImageSubresourceRange subresourceRange);

	void CmdTransitionCompute(VkCommandBuffer commandBuffer
		, const VkPipelineStageFlags2 srcStage
		, const VkPipelineStageFlags2 dstStage
		, const VkAccessFlags2 srcAccess
		, const VkAccessFlags2 dstAccess);
}
